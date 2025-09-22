#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <stdint.h>

typedef enum button_event_type:uint8_t {
	BUTTON_EVENT_NONE       = 0x00,
	BUTTON_EVENT_PRESSED    = 0x01,
	BUTTON_EVENT_LONG_PRESS = 0x02,
	BUTTON_EVENT_RELEASED   = 0x03,
	BUTTON_EVENT_CLICKED    = 0x04,
	BUTTON_EVENT_UNKNOWN    = 0xff
} button_event_type_t;

typedef struct button_event {
	button_event_type_t type;
	uint8_t button;
	void * args;

	union {
		struct {
			uint8_t clicks;
		} pressed;

		struct {
			uint8_t clicks;
			uint8_t count;
			uint32_t duration;
		} long_press;

		struct {
			uint8_t clicks;
			uint8_t long_press;
			uint32_t duration;
		} released;

		struct {
			uint8_t clicks;
		} clicked;
	};
} button_event_t;


bool buttons_add(uint8_t button, void (*callback)(const button_event_t * event), void * args = nullptr);

void buttons_start();


#endif // __BUTTONS_H__
