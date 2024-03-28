#include <Arduino.h>

const int MOSFET1_PIN = 2;  // Connect MOSFET 1 to digital pin 2
const int MOSFET2_PIN = 3;  // Connect MOSFET 2 to digital pin 3
const int SWITCH_PIN = 4;   // Connect switch to digital pin 4

enum SwitchState {
  WAITING_FOR_PRESS,
  MOSFETS_ON,
  WAITING_FOR_RELEASE,
  MOSFETS_OFF
};

SwitchState switchState = WAITING_FOR_PRESS;

void setup() {
  pinMode(MOSFET1_PIN, OUTPUT);
  pinMode(MOSFET2_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);  // Enable internal pull-up resistor for the switch
}

void loop() {
  switch (switchState) {
    case WAITING_FOR_PRESS:
      if (digitalRead(SWITCH_PIN) == LOW) {
        switchState = MOSFETS_ON;
        digitalWrite(MOSFET1_PIN, HIGH);
        digitalWrite(MOSFET2_PIN, LOW);
      }
      break;
      
    case MOSFETS_ON:
      if (digitalRead(SWITCH_PIN) == HIGH) {
        switchState = WAITING_FOR_RELEASE;
      }
      break;
      
    case WAITING_FOR_RELEASE:
      if (digitalRead(SWITCH_PIN) == HIGH) {
        switchState = MOSFETS_OFF;
        digitalWrite(MOSFET1_PIN, LOW);
        digitalWrite(MOSFET2_PIN, LOW);
      }
      break;
      
    case MOSFETS_OFF:
      if (digitalRead(SWITCH_PIN) == LOW) {
        switchState = WAITING_FOR_PRESS;
      }
      break;
  }
}
