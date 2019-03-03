#include <Joystick.h>

//https://github.com/LordNuke/ArduinoLibs/blob/master/Joystick/examples/JSTest/JSTest.ino

// Constant for determing which digital input to use.
const int digitalInputID = 9;

void setup() {
  // put your setup code here, to run once:
  Joystick.clearState();

  // Initialize button pins
  pinMode(digitalInputID, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Joystick.clearState();
  // Set the joystick state
  Joystick.state.x.axis = -32768;
  Joystick.state.buttons.b00 = !digitalRead(digitalInputID);

  // Send the joystick state
  Joystick.sendState();
  delay(50);
}
