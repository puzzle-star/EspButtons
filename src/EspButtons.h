#ifndef __ESPBUTTONS_H__
#define __ESPBUTTONS_H__

#include <stdint.h>

extern class EspButtons & Buttons;

class EspButtons {
	template<typename T> static void event(T);

	void (*Pressed)(uint8_t button, uint8_t clicks) = nullptr;
	void (*LongPress)(uint8_t button, uint8_t clicks, uint8_t count, uint32_t duration) = nullptr;
	void (*Released)(uint8_t button, uint8_t clicks, uint8_t long_press, uint32_t duration) = nullptr;
	void (*Clicked)(uint8_t button, uint8_t clicks) = nullptr;

	public:

	EspButtons();
	EspButtons(uint8_t button);

	bool add(uint8_t button);
	void start();

	void onButtonPressed(void (*Pressed)(uint8_t button, uint8_t clicks)) {
		this->Pressed = Pressed;
	}

	void onButtonLongPress(void (*LongPress)(uint8_t button, uint8_t clicks, uint8_t count, uint32_t duration)) {
		this->LongPress = LongPress;
	}

	void onButtonReleased(void (*Released)(uint8_t button, uint8_t clicks, uint8_t long_press, uint32_t duration)) {
		this->Released = Released;
	}

	void onButtonClicked(void (*Clicked)(uint8_t button, uint8_t clicks)) {
		this->Clicked = Clicked;
	}

};

#endif // __ESPBUTTONS_H__
