#include <Joystick.h>

// Arrays for determing which digital inputs each joystick should use.
int mainDigitalInputs[] = {24, 25, 26, 27, 28, 29, 30, 31, 32, 41, 42, 43, 44};
int dialDigitalInputs[] = {6, 7, 8, 9, 10, 11, 12, 13, 33, 34, 35, 36, 37, 38, 39, 40};

// Arrays for determing which digital USB outputs each joystick should use.
int mainDigitalOutputs[] = {7, 6, 5, 10, 9, 8, 7, 6, 5, 1, 0, 2, 3};
int dialDigitalOutputs[] = {0, 1, 2, 3, 4, 5, 6, 7, 11, 13, 12, 10, 9, 8, 15, 14};

// How many buttons (and digital inputs) we will use.
const unsigned int MAIN_MAX_BUTTONS = sizeof(mainDigitalInputs)/sizeof(int);
const unsigned int DIAL_MAX_BUTTONS = sizeof(dialDigitalInputs)/sizeof(int);

Joystick_ mainJoystick(0x03, JOYSTICK_TYPE_JOYSTICK, MAIN_MAX_BUTTONS - 2, 0, true, true, false, false, false, false, false, false, false, false, false);
Joystick_ dialJoystick(0x04, JOYSTICK_TYPE_JOYSTICK, DIAL_MAX_BUTTONS, 2, false, false, false, false, false, false, false, false, false, false, false);

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
    pinMode(mainDigitalInputs[buttonID], INPUT_PULLUP);
  }
  
  for(int buttonID = 0; buttonID < DIAL_MAX_BUTTONS; buttonID++) {
    pinMode(dialDigitalInputs[buttonID], INPUT_PULLUP);
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
    int currentButtonState = !digitalRead(mainDigitalInputs[index]);
    if (currentButtonState != mainLastButtonState[index])
    {
      mainJoystick.setButton(mainDigitalOutputs[index], currentButtonState);
      mainLastButtonState[index] = currentButtonState;
    }
  }

  // Read pin values for the dial joystick
  for (int index = 0; index < DIAL_MAX_BUTTONS; index++)
  {
    int currentButtonState = !digitalRead(dialDigitalInputs[index]);
    if (currentButtonState != dialLastButtonState[index])
    {
      dialJoystick.setButton(dialDigitalOutputs[index], currentButtonState);
      dialJoystick.setHatSwitch(index >= 8, 45*index + 180);
      dialLastButtonState[index] = currentButtonState;
    }
  }

  delay(50);
}
