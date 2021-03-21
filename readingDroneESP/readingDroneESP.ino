#include "CoDrone.h"

void setup() {
  // put your setup code here, to run once:
  CoDrone.begin();
  CoDrone.getBatteryPercentage();
  CoDrone.getDroneTemp();
  CoDrone.getHeight();
  CoDrone.isFlying();
}

void loop() {
  // put your main code here, to run repeatedly:

}
