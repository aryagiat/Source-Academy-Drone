#include <CoDrone.h> // The codrone library that holds all the background files for this
#include "helper.h"

void setup() {  
  CoDrone.begin(115200);  
  CoDrone.pair(Nearest);
  CoDrone.DroneModeChange(Flight);    // Changes the drone so that it is now in flight mode
  recalibrate(); // adjust any offset
}

void loop() {
  //grab buttons' data
  int LEFTSENSOR = getLeftSensor();
  int RIGHTSENSOR = getRightSensor();

  // Stop when the left butotn is pressed, but none of the others are
  if (LEFTSENSOR < 700) {
    CoDrone.emergencyStop();
  }

  // try to land when the right button is pressed and no others are
  if (RIGHTSENSOR < 700) {
     CoDrone.land();
  }
}
