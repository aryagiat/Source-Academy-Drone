#include <CoDrone.h>
#pragma GCC diagnostic warning "-fpermissive"
#pragma GCC diagnostic ignored "-Wwrite-strings"

#define RXD2 16
#define TXD2 17

void setup() {
  CoDrone.begin(115200);
  CoDrone.pair(Nearest);
//  CoDrone.pair(new byte[6]{0x26, 0xC5, 0xD, 0xD7, 0x93, 0x18});
} // End of setup.


// This is the Arduino main loop function.
void loop() {  
//  Serial.print("test");
  delay(1000); // Delay a second between loops.
} // End of loop
