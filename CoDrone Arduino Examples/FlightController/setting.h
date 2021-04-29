#ifndef setting_H
#define setting_H

extern int yawOffset = 0;
extern int rollOffset = 0;
extern int throttleOffset = 0;
extern int pitchOffset = 0;
extern int RANGEMIN = -50;
extern int RANGEMAX = 50;
extern int ORISCALE = 200; // from -100 to 100
extern int RANGE = RANGEMAX - RANGEMIN; // from -10 to 10
extern int SCALE = ORISCALE / RANGE;

extern const byte rollPort = A5;
extern const byte pitchPort = A6;
extern const byte yawPort = A3;
extern const byte throttlePort = A4;

#endif setting_H
