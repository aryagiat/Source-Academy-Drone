//#include <sinter.h>
//#include <sinter_config.h>

#include <CoDrone.h> // The codrone library that holds all the background files for this
#include "helper.h"

//sudo chmod a+rw /dev/ttyACM0

void setup() {  
  CoDrone.begin(115200);  
  CoDrone.pair(Nearest);
  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode
  //Serial.begin(9600);
  //recalibrate(); // adjust any offset
  //displaySetting();
}

void loop() {
  //grab buttons' data
  int MIDDLESENSOR = getMiddleSensor();
  int RIGHTSENSOR = getRightSensor();

  // Stop when the left butotn is pressed, but none of the others are
  if (MIDDLESENSOR < 100) {
    CoDrone.emergencyStop();
  }

  // try to land when the right button is pressed and no others are
  if (RIGHTSENSOR < 100) {
     CoDrone.land();
  }
//
//   Serial.print("Yaw: ");
//   Serial.println(readYawValue());
//   Serial.print("Roll: ");
//   Serial.println(readRollValue());
//   Serial.print("Throttle: ");
//   Serial.println(readThrottleValue());
//   Serial.print("Pitch : ");
//   Serial.println(readPitchValue()); 

   YAW = readYawValue();             // YAW (turn left or right), port A3
   THROTTLE  = readThrottleValue();  // THROTTLE (height), port A4
   ROLL = readRollValue();           // ROLL (tilt left or right), port A5
   PITCH = readPitchValue();         // PITCH (tilt front or back), port A6
   CoDrone.move(ROLL, PITCH, YAW, THROTTLE);
  
}
