#ifndef Helper_H
#define Helper_H

//for input value
int readYawValue();
int readRollValue();
int readThrottleValue();
int readPitchValue();

int getLeftSensor();
int getRightSensor();
int getMiddleSensor();

//Misc functions 
void recalibrate();
void displaySetting();
void displayMovement();

#endif Helper_H
