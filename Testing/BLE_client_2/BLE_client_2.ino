#include "CoDrone.h"

void setup() {
    //Serial.begin(115200); //Start serial monitor 
    //Serial.println("ESP32 BLE Server program"); //Intro message 

    CoDrone.begin();
    CoDrone.takeoff();
    CoDrone.isFlying();
    CoDrone.land();
    
    
}

void loop() {
  
}
