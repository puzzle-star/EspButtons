#include <Arduino.h>
#include "buttons.h"


#define BUTTON_LONG_PRESS_THRESHOLD   1000
#define BUTTON_LONG_PRESS_UPDATE      1000
#define BUTTON_DOUBLE_CLICK_THRESHOLD  300
#define BUTTON_DEBOUNCE                 50


typedef enum button_handler_state:uint8_t {
	BUTTON_HANDLER_IDLE     = 0x00,
	BUTTON_HANDLER_PRESSED  = 0x01,
	BUTTON_HANDLER_RELEASED = 0x02,
	BUTTON_HANDLER_UNKNOWN  = 0xff,
} button_handler_state_t;

typedef struct button_handler {
	uint8_t button = 0xff;
	void (*callback)(const button_event_t * event) = nullptr;
	void * args = nullptr;
	volatile uint32_t triggered = 0;
	volatile uint32_t time = 0;
	button_handler_state_t state = BUTTON_HANDLER_IDLE;
	uint32_t pressed_time = 0;
	uint32_t released_time = 0;
	uint8_t long_press = 0;
	uint8_t click_count = 0;
	struct button_handler * next = nullptr;
} button_handler_t;


static button_handler_t * button_handler_list = nullptr;

static SemaphoreHandle_t button_gpio_event_sem = nullptr;


static void IRAM_ATTR button_interrupt(void * arg) {
	button_handler * handler = (button_handler_t *) arg;
	uint32_t now = millis() | 1;

	if (!handler->triggered) {
		handler->time = now;
		xSemaphoreGiveFromISR(button_gpio_event_sem, nullptr);
	}

	handler->triggered = now;
}

static void buttons_setup() {
	button_gpio_event_sem = xSemaphoreCreateBinary();
}

static void button_pressed_event(button_handler_t * handler) {
	if (!handler->callback) {
		return;
	}

	button_event_t event = {
		.type = BUTTON_EVENT_PRESSED,
		.button = handler->button,
		.args = handler->args,
		.pressed = {
			.clicks = handler->click_count
		}
	};

	handler->callback(&event);
}

static void button_long_press_event(button_handler_t * handler, uint32_t duration) {
	if (!handler->callback) {
		return;
	}

	button_event_t event = {
		.type = BUTTON_EVENT_LONG_PRESS,
		.button = handler->button,
		.args = handler->args,
		.long_press = {
			.clicks = handler->click_count,
			.count = handler->long_press,
			.duration = duration
		}
	};

	handler->callback(&event);
}

static void button_released_event(button_handler_t * handler) {
	if (!handler->callback) {
		return;
	}

	button_event_t event = {
		.type = BUTTON_EVENT_RELEASED,
		.button = handler->button,
		.args = handler->args,
		.released = {
			.clicks = handler->click_count,
			.long_press = handler->long_press,
			.duration = handler->released_time - handler->pressed_time
		}
	};

	handler->callback(&event);
}

static void button_clicked_event(button_handler_t * handler) {
	if (!handler->callback) {
		return;
	}

	button_event_t event = {
		.type = BUTTON_EVENT_CLICKED,
		.button = handler->button,
		.args = handler->args,
		.clicked = {
			.clicks = handler->click_count
		}
	};

	handler->callback(&event);
}

static void buttons_task(void *) {
	while(true) {
		button_handler_t * handler;
		uint32_t now = millis() | 1;
		TickType_t wait = portMAX_DELAY;

		for (handler = button_handler_list ; handler ; handler = handler->next) {
			uint8_t button = handler->button;
			uint32_t triggered = 0;
			bool debouncing = false;
			bool pressed = digitalRead(button) == LOW;
	
			disableInterrupt(button);
		
			if (handler->triggered) {
				if (now - handler->triggered < BUTTON_DEBOUNCE) {
					debouncing = true;
					wait = pdMS_TO_TICKS(BUTTON_DEBOUNCE);
				}
				else {
					handler->triggered = 0;
					triggered = handler->time;
				}
			}
	
			enableInterrupt(button);

			if (triggered) {
				log_d("Button event: [%u] %u -> %s (@%lums)", button, handler->state, pressed ? "pressed" : "released", triggered);
			}
		
			switch (handler->state) {
				case BUTTON_HANDLER_IDLE:
					if (triggered && pressed) {
						handler->state = BUTTON_HANDLER_PRESSED;
						handler->pressed_time = triggered;
						handler->click_count = 1;
						handler->long_press = 0;

						log_d("Button pressed: %u", button);
						button_pressed_event(handler);

						wait = pdMS_TO_TICKS(BUTTON_DEBOUNCE);
					}
				break;

				case BUTTON_HANDLER_PRESSED:
					if (triggered && !pressed) {
						handler->state = BUTTON_HANDLER_RELEASED;
						handler->released_time = triggered;

						log_d("Button released: %u", button);
						button_released_event(handler);

						if (handler->long_press) {
							handler->state = BUTTON_HANDLER_IDLE;
						}
						else {
							wait = pdMS_TO_TICKS(BUTTON_DEBOUNCE);
						}
		
					}
					else if (!debouncing && pressed) {
						uint32_t duration = now - handler->pressed_time;

						if (duration >= BUTTON_LONG_PRESS_THRESHOLD + BUTTON_LONG_PRESS_UPDATE * (uint32_t) handler->long_press) {
							handler->long_press++;

							log_d("Button long press: %u (#%u, %lums)", button, handler->long_press, duration);
							button_long_press_event(handler, duration);
						}
		
						wait = pdMS_TO_TICKS(BUTTON_DEBOUNCE);
					}
	
				break;

				case BUTTON_HANDLER_RELEASED:
					if (triggered && pressed) {
						handler->state = BUTTON_HANDLER_PRESSED;
						handler->pressed_time = triggered;
						handler->click_count++;

						log_d("Button re-clicked: %u (#%u)", button, handler->click_count);
						button_pressed_event(handler);

						wait = pdMS_TO_TICKS(BUTTON_DEBOUNCE);
					}
					else if (!debouncing && !pressed) {
						if (now - handler->released_time >= BUTTON_DOUBLE_CLICK_THRESHOLD) {
							handler->state = BUTTON_HANDLER_IDLE;

							log_d("Button clicked: %u (#%u)", button, handler->click_count);
							button_clicked_event(handler);
						}
						else {
							wait = pdMS_TO_TICKS(BUTTON_DEBOUNCE);
						}
					}
	
				break;

				default:
				break;
			}
		}

		xSemaphoreTake(button_gpio_event_sem, wait);
	}
}

bool buttons_add(uint8_t button, void (*callback)(const button_event_t * event), void * args) {
	if (!button_gpio_event_sem) {
		buttons_setup();
	}

	button_handler_t * last = button_handler_list;

	while (last) {
		if (button == last->button) {
			return false;
		}

		if (!last->next) {
			break;
		}
		
		last = last->next;
	}

	button_handler_t * handler = new button_handler_t {
		.button = button,
		.callback = callback,
		.args = args
	};

	if (!last) {
		button_handler_list = handler;
	}
	else {
		last->next = handler;
	}

	pinMode(button, INPUT_PULLUP);
	attachInterruptArg(button, button_interrupt, (void *) handler, CHANGE);

	return true;
}

void buttons_start() {
	static bool started = false;

	if (started) {
		return;
	}

	started = true;

	if (!button_gpio_event_sem) {
		buttons_setup();
	}

	xTaskCreate(buttons_task, NULL, 4096, NULL, 5, NULL);
}

