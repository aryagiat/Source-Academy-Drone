#include "CoDrone.h"

void setup() {
  // put your setup code here, to run once:
  CoDrone.begin(115200);
  CoDrone.pair(Nearest);
  CoDrone.DroneModeChange(Flight);
  CoDrone.takeoff();
  CoDrone.land();

}

void loop() {
  // put your main code here, to run repeatedly:

}
