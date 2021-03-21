#include "CoDrone.h"

void setup() {
  // put your setup code here, to run once:
  CoDrone.begin(115200);
  Serial.begin(9600);
  Serial.println("hello");
  CoDrone.pair(Nearest);
  CoDrone.getBatteryPercentage();
}

void loop() {
  // put your main code here, to run repeatedly:

}
