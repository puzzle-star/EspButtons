# EspButtons
Simple button click event manager for Arduino/ESP32

Easily add (single-double-multiple) click and long press events to any input pin in ESP32

## Available events

* **Button Clicked**

  Called when the button is pressed and released, and after no more clicks (i.e. double clicks) are detected.

  This event is **not** called when the button is long-pressed (for more than 1s). Use the 'Release' event instead for long presses.

  **Event data:**
  - Button: Button that triggered the event.
  - Clicks: Number of clicks in current sequence of clicks (1 for first click, 2 for double click, etc).

* **Button Pressed**
  
  Called immediately when the button is pressed. You will normally want to use 'Click' event instead.
  
  **Event data:**
  - Button: Button that triggered the event.
  - Clicks: Number of clicks in current sequence of clicks (1 for first click, 2 for double click, etc).
     
* **Button Long Press**
  
  Called when button is left pressed (after 1 second), and every other second while not released.

  **Event data:**
  - Button: Button that triggered the event.
  - Clicks: Number of clicks in current click chain (1 for first click, 2 for double click, etc).
  - Count: Number of times the long press event has been called since the button was pressed.
  - Duration: Current total duration of the long press, in ms.
     
* **Button Released**

  Called immediately when the button is released. You will normally want to use 'Click' event instead, except for long presses.

  **Event data:**
  - Button: Button that triggered the event.
  - Clicks: Number of clicks in current sequence of clicks (1 for first click, 2 for double click, etc).
  - Long Press: Number of times the long press event has been called since the button was pressed (zero for clicks)
  - Duration: Total time elapsed since the button was pressed, in ms.

## Callback functions

```
void buttonPressed(uint8_t button, uint8_t clicks);
void buttonLongPress(uint8_t button, uint8_t clicks, uint8_t count, uint32_t duration);
void buttonReleased(uint8_t button, uint8_t clicks, uint8_t long_press, uint32_t duration);
void buttonClicked(uint8_t button, uint8_t clicks);
```

## Setup

```
Buttons.add(BUTTON_PIN);

Buttons.onButtonPressed(buttonPressed);
Buttons.onButtonLongPress(buttonLongPress);
Buttons.onButtonReleased(buttonReleased);
Buttons.onButtonClicked(buttonClicked);

Buttons.start();
```

