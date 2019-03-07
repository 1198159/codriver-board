#include <Joystick.h>

// How many buttons (and digital inputs) we will use.
const unsigned int MAIN_MAX_BUTTONS = 18;
const unsigned int DIAL_MAX_BUTTONS = 8;

Joystick_ mainJoystick(0x03, JOYSTICK_TYPE_JOYSTICK, MAIN_MAX_BUTTONS, 1, true, true, false, false, false, false, false, false, false, false, false);
Joystick_ dialJoystick(0x04, JOYSTICK_TYPE_JOYSTICK, DIAL_MAX_BUTTONS, 1, false, false, false, false, false, false, false, false, false, false, false);

// Constant for determing which digital input to use.
const int mainDigitalInputID = 24;
const int dialDigitalInputID = 6;

// Last state of the button
int *mainLastButtonState;
int *dialLastButtonState;

// The analog joystick pins.
int joyPin0 = 0;
int joyPin1 = 1;

// Range values
int minAxisVal = -32768;
int maxAxisVal = 32767;

int minAnalogVal = 0;
int maxAnalogVal = 1023;

int midAnalogXVal = 574;
int midAnalogYVal = 547;

// Starting hat digital input ID for the main joystick
int mainHatDigitalInputID = 33;

void setup() {
  // put your setup code here, to run once:

  // Initialize button state arrays
  int index;
  
  mainLastButtonState = (int*) calloc(MAIN_MAX_BUTTONS, sizeof(int));
  dialLastButtonState = (int*) calloc(DIAL_MAX_BUTTONS, sizeof(int));

  for(index = 0; index < MAIN_MAX_BUTTONS; index++) {
    mainLastButtonState[index] = 0;
  }

  for(index = 0; index < DIAL_MAX_BUTTONS; index++) {
    dialLastButtonState[index] = 0;
  }
  
  // Initialize Joystick Library
  mainJoystick.setXAxisRange(minAxisVal, maxAxisVal);
  mainJoystick.setYAxisRange(minAxisVal, maxAxisVal);
  mainJoystick.begin();

  dialJoystick.setXAxisRange(minAxisVal, maxAxisVal);
  dialJoystick.setYAxisRange(minAxisVal, maxAxisVal);
  dialJoystick.begin();

  // Initialize button pins
  for(int buttonID = 0; buttonID < MAIN_MAX_BUTTONS; buttonID++) {
    pinMode(mainDigitalInputID + buttonID, INPUT_PULLUP);
  }
  
  for(int buttonID = 0; buttonID < DIAL_MAX_BUTTONS; buttonID++) {
    pinMode(dialDigitalInputID + buttonID, INPUT_PULLUP);
  }
  Serial.begin(9600);
}

// Converts data from one range [a1, b1] to another [a2, b2].
int convertRange(int data, int a1, int b1, int a2, int b2) {
  return (data - a1) * (b2 - a2) / (b1 - a1) + a2;
}

// Converts analog data to values for the usb joystick.
int analogToStick(int rawData, int midVal) {
  int convertedData;
  if(rawData >= midVal) {
    convertedData = convertRange(rawData, midVal, maxAnalogVal, 0, maxAxisVal);
  } else {
    convertedData = convertRange(rawData, minAnalogVal, midVal, minAxisVal, 0);
  }
  
  Serial.println(rawData);
  Serial.println(convertedData);

  return convertedData;
}
void loop() {
  // put your main code here, to run repeatedly:
  
  // Set the joystick state
  int rawXAxisData = analogRead(joyPin1);
  mainJoystick.setXAxis(analogToStick(rawXAxisData, midAnalogXVal));

  int rawYAxisData = analogRead(joyPin0);
  mainJoystick.setYAxis(-analogToStick(rawYAxisData, midAnalogYVal)-1); // Flips axis and accounts for off by one error.

  // Read pin values for the main joystick
  for (int index = 0; index < MAIN_MAX_BUTTONS; index++)
  {
    int currentButtonState = !digitalRead(index + mainDigitalInputID);
    if (currentButtonState != mainLastButtonState[index])
    {
      mainJoystick.setButton(index, currentButtonState);
      mainLastButtonState[index] = currentButtonState;
      if (index + mainDigitalInputID >= mainHatDigitalInputID && index + mainDigitalInputID < mainHatDigitalInputID + 8) {
        mainJoystick.setHatSwitch(0, 45*(mainDigitalInputID + index - mainHatDigitalInputID) + 180);
      }
    }
  }

  // Read pin values for the dial joystick
  for (int index = 0; index < DIAL_MAX_BUTTONS; index++)
  {
    int currentButtonState = !digitalRead(index + dialDigitalInputID);
    if (currentButtonState != dialLastButtonState[index])
    {
      dialJoystick.setButton(index, currentButtonState);
      dialJoystick.setHatSwitch(0, 45*index + 180);
      dialLastButtonState[index] = currentButtonState;
    }
  }

  delay(50);
}
