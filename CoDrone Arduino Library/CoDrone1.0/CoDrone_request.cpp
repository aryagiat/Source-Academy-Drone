#include "CoDrone.h"
#include "Arduino.h"

//-------------------------------------------------------------------------------------------------------//
//------------------------------------------- GetData ---------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
void CoDroneClass::ReceiveGetData(byte _reqType)
{
	byte *_reqCheckType;

	sendCheckFlag = 1;
	Send_Command(cType_Request, _reqType);

	//---------------------------------------------------------------------------------//

	if (_reqType == Req_Attitude)
		_reqCheckType = &receiveAttitudeSuccess;
	else if (_reqType == Req_Battery)
		_reqCheckType = &receiveBatterySuccess;
	else if (_reqType == Req_Range)
		_reqCheckType = &receiveRangeSuccess;
	else if (_reqType == Req_State)
		_reqCheckType = &receiveStateSuccess;
	else if (_reqType == Req_ImageFlow)
		_reqCheckType = &receiveOptSuccess;
	else if (_reqType == Req_Pressure)
		_reqCheckType = &receivePressureSuccess;
	else if (_reqType == Req_TrimFlight)
		_reqCheckType = &receiveTrimSuccess;
	else if (_reqType == Req_ImuRawAndAngle)
		_reqCheckType = &receiveAccelSuccess;

	//--------------------------------------------------------------------------------//
	*_reqCheckType = 0;

	long oldTime = millis();
	while (!*_reqCheckType)
	{
		// should call the receive upon callback of send command
		if (oldTime + 5000 < millis())
		{
			// 5 seconds is the maxium time allowed
			Serial.println("time out, data is not fetched");
			break; //time out check
		}
	}
}
//--------------------------------------------------------------------------------//
/*
 *	function	:	getPressure()
 *	description : 	getPressure() is function for get pressure from barometer sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global bariable in header file also
 *	param 		:	none
 *	return 	 	:	
 *					- pressure 	: int
 */
int CoDroneClass::getPressure()
{
	ReceiveGetData(Req_Pressure);
	return pressure;
}

/*
 *	function	:	getDroneTemp()
 *	description : 	getDroneTemp() is function for get temperature from barometer sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global bariable in header file also
 *	param 		:	none
 *	return 	 	:	
 *					- temperature 	: int
 */
int CoDroneClass::getDroneTemp()
{
	ReceiveGetData(Req_Pressure);
	return temperature;
}

void CoDroneClass::getPressureTempData(uint8_t *pData)
{
	long d1 = ((pData[3 + 1] << 32) | (pData[2 + 1] << 16) | (pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));
	long d2 = ((pData[7 + 1] << 32) | (pData[6 + 1] << 16) | (pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));
	temperature = ((pData[11 + 1] << 32) | (pData[10 + 1] << 16) | (pData[9 + 1] << 8) | (pData[8 + 1] & 0xff));
	pressure = ((pData[15 + 1] << 32) | (pData[14 + 1] << 16) | (pData[13 + 1] << 8) | (pData[12 + 1] & 0xff));

	receivePressureSuccess = 1;
}

/*
 *	function	:	getState()
 *	description : 	getState() is function for get drone state.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Have to use with enum modeFlight
					- fMode_None = 0,
					- fMode_Ready,
					- fMode_TakeOff,
					- fMode_Flight, 
					- fMode_Flip, 	
					- fMode_Stop, 	
					- fMode_Landing,
					- fMode_Reverse,
					- fMode_Accident,
					- fMode_Error, 
					- fMode_EndOfType
 *	param 		:	none
 *	return 	 	:	
 *					- droneState[3] 	: int
 */

int CoDroneClass::getState()
{
	ReceiveGetData(Req_State);
	return droneState[2];
}

void CoDroneClass::getState(uint8_t *pData)
{
	droneState[0] = pData[0 + 1];
	droneState[1] = pData[1 + 1];
	droneState[2] = pData[2 + 1];
	droneState[3] = pData[3 + 1];
	droneState[4] = pData[4 + 1];
	droneState[5] = pData[5 + 1];
	droneState[6] = pData[6 + 1];

	receiveStateSuccess = 1;
}
/*
 *	function	:	getHeight()
 *	description : 	getHeight() is function for get height from IR distance sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global bariable in header file also
 *	param 		:	none
 *	return 	 	:	
 *					- height 	: int
 */
int CoDroneClass::getHeight()
{
	ReceiveGetData(Req_Range);
	Serial.print("Height : ");
	Serial.println(sensorRange[5]);
	return sensorRange[5];
}

void CoDroneClass::getSensorData(uint8_t *pData)
{
	sensorRange[0] = ((pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));		//left
	sensorRange[1] = ((pData[3 + 1] << 8) | (pData[2 + 1] & 0xff));		//front
	sensorRange[2] = ((pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));		//right
	sensorRange[3] = ((pData[7 + 1] << 8) | (pData[6 + 1] & 0xff));		//rear
	sensorRange[4] = ((pData[9 + 1] << 8) | (pData[8 + 1] & 0xff));		//top
	sensorRange[5] = ((pData[11 + 1] << 8) | (pData[10 + 1] & 0xff)); //bottom

	receiveRangeSuccess = 1;
}

