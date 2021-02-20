#include <sinter_config.h>
#include <sinter.h>
#include <CoDrone.h> // The codrone library that holds all the background files for this
#include "internal_functions.h"
#pragma GCC diagnostic warning "-fpermissive"
#pragma GCC diagnostic ignored "-Wwrite-strings"

const unsigned char hello_world_svm[] = {
  0xad, 0xac, 0x05, 0x50, 0x00, 0x00, 0x00 ,0x00,
  0x24, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x48, 0x65,
  0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c,
  0x64, 0x21, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x0d, 0x10, 0x00, 0x00, 0x00, 0x42, 0x05, 0x01,
  0x46 
};

const unsigned char drone_fly_svm[] = {
  0xad, 0xac, 0x05, 0x50, 0x00, 0x00, 0x00 ,0x00,
  0x10, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x44, 0x16, 0x00, 0x46
};



const unsigned int hello_world_svm_len = 49;
const unsigned int drone_fly_svm_len = 24;
char heap[0x4000];

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
  // setting up serial 
  Serial.begin(115200);
  while(!Serial); 

  Serial.println("Begin");
 // CoDrone.begin(115200);

  // or can hardcode the address new byte[6]{0x26, 0xC5, 0xD, 0xD7, 0x93, 0x18});
//  CoDrone.pair(Nearest);/
//  CoDrone.DroneModeChange(Flight);
  delay(1000);
  Serial.println("Paired");
  
  /**
   * Recalibrate controller.
   * 
   * roll control -- port A5
   * pitch control -- port A6
   * yaw control -- port A3
   * throttle control -- port A4
   */
//  rollOffset = CoDrone.AnalogScaleChange(analogRead(rollPort));
//  pitchOffset = CoDrone.AnalogScaleChange(analogRead(pitchPort));
//  yawOffset = CoDrone.AnalogScaleChange(analogRead(yawPort));
//  throttleOffset = CoDrone.AnalogScaleChange(analogRead(throttlePort));
//  setupInternals();
}

/**
 * Reading the value of the IR sensor 
 * @param SENSOR -- int represented by the constant
 * @return the value by the IR 
 */
 
//int readIRSensor(int Sensor) {
//  if (Sensor > 21 || Sensor < 19) {
//    // wrong input port
//    return 0;
//  }
//  int result = analogRead(Sensor);
//  return result;
//}

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
  sinter_setup_heap(heap, 0x4000);

  sinter_value_t result;
  sinter_fault_t fault = sinter_run(drone_fly_svm, drone_fly_svm_len, &result);

  Serial.print("Program exited with fault ");
  Serial.print(fault);
  Serial.print(" and result type ");
  Serial.print(result.type);
  Serial.print(" (");
  Serial.print(result.integer_value);
  Serial.print(", ");
  Serial.print(result.float_value);
  Serial.print(")\n");
  Serial.print("Result : ");
  Serial.println(result.string_value);
  
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
