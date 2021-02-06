#ifndef Helper_H
#define Helper_H

int readYawValue(int yawOffset = 0);
int readRollValue(int rollOffset = 0);
int readThrottleValue(int throttleOffset = 0);
int readPitchValue(int pitchOffset = 0);
void recalibrate();

#endif Helper_H