/*
 *	function	:	getBatteryPercentage()
 *	description : 	getBatteryPercentage() is function for get battery percentage.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					If you request in a 5 seconds, it's return data that save in a header file.
 *	param 		:	none
 *	return 	 	:	
 *					- Battery_Percent 	: int
 */

int CoDroneClass::getBatteryPercentage()
{
	ReceiveGetData(Req_Battery);
	Serial.print("Batter percentage : ");
	Serial.print(batteryPercent);
	return batteryPercent;
}

/*
 *	function	:	getBatteryVoltage()
 *	description : 	getBatteryVoltage() is function for get battery voltage.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					If you request in a 5 seconds, it's return data that save in a header file.
 *	param 		:	none
 *	return 	 	:	
 *					- Battery_voltage 	: int
 */
int CoDroneClass::getBatteryVoltage()
{
	ReceiveGetData(Req_Battery);
	Serial.print("Batter Voltage : ");
	Serial.print(batteryVoltage);
	return batteryVoltage;
}

void CoDroneClass::getBatteryData(uint8_t *pData)
{
	// already plus one for everything
	int batteryV30 = ((pData[2] << 8) | (pData[1] & 0xff));
	int batteryV33 = ((pData[4] << 8) | (pData[3] & 0xff));
	int batteryGradient = ((pData[6] << 8) | (pData[5] & 0xff));
	int batteryyIntercept = ((pData[8] << 8) | (pData[7] & 0xff));
	int flagBatteryCalibration = pData[8];
	int batteryRaw = ((pData[13] << 32) | (pData[12] << 16) | (pData[11] << 8) | (pData[10] & 0xff));
	batteryPercent = pData[14];
	batteryVoltage = ((pData[16] << 8) | (pData[15] & 0xff));

	Serial.print("Battery ");
	Serial.println(batteryPercent);
	Serial.print("Voltage ");
	Serial.println(batteryVoltage);

	receiveBatterySuccess = 1;
}

/*
 *	function	:	getAceelerometer()
 *	description : 	getAceelerometer() is function for get Aceelerometer value from gyro sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure acceldata  is made of x, y, and z
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: acceldata
 */

acceldata CoDroneClass::getAccelerometer()
{
	ReceiveGetData(Req_ImuRawAndAngle);

	acceldata result;
	result.x = ImuAccX;
	result.y = ImuAccY;
	result.z = ImuAccZ;
	return result;
}

void CoDroneClass::getImuRawAndAngle(uint8_t *pData)
{
	ImuAccX = (pData[3 + 1] << 8) | (pData[2 + 1]);	 // x and y are switched
	ImuAccY = (pData[1 + 1] << 8) | (pData[0 + 1]);	 // x and y are switched
	ImuAccZ = -(pData[5 + 1] << 8) | (pData[4 + 1]); // y needs to be flipped to have gravity be negative

	ImuGyroRoll = (pData[7 + 1] << 8) | (pData[6 + 1]);
	ImuGyroPitch = (pData[9 + 1] << 8) | (pData[8 + 1]);
	ImuGyroYaw = (pData[11 + 1] << 8) | (pData[10 + 1]);

	ImuAngleRoll = (pData[13 + 1] << 8) | (pData[12 + 1]);
	ImuAnglePitch = (pData[15 + 1] << 8) | (pData[14 + 1]);
	ImuAngleYaw = (pData[17 + 1] << 8) | (pData[16 + 1]);

	receiveAccelSuccess = 1;
}

/*
 *	function	:	getTrim()
 *	description : 	getTrim() is function for get current trim value for flight.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also.
 *					Structure trim data has roll, pitch, yaw, and throttle.
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: int
 */

trimdata CoDroneClass::getTrim()
{
	ReceiveGetData(Req_TrimFlight);

	trimdata result;
	result.roll = TrimAll_Roll;
	result.pitch = TrimAll_Pitch;
	result.yaw = TrimAll_Yaw;
	result.throttle = TrimAll_Throttle;

	return result;
}

void CoDroneClass::getTrimData(uint8_t *pData)
{
	TrimAll_Roll = ((pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));
	TrimAll_Pitch = ((pData[3 + 1] << 8) | (pData[2 + 1] & 0xff));
	TrimAll_Yaw = ((pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));
	TrimAll_Throttle = ((pData[7 + 1] << 8) | (pData[6 + 1] & 0xff));

	receiveTrimSuccess = 1;
}

void CoDroneClass::getTrimDataAll(uint8_t *pData)
{
	TrimAll_Roll = ((pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));
	TrimAll_Pitch = ((pData[3 + 1] << 8) | (pData[2 + 1] & 0xff));
	TrimAll_Yaw = ((pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));
	TrimAll_Throttle = ((pData[7 + 1] << 8) | (pData[6 + 1] & 0xff));
	TrimAll_Wheel = ((pData[9 + 1] << 8) | (pData[8 + 1] & 0xff));

	receiveTrimSuccess = 1;
}

