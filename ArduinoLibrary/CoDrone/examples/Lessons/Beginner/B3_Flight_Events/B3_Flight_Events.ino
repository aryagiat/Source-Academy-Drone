#include <CoDrone.h>

void setup() {    
    CoDrone.begin(115200);    
    CoDrone.AutoConnect(NearbyDrone);    

    CoDrone.FlightEvent(TakeOff);    
    delay(2000);    
    CoDrone.FlightEvent(Landing);
}

void loop(){    
    byte bt8 = digitalRead(18);    
    byte bt4 = digitalRead(14);    
    byte bt1 = digitalRead(11);

    if(bt1 && bt4 && !bt8){    
    CoDrone.FlightEvent(Stop);
    }
}







