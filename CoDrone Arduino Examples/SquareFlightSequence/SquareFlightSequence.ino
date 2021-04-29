/**
 * Square Flight Sequence
 * 
 * description: using flight commands, control the drone to lift off, hover, create a square, and land.
 */
#include <CoDrone.h> // The codrone library that holds all the background files for this

void setup() {  
  CoDrone.begin(115200);            // sets up the connection to the drone using the bluetooth module at 115200bps (bits per second)
  CoDrone.pair(Nearest);            // Pairs with the nearest CoDrone.
  CoDrone.DroneModeChange(Flight);  // Changes the drone so that it is now in flight mode

  delay(1000);

  CoDrone.takeoff();                // Take off and hover for 3 seconds for little auto calibrate

  // Move forward for 1 seconds.
  CoDrone.setPitch(70);
  CoDrone.move(1);
  CoDrone.hover(1);

  // Move to the left for 1 seconds.
  CoDrone.setRoll(-70);
  CoDrone.move(1);
  CoDrone.hover(1);

  // Move backwards for 1 seconds.
  CoDrone.setPitch(-70);
  CoDrone.move(1);
  CoDrone.hover(1);

  // Move to the right for 1 seconds.
  CoDrone.setRoll(70);
  CoDrone.move(1);
  CoDrone.hover(1);

  
  CoDrone.land();                   // Landing with sound.
}

void loop() {
  //grab analog IR's data
  int LEFTSENSOR = analogRead(19);    // Left IR sensor (Port 19)
  int MIDDLESENSOR = analogRead(20);  // Middle IR sensor (Port 20)
  int RIGHTSENSOR = analogRead(21);   // Right IR sensor (Port 21)

  // Stop when the left butotn is pressed, but none of the others are
  if (LEFTSENSOR < 700)
  {
    CoDrone.emergencyStop();
  }

  // try to land when the right button is pressed and no others are
  if (RIGHTSENSOR < 700)
  {
     CoDrone.land();
  }
}
