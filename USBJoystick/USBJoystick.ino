#include <Joystick.h>

//https://github.com/LordNuke/ArduinoLibs/blob/master/Joystick/examples/JSTest/JSTest.ino

// Constant for determing which digital input to use.
const int digitalInputID = 22;

// How many buttons (and digital inputs) we will use.
const unsigned int MAX_BUTTONS = 8;

void setup() {
  // put your setup code here, to run once:
  Joystick.clearState();

  // Initialize button pins
  for(int buttonID = 0; buttonID < MAX_BUTTONS; buttonID++) {
    pinMode(digitalInputID + buttonID, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Joystick.clearState();
  // Set the joystick state
  Joystick.state.x.axis = -32768;

  // Set the joystick buttons according to the respective IDs
  for(int buttonID = 0; buttonID < MAX_BUTTONS; buttonID++) {
    setButton(buttonID, digitalInputID + buttonID);
  }

  Joystick.sendState();
  delay(50);
}

void setButton(int buttonID, int inputID) {
  Joystick.state.buttons.bitfield |= (!digitalRead(inputID) << buttonID);
}
