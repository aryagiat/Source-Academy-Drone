#include <CoDrone.h>
#include "Arduino.h"
#include "helper.h"
#include "setting.h"

int readYawValue(int yawOffset) {
   int yawValue = -1 * CoDrone.AnalogScaleChange(analogRead(A3));
   // recalibrate the range 
   return yawValue / SCALE - yawOffset;
}

int readRollValue(int rollOffset) {
   int rollValue = -1 * CoDrone.AnalogScaleChange(analogRead(A4));
   // recalibrate the range 
   return rollValue / SCALE - rollOffset;
}

int readThrottleValue(int throttleOffset) {
   int throttleValue = CoDrone.AnalogScaleChange(analogRead(A5));
   // recalibrate the range 
   return throttleValue / SCALE - throttleOffset;
}

int readPitchValue(int pitchOffset) {
   int pitchValue = CoDrone.AnalogScaleChange(analogRead(A6));
   // recalibrate the range 
   return pitchValue / SCALE - pitchOffset;
}

void recalibrate(){
	yawOffset = readYawValue(0);
	rollOffset = readRollValue(0);
	throttleOffset = readThrottleValue(0);
	pitchOffset = readPitchValue(0);
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
