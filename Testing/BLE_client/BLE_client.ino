/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
#include "CoDrone.h"
//#include "BLEScan.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("c320df00-7891-11e5-8bcf-feff819cdc9f");     // Service UUID
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("c320df01-7891-11e5-8bcf-feff819cdc9f");     // DRONE_DATA (Drone -> Device)
static BLEUUID    charUUID2("c320df02-7891-11e5-8bcf-feff819cdc9f");    // DRONE_CONF (Device -> Drone)

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLERemoteCharacteristic* pRemoteCharacteristic2;
static BLEAdvertisedDevice* myDevice;

static void Send_Command(int sendCommand, int sendOption)
{  
  byte _packet[9];
  byte _crc[2];
  
  byte _cType = 0x11; // dType_Command
  byte _len   = 0x02;
  
  //header
  _packet[0] = _cType;
  _packet[1] = _len;
  
  //data
  _packet[2] = sendCommand;
  _packet[3] = sendOption;
  
  unsigned short crcCal = CoDrone.CRC16_Make(_packet, _len+2);
  _crc[0] = (crcCal >> 8) & 0xff;
  _crc[1] = crcCal & 0xff;

  int _length = (int) sizeof(_packet)/sizeof(_packet[0]);
  
  Send_Processing(_packet, _len, _crc);
}

static void Send_Processing(byte _data[], byte _length, byte _crc[]) {
  byte _packet[30];
  // START CODE
  _packet[0] = 0x0A;    // START1
  _packet[1] = 0x55;    // START2

  // HEADER & DATA
  for(int i = 0; i < _length + 3 ; i++)   _packet[i+2] = _data[i];

  //CRC
  _packet[_length + 4] =_crc[1];
  _packet[_length + 5] =_crc[0];

  // data send control
  Serial.println("_packet: ");
  for (int i = 0; i < _length + 6; ++i) {
    Serial.print(_packet[i]);
    Serial.print(" ");
  }
  Serial.println("");
  pRemoteCharacteristic2->writeValue(_packet, _length + 6);
}

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,uint8_t* pData,size_t length,bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    //Serial.println(pRemoteCharacteristic->readValue().c_str());
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    pRemoteCharacteristic2 = pRemoteService->getCharacteristic(charUUID2);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify()) {
      //pRemoteCharacteristic->registerForNotify(notifyCallback);
      Serial.println("can notify");
      notifyCallback(pRemoteCharacteristic,0,69,true);
    }

    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(9600);
  //CoDrone.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    //pRemoteCharacteristic2->writeValue(newValue.c_str(), newValue.length());
    //Send_Command(0x10, 0x10); // Change mode
    //Send_Command(0x11, 0x02); // change mode from the HEX Test (commented)
    //Send_Command(0x22, 0x02); // takeoff

    Serial.println("control");
    byte _control[10] = {0x0A, 0x55, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x82, 0x93};
    pRemoteCharacteristic2->writeValue(_control, 10);
    Serial.println("controlled");

    Serial.println("Changin mode");
    byte _DroneModeChange[8] = {0x0A, 0x55, 0x11, 0x02, 0x10, 0x10, 0x31, 0x12};
    pRemoteCharacteristic2->writeValue(_DroneModeChange, 8);
    Serial.println("change mode done");
    
    delay(2000);

    Serial.println("flying");
    byte _takeoff[8] = {0x0A, 0x55, 0x11, 0x02, 0x22, 0x01, 0xD6, 0x73};
    pRemoteCharacteristic2->writeValue(_takeoff, 8);
    Serial.println("flight done");

    Serial.println("landing");
    byte _land[8] = {0x0A, 0x55, 0x11, 0x02, 0x22, 0x07, 0x10, 0x13};
    pRemoteCharacteristic2->writeValue(_land, 8);
    Serial.println("touch down");
    
    delay(5000);
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(5000); // Delay a second between loops.
} // End of loop
