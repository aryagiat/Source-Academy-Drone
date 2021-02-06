#ifndef Helper_H
#define Helper_H

//for input value
int readYawValue(int yawOffset = 0);
int readRollValue(int rollOffset = 0);
int readThrottleValue(int throttleOffset = 0);
int readPitchValue(int pitchOffset = 0);

int getLeftSensor();
int getRightSensor();
int getMiddleSensor();

//Misc functions 
void recalibrate();


#endif Helper_H
