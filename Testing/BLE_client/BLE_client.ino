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
  
  byte _cType = 0x11; // dType_Command
  
  //header
  _packet[0] = _cType;
  
  //data
  _packet[1] = sendCommand;
  _packet[2] = sendOption;
  
  // sending the data
  pRemoteCharacteristic2->writeValue(_packet, 3);
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
    
    if (pRemoteCharacteristic2 == nullptr) {
      Serial.print("Failed to find our characteristic2 UUID: ");
      Serial.println(charUUID2.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic2");

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

    // Changing the mode of the drone to flight mode
    Serial.println("Changin mode");
    byte _DroneModeChange[8] = {0x11, 0x10, 0x10};
    pRemoteCharacteristic2->writeValue(_DroneModeChange, 3);
    Serial.println("change mode done");
    
    delay(2000);

    // Doing CoDrone.takeoff()
    Serial.println("flying");
    byte _takeoff[8] = {0x11, 0x22, 0x01};
    pRemoteCharacteristic2->writeValue(_takeoff, 3);
    Serial.println("flight done");

    delay(3000);

    // Doing CoDrone.land()
    Serial.println("landing");
    byte _land[8] = {0x11, 0x22, 0x07};
    pRemoteCharacteristic2->writeValue(_land, 3);
    Serial.println("touch down");
    
    delay(5000);
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(5000); // Delay a second between loops.
} // End of loop
