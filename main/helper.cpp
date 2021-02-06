#include <CoDrone.h>
#include "Arduino.h"
#include "helper.h"
#include "setting.h"

int readYawValue() {
   int yawValue = -1 * CoDrone.AnalogScaleChange(analogRead(A3));
   // recalibrate the range 
   return yawValue / SCALE - yawOffset;
}

int readRollValue() {
   int rollValue = -1 * CoDrone.AnalogScaleChange(analogRead(A4));
   // recalibrate the range 
   return rollValue / SCALE - rollOffset;
}

int readThrottleValue() {
   int throttleValue = CoDrone.AnalogScaleChange(analogRead(A5));
   // recalibrate the range 
   return throttleValue / SCALE - throttleOffset;
}

int readPitchValue() {
   int pitchValue = CoDrone.AnalogScaleChange(analogRead(A6));
   // recalibrate the range 
   return pitchValue / SCALE - pitchOffset;
}

void recalibrate(){
	yawOffset = readYawValue();
	rollOffset = readRollValue();
	throttleOffset = readThrottleValue();
	pitchOffset = readPitchValue();
}

int getLeftSensor(){
   return analogRead(19);
}

int getMiddleSensor(){
   return analogRead(20);
}

int getRightSensor(){
   return analogRead(21);
}

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