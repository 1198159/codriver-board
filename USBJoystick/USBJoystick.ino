#include <Joystick.h>

//BUTTON PORTS

//CLIMB
//INPUTS
const int climbTop = 22;
const int climbMid = 24;
const int climbBot = 26;

//OUTPUTS
const int climbTop_O = 0;
const int climbMid_O = 1;
const int climbBot_O = 2;
//MODES
const int indexSpit = 23;
const int climbMode = 25;
const int manualShooter = 44;

const int indexSpit_O = 3;
const int climbMode_O = 4;
const int manualShooter_O = 7;

//INTAKE
//INPUTS
const int intakeFront = 34;
const int intakeBack = 35;

//OUTPUTS
const int intakeFront_O = 0;
const int intakeBack_O = 1;

//FIRST VAL IS ON, SECOND IS OFF
//INPUTS
const int intakeInOutFront[] = {36, 37};
const int intakeInOutBack[] = {38, 39};

//OUTPUTS
const int intakeInOutFront_O[] = {2, 3};
const int intakeInOutBack_O[] = {4, 5};

//LIFT
//INPUTS
const int lift = 40;

//OUTPUTS
const int lift_O = 6;
//DIAL
const int dial = 2;
//slide
const int slide = 3;

//CLICKY DIAL
const int thingy[] = {53, 50, 49, 47, 45, 43};
const int thingy_O[] = {5,6,7,8,9,10};
//STICK
 const int stick[] = {1, 0};

//const 
// Range values
const int minAxisVal = -32768;
const int maxAxisVal = 32767;

const int minAnalogVal = 0;
const int maxAnalogVal = 1023;

const int midAnalogXVal = 800;
const int midAnalogYVal = 800;
const int midAnalogZVal = 511;

//ARRAY OF INPUTS
const int mainDigitalInputs[] = {climbTop, climbMid, climbBot, indexSpit, climbMode, thingy[0], thingy[1], thingy[2], thingy[3], thingy[4], thingy[5]};
const int secondaryDigitalInputs[] = {intakeFront, intakeBack, intakeInOutFront[0], intakeInOutFront[1], intakeInOutBack[0], intakeInOutBack[1], lift, manualShooter};

// Arrays for determing which digital USB outputs each joystick should use.
const int mainDigitalOutputs[] = {climbTop_O, climbMid_O, climbBot_O, indexSpit_O, climbMode_O, thingy_O[0], thingy_O[1], thingy_O[2], thingy_O[3], thingy_O[4], thingy_O[5]};

const int secondaryDigitalOutputs[] = {intakeFront_O, intakeBack_O, intakeInOutFront_O[0], intakeInOutFront_O[1], intakeInOutBack_O[0], intakeInOutBack_O[1], lift_O, manualShooter_O};

// How many buttons (and digital inputs) we will use.
const unsigned int MAIN_MAX_BUTTONS = sizeof(mainDigitalInputs) / sizeof(int);
const unsigned int SECONDARY_MAX_BUTTONS = sizeof(secondaryDigitalInputs) / sizeof(int);

Joystick_ mainJoystick(0x03, JOYSTICK_TYPE_JOYSTICK, MAIN_MAX_BUTTONS, 0, true, true, false, false, false, false, false, false, false, false, false);
Joystick_ secondaryJoystick(0x04, JOYSTICK_TYPE_JOYSTICK, SECONDARY_MAX_BUTTONS, 0, true, true, false, false, false, false, false, false, false, false, false);

// Last state of the button
int *mainLastButtonState;

int *secondaryLastButtonState;

// Starting hat digital input ID for the main joystick
int mainHatDigitalInputID = 33;

void setup() {
  // put your setup code here, to run once:

  // Initialize button state arrays
  int index;

  mainLastButtonState = (int*) calloc(MAIN_MAX_BUTTONS, sizeof(int));
  secondaryLastButtonState = (int*) calloc(SECONDARY_MAX_BUTTONS, sizeof(int));
  for (index = 0; index < MAIN_MAX_BUTTONS; index++) {
    mainLastButtonState[index] = 0;
  }
  for (index = 0; index < SECONDARY_MAX_BUTTONS; index++) {
    secondaryLastButtonState[index] = 0;
  }
  // Initialize Joystick Library
  mainJoystick.setXAxisRange(minAxisVal, maxAxisVal);
  mainJoystick.setYAxisRange(minAxisVal, maxAxisVal);
  secondaryJoystick.setXAxisRange(minAxisVal, maxAxisVal);
  secondaryJoystick.setYAxisRange(minAxisVal, maxAxisVal);
  mainJoystick.begin();
  secondaryJoystick.begin();

  // Initialize button pins
  for (int buttonID = 0; buttonID < MAIN_MAX_BUTTONS; buttonID++) {
    pinMode(mainDigitalInputs[buttonID], INPUT_PULLUP);
  }
  for (int buttonID = 0; buttonID < SECONDARY_MAX_BUTTONS; buttonID++) {
    pinMode(secondaryDigitalInputs[buttonID], INPUT_PULLUP);
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
  if (rawData >= midVal) {
    convertedData = convertRange(rawData, midVal, maxAnalogVal, 0, maxAxisVal);
  } else {
    convertedData = convertRange(rawData, minAnalogVal, midVal, minAxisVal, 0);
  }

  //  Serial.println(rawData);
  //  Serial.println(convertedData);

  return convertedData;
}
void loop() {
  Serial.println(analogRead(slide));
  // Set the joystick state
  int rawXAxisData = analogRead(stick[1]);
  mainJoystick.setXAxis(analogToStick(rawXAxisData, midAnalogXVal));

  int rawYAxisData = analogRead(stick[0]);
  mainJoystick.setYAxis(-analogToStick(rawYAxisData, midAnalogYVal) - 1); // Flips axis and accounts for off by one error.

    // Set the joystick state
  int rawZAxisData = analogRead(dial);
  secondaryJoystick.setXAxis(analogToStick(rawZAxisData, midAnalogZVal));

  int rawZAxisData2 = analogRead(slide);
  secondaryJoystick.setYAxis(analogToStick(rawZAxisData2, midAnalogZVal));

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
  for (int index = 0; index < SECONDARY_MAX_BUTTONS; index++)
  {
    int currentButtonState = !digitalRead(secondaryDigitalInputs[index]);
    if (currentButtonState != secondaryLastButtonState[index])
    {
      secondaryJoystick.setButton(secondaryDigitalOutputs[index], currentButtonState);
      secondaryLastButtonState[index] = currentButtonState;
    }
  }
  delay(50);
}
