/* ---------------------------------------------------------------------------------------------------------------------
//  EspButtons - Simple button click event manager for ESP32
//  (c) 2025, Pedro Garcia
--------------------------------------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------------------------
//  This example shows how to use different callbacks for different buttons.
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
#define BUTTON_PIN_2 9    // Pick another available pin in your card. Using the same won't work


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

void otherButtonPressed(uint8_t button, uint8_t clicks) {
  Serial.printf("Button pressed (other): [%u] (#%u)\n",button, clicks);
}

void otherButtonLongPress(uint8_t button, uint8_t clicks, uint8_t count, uint32_t duration) {
  Serial.printf("Button long press (other): [%u] (#%u, %u, %lums)\n", button, clicks, count, duration);
}

void otherButtonReleased(uint8_t button, uint8_t clicks, uint8_t long_press, uint32_t duration) {
  Serial.printf("Button released (other): [%u] (#%u, %s, %lums)\n", button, clicks, long_press ? "long press" : "click", duration);
}

void otherButtonClicked(uint8_t button, uint8_t clicks) {
  Serial.printf("Button clicked (other): [%u] (#%u)\n", button, clicks);
}

/* ---------------------------------------------------------------------------------------------------------------------
//  You can use additional EspButton objects with different set of callbacks.
--------------------------------------------------------------------------------------------------------------------- */

EspButtons MyButtons = EspButtons(BUTTON_PIN);
EspButtons MyOtherButtons = EspButtons(BUTTON_PIN_2); // The same button pin can only be added to one EspButton object


/* ---------------------------------------------------------------------------------------------------------------------
//  Sample setup
--------------------------------------------------------------------------------------------------------------------- */

void setup() {
  Serial.begin(115200);


  // Add as many buttons as necessary
  // For the same Buttons object all added buttons will use the same set of callbacks

  // MyButtons.add(BUTTON_PIN); // This is not necessary if we passed a button to the MyButtons constructor
  // MyOtherButtons.add(BUTTON_PIN_2); // This is not necessary if we passed a button to the MyOtherButtons constructor

  // MyButtons.add(BUTTON_PIN_OTHER); // But we can add other buttons as needed


  // Set-up the callbacks for Button events

  MyButtons.onButtonPressed(buttonPressed);
  MyButtons.onButtonLongPress(buttonLongPress);
  MyButtons.onButtonReleased(buttonReleased);
  MyButtons.onButtonClicked(buttonClicked);

  MyOtherButtons.onButtonPressed(otherButtonPressed);
  MyOtherButtons.onButtonLongPress(otherButtonLongPress);
  MyOtherButtons.onButtonReleased(otherButtonReleased);
  MyOtherButtons.onButtonClicked(otherButtonClicked);


  // Start the EspButton background task.
  // It only needs to be called once, even if we created more than one object

  Buttons.start();  
}

void loop() {
  // EspButtons runs in a background task so no need to add anything in loop
}