/*
 *	function	:	getOptFlowPosition()
 *	description : 	getOptFlowPosition() is function for get x,y position value from optical flow sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure optdata  is made of x, and y
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: optdata
 */

optdata CoDroneClass::getOptFlowPosition()
{
	ReceiveGetData(Req_ImageFlow);

	optdata result;
	result.x = fVelocitySumX;
	result.y = fVelocitySumY;
	return result;
}

void CoDroneClass::getOptFlowData(uint8_t *pData)
{
	fVelocitySumX = ((pData[3 + 1] << 32) | (pData[2 + 1] << 16) | (pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));
	fVelocitySumY = ((pData[7 + 1] << 32) | (pData[6 + 1] << 16) | (pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));

	receiveOptSuccess = 1;
}

/*
 *	function	:	getAngulerSpeed()
 *	description : 	getAngulerSpeed() is function for get gyro value from gyro sensor.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure gyrodata  is made of roll, pitch, and yaw
 *	param 		:	none
 *	return 	 	:	
 *					- result 	: gyrodata
 */

gyrodata CoDroneClass::getAngularSpeed()
{
	ReceiveGetData(Req_ImuRawAndAngle);

	gyrodata result;
	result.roll = ImuGyroRoll;
	result.pitch = ImuGyroPitch;
	result.yaw = ImuGyroYaw;

	return result;
}

void CoDroneClass::getGyroBias(uint8_t *pData)
{
	int GyroBias_Roll = ((pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));
	int GyroBias_Pitch = ((pData[3 + 1] << 8) | (pData[2 + 1] & 0xff));
	int GyroBias_Yaw = ((pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));

	receiveGyroSuccess = 1;
}

/*
 *	function	:	getGyroAngles()
 *	description : 	getGyroAngles() is function for get angle value.
 *					this function is sending message to drone and wait for 0 ~ 1 second to get data
 *					this value is save at global variable in header file also
 *					Structure angledata  is made of roll, pitch, and yaw
 *	param 		:	none
 *	return 	 	:	
 *					- result	: angledata
 */

angledata CoDroneClass::getGyroAngles()
{
	ReceiveGetData(Req_Attitude);

	angledata result;
	result.roll = attitudeRoll;
	result.pitch = attitudePitch;
	result.yaw = attitudeYaw;

	return result;
}

void CoDroneClass::getIrMessage(uint8_t *pData)
{
	byte irMassageDirection = pData[0 + 1];

	unsigned long _irMessge[4];

	_irMessge[0] = pData[1 + 1];
	_irMessge[1] = pData[2 + 1];
	_irMessge[2] = pData[3 + 1];
	_irMessge[3] = pData[4 + 1];

	irMessageReceive = ((_irMessge[3] << 24) | (_irMessge[2] << 16) | (_irMessge[1] << 8) | (_irMessge[0] & 0xff));
}

void CoDroneClass::getAttitudeData(uint8_t *pData)
{
	attitudeRoll = ((pData[1 + 1] << 8) | (pData[0 + 1] & 0xff));
	attitudePitch = ((pData[3 + 1] << 8) | (pData[2 + 1] & 0xff));
	attitudeYaw = ((pData[5 + 1] << 8) | (pData[4 + 1] & 0xff));

	receiveAttitudeSuccess = 1;
}
/*
 *	function	:	isUpsideDown()
 *	description : 	isUpsideDown() is function for check drone is upside down or not
 *					If drone is flipped return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::isUpsideDown()
{
	if (getState() == fMode_Flip)
	{
		Serial.println("is Upside down");
		return true;
	}
	return false;
}

/*
 *	function	:	isFlying()
 *	description : 	isFlying() is function for check drone is flying or not
 *					If drone is flipped return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::isFlying()
{
	if (getState() == fMode_Flight)
	{
		Serial.println("is Flying");
		return true;
	}
	Serial.println("is not Flying");
	return false;
}

/*
 *	function	:	isReadyToFly()
 *	description : 	isReadyToFly() is function for check drone is ready or not
 *					If drone is flipped return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::isReadyToFly()
{
	if (getState() == fMode_Ready)
		return true;
	return false;
}

/*
 *	function	:	lowBatteryCheck()
 *	description : 	lowBatteryCheck() is function for check lowBattery or not 
 *					If drone is 30% or low return true. else return false.
 *	param 		:	none
 *	return 	 	:	none
 */
boolean CoDroneClass::lowBatteryCheck()
{
	if (getBatteryPercentage() <= 30)
		return true;
	return false;
}

void CoDroneClass::getRssiData(uint8_t *pData)
{
	rssi = pData[0 + 1];
	rssi = rssi - 256;
}

//-------------------------------------------------------------------------------------------------------//
