#include <CoDrone.h> // The codrone library that holds all the background files for this

// Controller offset
int rollOffset = 0;
int pitchOffset = 0;
int yawOffset = 0;
int throttleOffset = 0;

// Controller settings
int RANGEMIN = -50;
int RANGEMAX = 50;
const int ORIRANGE = 200; // original codrone controller scale from -100 to 100.
const int RANGE = RANGEMAX - RANGEMIN;
const int SCALE = ORIRANGE / RANGE;

// Joystick Ports
const byte rollPort = A5;
const byte pitchPort = A6;
const byte yawPort = A3;
const byte throttlePort = A4;


// Setup of the program. Only run once at the beginning
void setup() {
  CoDrone.begin(115200);
  CoDrone.pair(Nearest);
  CoDrone.DroneModeChange(Flight);

  //Serial.begin(9600);

  /**
   * Recalibrate controller.
   * 
   * roll control -- port A5
   * pitch control -- port A6
   * yaw control -- port A3
   * throttle control -- port A4
   */
  rollOffset = CoDrone.AnalogScaleChange(analogRead(rollPort));
  pitchOffset = CoDrone.AnalogScaleChange(analogRead(pitchPort));
  yawOffset = CoDrone.AnalogScaleChange(analogRead(yawPort));
  throttleOffset = CoDrone.AnalogScaleChange(analogRead(throttlePort));
}

/**
 * Reading the value of the joystick of the given port.
 * 
 * @param port -- the port of the joystick
 * @return the manipulated value of the analog joystick input.
 */
int readJoystickValue(byte port) {
  int analogValue = CoDrone.AnalogScaleChange(analogRead(port));
  int result = analogValue;
  switch (port) {
    case A5:
      result = result - rollOffset;
      result = result / SCALE;
      result = -1 * result;
      break;
    case A6:
      result = result - pitchOffset;
      result = result / SCALE;
      break;
    case A3:
      result = result - yawOffset;
      result = result / SCALE;
      result = -1 * result;
      break;
    case A4:
      result = result - throttleOffset;
      result = result / SCALE;
      break;
    default:
      break;
  }
  
  // Ignore if the joystick is at its default position.
  if (result > 2 || result < -2){
    return result;
  } else {
    return 0;
  }
}

void loop() {
  ROLL = readJoystickValue(rollPort);
  PITCH = readJoystickValue(pitchPort);
  YAW = readJoystickValue(yawPort);
  THROTTLE = readJoystickValue(throttlePort);

  int MIDDLESENSOR = analogRead(20);
  int RIGHTSENSOR = analogRead(21);

//  if (ROLL != 0) {
//    Serial.print("ROLL: ");
//    Serial.println(ROLL);
//  } 
//  if (PITCH != 0) {
//    Serial.print("PITCH: ");
//    Serial.println(PITCH);
//  } 
//  if (YAW != 0) {
//    Serial.print("YAW: ");
//    Serial.println(YAW);
//  } 
//  if (THROTTLE != 0) {
//    Serial.print("THROTTLE: ");
//    Serial.println(THROTTLE);
//  } 
  

  //Stop when the MIDDLE butotn is pressed, but none of the others are
  if (MIDDLESENSOR < 100) {
    CoDrone.emergencyStop();
  }

  // try to land when the right button is pressed and no others are
  if (RIGHTSENSOR < 100) {
     CoDrone.land();
  }

  CoDrone.move(ROLL, PITCH, YAW, THROTTLE);
}
