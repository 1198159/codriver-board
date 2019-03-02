#include <Joystick.h>

//https://github.com/LordNuke/ArduinoLibs/blob/master/Joystick/examples/JSTest/JSTest.ino

void setup() {
  // put your setup code here, to run once:
  Joystick.clearState();
}

void loop() {
  // put your main code here, to run repeatedly:
  Joystick.clearState();
  // Set the joystick state
  Joystick.state.x.axis = -32768;
  Joystick.state.buttons.b00 = 1;

  // Send the joystick state
  Joystick.sendState();
  delay(50);
}
