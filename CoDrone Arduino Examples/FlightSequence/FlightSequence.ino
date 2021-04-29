/*------------------------------------------------------------------
Sequence
Objective   : how to use flySequence()
syntax      : flySequence("action")
			  *action can be : ZIGZAG, TRIANGLE, SQUARE, SWAY, SPIRAL, CIRCLE, HOP
description : flySequce function is preset fuction for drone movement.
			  It's not really 
-------------------------------------------------------------------*/
#include <CoDrone.h> // The codrone library that holds all the background files for this

/*-----------------------
 * For Linux User, run this command 
 * sudo chmod 666 /dev/ttyACM0/
 * newgrp dialout 
 * sudo gpasswd --add ${USER} dialout
 * check whether dialout is in your USER group using this command : groups ${USER}
 */

int incomingByte = 0;

void setup() {  
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("Begin");
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  Serial.println("Pair Device");
  CoDrone.pair(Nearest);//new byte[6]{0x26, 0xC5, 0xD, 0xD7, 0x93, 0x18});
  Serial.println("Set Mode Flight");
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode
  delay(1000); 
  Serial.println("Ready!");
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (incomingByte == 49) {
      incomingByte = 0;
      Serial.println("Take off");
      CoDrone.takeoff();           
      Serial.println("Zig Zag");
      CoDrone.flySequence(ZIGZAG);
      Serial.println("Land");
      CoDrone.land(); 
      delay(3000); 
      Serial.println("Ready!");
    }
  }
  delay(1000); 
  Serial.println("Idle!");

}
