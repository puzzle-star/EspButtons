/* ---------------------------------------------------------------------------------------------------------------------
//  EspButtons - Simple button click event manager for ESP32
//  (c) 2025, Pedro Garcia
--------------------------------------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------------------------
//  This example shows how to add different callback actions to a button. You can also add more buttons as needed, 
//  which would use the same set of callbacks. If you need different callbacks for different buttons, please see the 
//  other example.
--------------------------------------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------------------------
//  Available events:
/-----------------------------------------------------------------------------------------------------------------------
//
//  * Button Clicked:
//
//    Called when the button is pressed and released, and after no more clicks (i.e. double clicks) are detected.
//
//    This event is **not** called when the button is long-pressed (for more than 1s). Use the 'Release' event instead
//    for long presses.
//
//    Event data:
//     - Button: Button that triggered the event.
//     - Clicks: Number of clicks in current sequence of clicks (1 for first click, 2 for double click, etc).
//
// ---------------------------------------------------------------------------------------------------------------------
//
//  * Button Pressed:
//
//    Called immediately when the button is pressed. You will normally want to use 'Click' event instead.
//
//    Event data:
//     - Button: Button that triggered the event.
//     - Clicks: Number of clicks in current sequence of clicks (1 for first click, 2 for double click, etc).
//
// ---------------------------------------------------------------------------------------------------------------------
//
//  * Button Long Press:
//
//    Called when button is left pressed (after 1 second), and every other second while not released.
//
//    Event data:
//     - Button: Button that triggered the event.
//     - Clicks: Number of clicks in current click chain (1 for first click, 2 for double click, etc).
//     - Count: Number of times the long press event has been called since the button was pressed.
//     - Duration: Current total duration of the long press, in ms.
//
// ---------------------------------------------------------------------------------------------------------------------
//
//  * Button Released:
//
//    Called immediately when the button is released. You will normally want to use 'Click' event instead, except 
//    for long presses.
//
//    Event data:
//     - Button: Button that triggered the event.
//     - Clicks: Number of clicks in current sequence of clicks (1 for first click, 2 for double click, etc).
//     - Long Press: Number of times the long press event has been called since the button was pressed (zero for clicks)
//     - Duration: Total time elapsed since the button was pressed, in ms.
//
// ------------------------------------------------------------------------------------------------------------------ */


/* ---------------------------------------------------------------------------------------------------------------------
//  Sample code
--------------------------------------------------------------------------------------------------------------------- */

#include <EspButtons.h>

#define BUTTON_PIN   9    // Pin 9 is the "Boot" button in ESP32-C6 board. Pick any available button in your board
// #define BUTTON_PIN_2 9    // Pick another available pin in your card. Using the same won't work


/* ---------------------------------------------------------------------------------------------------------------------
//  Callback examples
--------------------------------------------------------------------------------------------------------------------- */

void buttonPressed(uint8_t button, uint8_t clicks) {
  Serial.printf("Button pressed: [%u] (#%u)\n",button, clicks);
}

void buttonLongPress(uint8_t button, uint8_t clicks, uint8_t count, uint32_t duration) {
  Serial.printf("Button long press: [%u] (#%u, %u, %lums)\n", button, clicks, count, duration);
}

void buttonReleased(uint8_t button, uint8_t clicks, uint8_t long_press, uint32_t duration) {
  Serial.printf("Button released: [%u] (#%u, %s, %lums)\n", button, clicks, long_press ? "long press" : "click", duration);
}

void buttonClicked(uint8_t button, uint8_t clicks) {
  Serial.printf("Button clicked: [%u] (#%u)\n", button, clicks);
}


/* ---------------------------------------------------------------------------------------------------------------------
//  Sample setup
--------------------------------------------------------------------------------------------------------------------- */

void setup() {
  Serial.begin(115200);


  // Add as many buttons as necessary
  // For the same Buttons object all added buttons will use the same set of callbacks

  Buttons.add(BUTTON_PIN);
  // Buttons.add(BUTTON_PIN_2);


  // Set-up the callbacks for Button events

  Buttons.onButtonPressed(buttonPressed);
  Buttons.onButtonLongPress(buttonLongPress);
  Buttons.onButtonReleased(buttonReleased);
  Buttons.onButtonClicked(buttonClicked);


  // Start the EspButton background task.

  Buttons.start();  
}

void loop() {
  // EspButtons runs in a background task so no need to add anything in loop
}

