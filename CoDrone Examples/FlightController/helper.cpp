#include <CoDrone.h>
#include "Arduino.h"
#include "helper.h"
#include "setting.h"

/**
 * Read the yaw value from the joystick.
 * @return Manipulated yaw analog value from the joystick.
 */
int readYawValue() {
   int yawValue = CoDrone.AnalogScaleChange(analogRead(A3));
   // recalibrate the range 
   return -1 * ((yawValue - yawOffset) / SCALE);
}

/**
 * Read the roll value from the joystick.
 * @return Manipulated roll analog value from the joystick.
 */
int readRollValue() {
   int rollValue = CoDrone.AnalogScaleChange(analogRead(A4));
   // recalibrate the range 
   return -1 * ((rollValue - rollOffset) / SCALE);
}

/**
 * Read the throttle value from the joystick.
 * @return Manipulated throttle analog value from the joystick.
 */
int readThrottleValue() {
   int throttleValue = CoDrone.AnalogScaleChange(analogRead(A5));
   // recalibrate the range 
   return (throttleValue - throttleOffset) / SCALE;
}

/**
 * Read the pitch value from the joystick.
 * @return Manipulated pitch analog value from the joystick.
 */
int readPitchValue() {
   int pitchValue = CoDrone.AnalogScaleChange(analogRead(A6));
   // recalibrate the range 
   return (pitchValue - pitchOffset) / SCALE;
}

/**
 * Calibrate the joystick by setting the offsets.
 */
void recalibrate(){
  rollOffset = CoDrone.AnalogScaleChange(analogRead(rollPort));
  pitchOffset = CoDrone.AnalogScaleChange(analogRead(pitchPort));
  yawOffset = CoDrone.AnalogScaleChange(analogRead(yawPort));
  throttleOffset = CoDrone.AnalogScaleChange(analogRead(throttlePort));
}

/**
 * Getting the left IR sensor.
 */
int getLeftSensor(){
   return analogRead(19);
}

/**
 * Getting the middle IR sensor.
 */
int getMiddleSensor(){
   return analogRead(20);
}

/**
 * Getting the right IR sensor.
 */
int getRightSensor(){
   return analogRead(21);
}

/**
 * Displaying the offsets.
 */
void displaySetting(){
   Serial.print("yawOffset : "); 
   Serial.println(yawOffset);

   Serial.print("rollOffset : "); 
   Serial.println(rollOffset);
   
   Serial.print("throttleOffset : "); 
   Serial.println(throttleOffset);
   
   Serial.print("pitchOffset : "); 
   Serial.println(pitchOffset);
   
   Serial.print("RANGE MIN : "); 
   Serial.println(RANGEMIN);
   
   Serial.print("RANGE MAX : "); 
   Serial.println(RANGEMAX);
}

/**
 * Displaying the movements.
 */
void displayMovement(){
   Serial.print("Yaw: ");
   Serial.println(readYawValue());
   Serial.print("Roll: ");
   Serial.println(readRollValue());
   Serial.print("Throttle: ");
   Serial.println(readThrottleValue());
   Serial.print("Pitch : ");
   Serial.println(readPitchValue());   
}
