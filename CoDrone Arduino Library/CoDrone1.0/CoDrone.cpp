/*
	CoDrone.cpp	-	CoDrone	library
	Copyright	(C)	2014 RoboLink.	All	rights reserved.
	LastUpdate : 2018-05-31
*/

#include "CoDrone.h"
#include "Arduino.h"

CoDroneClass CoDrone;

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------- Begin ----------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

#include "BLEDevice.h"
//#include "BLEScan.h"
// The remote service we wish to connect to.

/**
 * Callback function to manage all the data notified by the CoDrone
 */
static void notifyCallback(
		BLERemoteCharacteristic *pBLERemoteCharacteristic,
		uint8_t *pData,
		size_t length,
		bool isNotify)
{
	/**
	* debugging line to see the call data being sent
	*/
	// Serial.print("Notify callback for characteristic ");
	// Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
	// Serial.print(" of data length ");
	// Serial.println(length);
	if (isNotify)
	{
		// for (int i = 0; i < length; i++)
		// {
		// 	Serial.print(pData[i]);
		// 	Serial.print(" ");
		// }
		// pData[0] is the data type
		switch (pData[0])
		{
		case dType_Battery:
			CoDrone.getBatteryData(pData);
			break;
		case dType_Pressure:
			CoDrone.getPressureTempData(pData);
			break;
		case dType_Range:
			CoDrone.getSensorData(pData);
			break;
		case dType_State:
			CoDrone.getState(pData);
			break;
		case dType_ImuRawAndAngle:
			CoDrone.getImuRawAndAngle(pData);
			break;
		case dType_TrimFlight:
			CoDrone.getTrimData(pData);
			break;
		case dType_Attitude:
			CoDrone.getAttitudeData(pData);
			break;
		case dType_ImageFlow:
			CoDrone.getOptFlowData(pData);
			break;
		case dType_TrimAll: // wont be called from receivedgetData
			CoDrone.getTrimDataAll(pData);
			break;
		case dType_GyroBias: //wont be called from receivedgetData
			CoDrone.getGyroBias(pData);
			break;
		case dType_IrMessage: //wont be called from receivedgetData
			CoDrone.getIrMessage(pData);
			break;
		case dType_LinkRssi: // wont be called, this is used for traditional pairing
			CoDrone.getRssiData(pData);
			break;
		}
	}
}

class MyClientCallback : public BLEClientCallbacks
{
	void onConnect(BLEClient *pclient)
	{
		CoDrone.connected = true;
		Serial.println("Connected");
	}

	void onDisconnect(BLEClient *pclient)
	{
		CoDrone.connected = false;
		Serial.println("onDisconnect");
	}
};

bool connectToServer()
{
	//Serial.print("Forming a connection to ");
	//Serial.println(myDevice->getAddress().toString().c_str());

	BLEClient *pClient = BLEDevice::createClient();
	//Serial.println(" - Created client");

	pClient->setClientCallbacks(new MyClientCallback());

	// Connect to the remove BLE Server.
	pClient->connect(CoDrone.myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
	//Serial.println(" - Connected to server");

	// Obtain a reference to the service we are after in the remote BLE server.
	BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
	if (pRemoteService == nullptr)
	{
		Serial.print("Failed to find our service UUID: ");
		Serial.println(serviceUUID.toString().c_str());
		pClient->disconnect();
		return false;
	}
	//Serial.println(" - Found our service");

	// Obtain a reference to the characteristic in the service of the remote BLE server.
	CoDrone.pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);	 // read from device
	CoDrone.pRemoteCharacteristic2 = pRemoteService->getCharacteristic(charUUID2); // write to device

	if (CoDrone.pRemoteCharacteristic->canNotify())
	{
		CoDrone.pRemoteCharacteristic->registerForNotify(notifyCallback);
		Serial.println("can notify");
	}
	CoDrone.Send_Command(cType_Request, Req_Battery);

	return true;
}

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
	/**
   * Called for each advertising BLE server.
   */
	void onResult(BLEAdvertisedDevice advertisedDevice)
	{
		// Serial.print("BLE Advertised Device found: ");
		// Serial.println(advertisedDevice.toString().c_str());

		// We have found a device, let us now see if it contains the service we are looking for.
		if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
		{

			BLEDevice::getScan()->stop();
			CoDrone.myDevice = new BLEAdvertisedDevice(advertisedDevice);
			CoDrone.doConnect = true;
			CoDrone.doScan = true;

		} // Found our server
	}		// onResult
};		// MyAdvertisedDeviceCallbacks

void CoDroneClass::begin()
{
	Serial.begin(115200);
	BLEDevice::init("");

	// Retrieve a Scanner and set the callback we want to use to be informed when we
	// have detected a new device.  Specify that we want active scanning and start the
	// scan to run for 5 seconds.
	BLEScan *pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
	pBLEScan->setInterval(1349);
	pBLEScan->setWindow(449);
	pBLEScan->setActiveScan(true);
	pBLEScan->start(5, false);

	while (!connected)
	{
		connectToServer();
	}

	Serial.println("Connected");
}
