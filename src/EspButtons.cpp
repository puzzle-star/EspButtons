#include "EspButtons.h"
#include "buttons.h"

EspButtons & Buttons = * new EspButtons();

EspButtons::EspButtons() {
}

EspButtons::EspButtons(uint8_t button) {
	add(button);
}

// template<> void EspButtons::event<const button_event_t *>(const button_event_t * event) {
template<> void EspButtons::event(const button_event_t * event) {
	EspButtons * buttons = static_cast<EspButtons *>(event->args);

	switch (event->type) {
		case BUTTON_EVENT_PRESSED:
			if (buttons->Pressed) {
				buttons->Pressed(event->button, event->pressed.clicks);
			}
			break;

		case BUTTON_EVENT_LONG_PRESS:
			if (buttons->LongPress) {
				buttons->LongPress( event->button, event->long_press.clicks, event->long_press.count, event->long_press.duration);
			}
			break;

		case BUTTON_EVENT_RELEASED:
			if (buttons->Released) {
				buttons->Released(event->button, event->released.clicks, event->released.long_press, event->released.duration);
			}
			break;

		case BUTTON_EVENT_CLICKED:
			if (buttons->Clicked) {
				buttons->Clicked(event->button, event->clicked.clicks);
			}
			break;

		default:
			break;
	}
}

bool EspButtons::add(uint8_t button) {
	return buttons_add(button, &EspButtons::event<const button_event_t *>, this);
}

void EspButtons::start() {
	buttons_start();
}

