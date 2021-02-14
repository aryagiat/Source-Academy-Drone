#include <CoDrone.h> // The codrone library that holds all the background files for this

// Controller offset
int rollOffset = 0;
int pitchOffset = 0;
int yawOffset = 0;
int throttleOffset = 0;

// Controller settings
const int RANGEMIN = -50;
const int RANGEMAX = 50;
const int ORIRANGE = 200; // original codrone controller scale from -100 to 100.
const int RANGE = RANGEMAX - RANGEMIN;
const int SCALE = ORIRANGE / RANGE;

// Joystick Ports
const byte rollPort = A5;
const byte pitchPort = A6;
const byte yawPort = A3;
const byte throttlePort = A4;

// IR Sensors
const int LEFTSENSOR = 19;
const int MIDDLESENSOR = 20;
const int RIGHTSENSOR = 21;

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
 * Reading the value of the IR sensor 
 * @param SENSOR -- int represented by the constant
 * @return the value by the IR 
 */
 
int readIRSensor(int Sensor) {
  if (Sensor > 21 || Sensor < 19) {
    // wrong input port
    return 0;
  }
  int result = analogRead(Sensor);
  return result;
}

/**
 * Reading the value of the joystick of the given port.
 * 
 * @param port -- the port of the joystick
 * @return the manipulated value of the analog joystick input.
 */
int readJoystickValue(byte port) {
  int result = CoDrone.AnalogScaleChange(analogRead(port));
  switch (port) {
    case A5:
      result -= rollOffset;
      result /= -SCALE;
      break;
    case A6:
      result -= pitchOffset;
      result /= SCALE;
      break;
    case A3:
      result -= yawOffset;
      result /= -SCALE;
      break;
    case A4:
      result -= throttleOffset;
      result /= SCALE;
      break;
    default:
      break;
  }
  
  // Ignore if the joystick is at its default position.
  return (result > 2 || result < -2) ? result : 0;
}

void loop() {
  ROLL = readJoystickValue(rollPort);
  PITCH = readJoystickValue(pitchPort);
  YAW = readJoystickValue(yawPort);
  THROTTLE = readJoystickValue(throttlePort);

  int middleSensor = readIRSensor(MIDDLESENSOR);
  int rightSensor = readIRSensor(RIGHTSENSOR);

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
  if (middleSensor < 100) {
    CoDrone.emergencyStop();
  }

  // try to land when the right button is pressed and no others are
  if (rightSensor < 100) {
     CoDrone.land();
  }

  CoDrone.move(ROLL, PITCH, YAW, THROTTLE);
}
