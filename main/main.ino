#include <CoDrone.h> // The codrone library that holds all the background files for this
#include "helper.h"

//sudo chmod a+rw /dev/ttyACM0

void setup() {  
//  CoDrone.begin(115200);  
//  CoDrone.pair(Nearest);
//  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode
  Serial.begin(9600);
  recalibrate(); // adjust any offset
}

void loop() {
  //grab buttons' data
  int LEFTSENSOR = getLeftSensor();
  int RIGHTSENSOR = getRightSensor();

  // Stop when the left butotn is pressed, but none of the others are
//  if (LEFTSENSOR < 700) {
//    CoDrone.emergencyStop();
//  }
//
//  // try to land when the right button is pressed and no others are
//  if (RIGHTSENSOR < 700) {
//     CoDrone.land();
//  }

  Serial.print("Yaw: ");
  Serial.println(readYawValue());
  Serial.print("Roll: ");
  Serial.println(readRollValue());
  Serial.print("Throttle: ");
  Serial.println(readThrottleValue());
  Serial.print("Pitch : ");
  Serial.println(readPitchValue());
  delay(2000);
}
