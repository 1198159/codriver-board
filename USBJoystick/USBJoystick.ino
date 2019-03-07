#include <Joystick.h>

Joystick_ Joystick(0x03, JOYSTICK_TYPE_JOYSTICK, 20, 2, true, true, false, false, false, false, false, false, false, false, false);

// Constant for determing which digital input to use.
const int digitalInputID = 22;

// How many buttons (and digital inputs) we will use.
const unsigned int MAX_BUTTONS = 8;

// Last state of the button
int lastButtonState[8] = {0,0,0,0,0,0,0,0};

// The analog joystick pins.
int joyPin0 = 0;
int joyPin1 = 1;
int median = 606;

// Range values
int minAxisVal = -32768;
int maxAxisVal = 32767;

int minAnalogVal = 0;
int maxAnalogVal = 1023;

int midAnalogXVal = 700;
int midAnalogYVal = 530;

void setup() {
  // put your setup code here, to run once:

  // Initialize Joystick Library
  Joystick.setXAxisRange(minAxisVal, maxAxisVal);
  Joystick.setYAxisRange(minAxisVal, maxAxisVal);
  Joystick.begin();

  // Initialize button pins
  for(int buttonID = 0; buttonID < MAX_BUTTONS; buttonID++) {
    pinMode(digitalInputID + buttonID, INPUT_PULLUP);
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
  int rawXAxisData = analogRead(joyPin0);
  Joystick.setXAxis(analogToStick(rawXAxisData, midAnalogXVal));

  int rawYAxisData = analogRead(joyPin1);
  Joystick.setYAxis(analogToStick(rawYAxisData, midAnalogYVal));

  // Read pin values
  for (int index = 0; index < MAX_BUTTONS; index++)
  {
    int currentButtonState = !digitalRead(index + digitalInputID);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
    }
  }

  delay(50);
}
