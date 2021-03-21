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
		if (pData[0] == dType_Battery)
		{
			// battery request type
			CoDrone.getBatteryData(pData);
		}
		else if (pData[0] == dType_Pressure)
		{
			// get temperature and pressure data
			CoDrone.getPressureTempData(pData);
		}
		else if (pData[0] == dType_Range)
		{
			// get all the sensors data
			CoDrone.getSensorData(pData);
		}
		else if (pData[0] == dType_State)
		{
			// change all the drone state
			CoDrone.getState(pData);
		}
		else if (pData[0] == dType_ImuRawAndAngle)
		{
			CoDrone.getState(pData);
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

//-------------------------------------------------------------------------------------------------------//
//---------------------------------- ReceiveEventCheck --------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

void CoDroneClass::ReceiveEventCheck(byte _completeData[])
{
	if (receiveDtype == dType_LinkDiscoveredDevice) //Discovered Device
	{
		byte devIndex = _completeData[0];
		byte devName[20];

		for (int i = 1; i <= 6; i++)
		{
			if (devIndex == 0)
				devAddress0[i - 1] = _completeData[i];
			else if (devIndex == 1)
				devAddress1[i - 1] = _completeData[i];
			else if (devIndex == 2)
				devAddress2[i - 1] = _completeData[i];
			else if (devIndex == 3)
				devAddress3[i - 1] = _completeData[i];
			else if (devIndex == 4)
				devAddress4[i - 1] = _completeData[i];
		}

		devRSSI[devIndex] = _completeData[27];
		devFind[devIndex] = 1;
		devCount = devFind[0] + devFind[1] + devFind[2] + devFind[3] + devFind[4];
	}

	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_State)
	{

		droneState[0] = _completeData[0];
		droneState[1] = _completeData[1];
		droneState[2] = _completeData[2];
		droneState[3] = _completeData[3];
		droneState[4] = _completeData[4];
		droneState[5] = _completeData[5];
		droneState[6] = _completeData[6];

		receiveStateSuccess = 1;
	}
	//---------------------------------------------------------------------------------------------//

	else if (receiveDtype == dType_LinkRssi)
	{
		rssi = _completeData[0];
		rssi = rssi - 256;
	}

	else if (receiveDtype == dType_Ack)
	{
	}

	else if (receiveDtype == dType_IrMessage)
	{
		byte irMassageDirection = _completeData[0];

		unsigned long _irMessge[4];

		_irMessge[0] = _completeData[1];
		_irMessge[1] = _completeData[2];
		_irMessge[2] = _completeData[3];
		_irMessge[3] = _completeData[4];

		irMessageReceive = ((_irMessge[3] << 24) | (_irMessge[2] << 16) | (_irMessge[1] << 8) | (_irMessge[0] & 0xff));
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_Attitude)
	{
		attitudeRoll = ((_completeData[1] << 8) | (_completeData[0] & 0xff));
		attitudePitch = ((_completeData[3] << 8) | (_completeData[2] & 0xff));
		attitudeYaw = ((_completeData[5] << 8) | (_completeData[4] & 0xff));

		receiveAttitudeSuccess = 1;
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_GyroBias)
	{
		int GyroBias_Roll = ((_completeData[1] << 8) | (_completeData[0] & 0xff));
		int GyroBias_Pitch = ((_completeData[3] << 8) | (_completeData[2] & 0xff));
		int GyroBias_Yaw = ((_completeData[5] << 8) | (_completeData[4] & 0xff));

		receiveGyroSuccess = 1;
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_TrimAll)
	{
		TrimAll_Roll = ((_completeData[1] << 8) | (_completeData[0] & 0xff));
		TrimAll_Pitch = ((_completeData[3] << 8) | (_completeData[2] & 0xff));
		TrimAll_Yaw = ((_completeData[5] << 8) | (_completeData[4] & 0xff));
		TrimAll_Throttle = ((_completeData[7] << 8) | (_completeData[6] & 0xff));
		TrimAll_Wheel = ((_completeData[9] << 8) | (_completeData[8] & 0xff));
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_TrimFlight) //
	{
		TrimAll_Roll = ((_completeData[1] << 8) | (_completeData[0] & 0xff));
		TrimAll_Pitch = ((_completeData[3] << 8) | (_completeData[2] & 0xff));
		TrimAll_Yaw = ((_completeData[5] << 8) | (_completeData[4] & 0xff));
		TrimAll_Throttle = ((_completeData[7] << 8) | (_completeData[6] & 0xff));

		receiveTrimSuccess = 1;
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_TrimDrive)
	{
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_ImuRawAndAngle) // Done
	{
		ImuAccX = (_completeData[3] << 8) | (_completeData[2]);	 // x and y are switched
		ImuAccY = (_completeData[1] << 8) | (_completeData[0]);	 // x and y are switched
		ImuAccZ = -(_completeData[5] << 8) | (_completeData[4]); // y needs to be flipped to have gravity be negative

		ImuGyroRoll = (_completeData[7] << 8) | (_completeData[6]);
		;
		ImuGyroPitch = (_completeData[9] << 8) | (_completeData[8]);
		ImuGyroYaw = (_completeData[11] << 8) | (_completeData[10]);

		ImuAngleRoll = (_completeData[13] << 8) | (_completeData[12]);
		ImuAnglePitch = (_completeData[15] << 8) | (_completeData[14]);
		ImuAngleYaw = (_completeData[17] << 8) | (_completeData[16]);

		receiveAccelSuccess = 1;
	}

	//---------------------------------------------------------------------------------------------//

	else if (receiveDtype == dType_Pressure) // Done
	{
		long d1 = ((_completeData[3] << 32) | (_completeData[2] << 16) | (_completeData[1] << 8) | (_completeData[0] & 0xff));
		long d2 = ((_completeData[7] << 32) | (_completeData[6] << 16) | (_completeData[5] << 8) | (_completeData[4] & 0xff));
		temperature = ((_completeData[11] << 32) | (_completeData[10] << 16) | (_completeData[9] << 8) | (_completeData[8] & 0xff));
		pressure = ((_completeData[15] << 32) | (_completeData[14] << 16) | (_completeData[13] << 8) | (_completeData[12] & 0xff));

		receivePressureSuccess = 1;
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_ImageFlow) // Done
	{
		fVelocitySumX = ((_completeData[3] << 32) | (_completeData[2] << 16) | (_completeData[1] << 8) | (_completeData[0] & 0xff));
		fVelocitySumY = ((_completeData[7] << 32) | (_completeData[6] << 16) | (_completeData[5] << 8) | (_completeData[4] & 0xff));

		receiveOptSuccess = 1;
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_Button)
	{
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_Battery) // Done
	{
		int batteryV30 = ((_completeData[1] << 8) | (_completeData[0] & 0xff));
		int batteryV33 = ((_completeData[3] << 8) | (_completeData[2] & 0xff));
		int batteryGradient = ((_completeData[5] << 8) | (_completeData[4] & 0xff));
		int batteryyIntercept = ((_completeData[7] << 8) | (_completeData[6] & 0xff));
		int flagBatteryCalibration = _completeData[8];
		int batteryRaw = ((_completeData[12] << 32) | (_completeData[11] << 16) | (_completeData[10] << 8) | (_completeData[9] & 0xff));
		batteryPercent = _completeData[13];
		batteryVoltage = ((_completeData[15] << 8) | (_completeData[14] & 0xff));

		receiveBatterySuccess = 1;
	}

	else if (receiveDtype == dType_Range) // Done
	{
		sensorRange[0] = ((_completeData[1] << 8) | (_completeData[0] & 0xff));		//left
		sensorRange[1] = ((_completeData[3] << 8) | (_completeData[2] & 0xff));		//front
		sensorRange[2] = ((_completeData[5] << 8) | (_completeData[4] & 0xff));		//right
		sensorRange[3] = ((_completeData[7] << 8) | (_completeData[6] & 0xff));		//rear
		sensorRange[4] = ((_completeData[9] << 8) | (_completeData[8] & 0xff));		//top
		sensorRange[5] = ((_completeData[11] << 8) | (_completeData[10] & 0xff)); //bottom

		receiveRangeSuccess = 1;
	}
	else if (receiveDtype == dType_CountDrive)
	{
	}

	else if (receiveDtype == dType_CountFlight)
	{
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_Motor)
	{
	}
	//---------------------------------------------------------------------------------------------//
	else if (receiveDtype == dType_Temperature)
	{
	}

	//---------------------------------------------------------------------------------------------//

	else if (receiveDtype == dType_LinkState)
	{
		byte receiveLinkState = _completeData[0];
		//	receiveLikMode		=	_completeData[1];

		if (receiveLinkState == linkMode_None)
		{
		}
		else if (receiveLinkState == linkMode_Boot)
		{
		}
		else if (receiveLinkState == linkMode_Ready)
		{
		}
		else if (receiveLinkState == linkMode_Connecting)
		{
		}
		else if (receiveLinkState == linkMode_Connected)
		{
		}
		else if (receiveLinkState == linkMode_Disconnecting)
		{
		}
		else if (receiveLinkState == linkMode_ReadyToReset)
		{
		}
		linkState = receiveLinkState;
	}
	//---------------------------------------------------------------------------------------------//

	else if ((receiveDtype == dType_LinkEvent) || (receiveDtype == dType_LinkEventAddress))
	{
		byte receiveEventState = _completeData[0];

		//-----------------------------------------------------------------------------------------------//

		if (receiveEventState == linkEvent_ScanStop)
		{
			if ((discoverFlag == 1) || (discoverFlag == 2))
				discoverFlag = 3;
		}

		else if (receiveEventState == linkEvent_Connected)
		{
			if (connectFlag == 1)
			{
				connectFlag = 0;

				devAddressCheckEEPROMStandIn = 0x01;
				for (int i = 0; i <= 5; i++)
					devAddressConnectedEEPROMStandIn[i] = devAddressBuf[i];
			}
			pairing = true;
			delay(500);
		}

		else if (receiveEventState == linkEvent_ReadyToControl)
		{
			if (connectFlag == 1)
			{
				connectFlag = 0;

				devAddressCheckEEPROMStandIn = 0x01;
				for (int i = 0; i <= 5; i++)
					devAddressConnectedEEPROMStandIn[i] = devAddressBuf[i];
			}
			pairing = true;
			delay(500);
		}

		else if (receiveEventState == linkEvent_RspWriteSuccess)
		{
			if (sendCheckFlag == 1)
				sendCheckFlag = 2;
		}

		else if (receiveEventState == linkEvent_Write)
		{
			if (sendCheckFlag == 2)
				sendCheckFlag = 3;
		}

		else if (receiveEventState == linkEvent_SystemReset)
		{
		}

		else if (receiveEventState == linkEvent_Initialized)
		{
		}

		else if (receiveEventState == linkEvent_Scanning)
		{
			if (discoverFlag == 1)
				discoverFlag = 2;
		}

		//----------------------------------------------------------------------//

		else if (receiveEventState == linkEvent_None)
		{
		}

		else if (receiveEventState == linkEvent_FoundDroneService)
		{
		}

		else if (receiveEventState == linkEvent_Connecting)
		{
		}

		else if (receiveEventState == linkEvent_ConnectionFaild)
		{
		}

		else if (receiveEventState == linkEvent_ConnectionFaildNoDevices)
		{
		}

		else if (receiveEventState == linkEvent_ConnectionFaildNotReady)
		{
		}

		else if (receiveEventState == linkEvent_PairingStart)
		{
		}

		else if (receiveEventState == linkEvent_PairingSuccess)
		{
		}

		else if (receiveEventState == linkEvent_PairingFaild)
		{
		}

		else if (receiveEventState == linkEvent_BondingSuccess)
		{
		}

		else if (receiveEventState == linkEvent_LookupAttribute)
		{
		}

		else if (receiveEventState == linkEvent_RssiPollingStart)
		{
		}

		else if (receiveEventState == linkEvent_RssiPollingStop)
		{
		}

		else if (receiveEventState == linkEvent_DiscoverService)
		{
		}

		else if (receiveEventState == linkEvent_DiscoverCharacteristic)
		{
		}

		else if (receiveEventState == linkEvent_DiscoverCharacteristicDroneData)
		{
		}

		else if (receiveEventState == linkEvent_DiscoverCharacteristicDroneConfig)
		{
		}

		else if (receiveEventState == linkEvent_DiscoverCharacteristicUnknown)
		{
		}

		else if (receiveEventState == linkEvent_DiscoverCCCD)
		{
		}

		else if (receiveEventState == linkEvent_Disconnecting)
		{
		}

		else if (receiveEventState == linkEvent_Disconnected)
		{
		}

		else if (receiveEventState == linkEvent_GapLinkParamUpdate)
		{
		}

		else if (receiveEventState == linkEvent_RspReadError)
		{
		}

		else if (receiveEventState == linkEvent_RspReadSuccess)
		{
		}

		else if (receiveEventState == linkEvent_RspWriteError)
		{
		}

		else if (receiveEventState == linkEvent_SetNotify)
		{
		}
	}
}
//-------------------------------------------------------------------------------------------------------//0
