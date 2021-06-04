#include "mcp_can.h"
#include <SPI.h>
#include <PS3BT.h>
#include <PS3USB.h>
#include <usbhub.h>
#ifdef dobogusinclude

#include <spi4teensy3.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

#ifdef USBMODE
PS3USB PS3(&Usb);
#else
BTD Btd(&Usb);
PS3BT PS3(&Btd);
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
#endif

bool printTemperature, printAngle;



const int SPI_CS_PIN = 6;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


void setup() 
{
   Serial.begin(115200);
   
if (Usb.Init() == -1) 
  {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  
    while (CAN_OK != CAN.begin(CAN_1000KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    //attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
    
  

}
 unsigned char flagRecv = 0;
     unsigned char len = 0;
    unsigned char buf[8];
      unsigned long id =0;
      
void MCP2515_ISR()//interrupt callback function
{
        //read data,  len: data length, buf: data buf
    CAN.readMsgBufID(&id, &len, buf); 
     flagRecv=1;
}




 
void loop() 
{

 Usb.Task(); 
 unsigned char buttondata[8] = { 0, };
 CAN.readMsgBufID(&id, &len, buf); 
 if (PS3.PS3Connected || PS3.PS3NavigationConnected)
 {
	 if (id ==0x74)
	 {
		 Serial.println("receeive ok");
		 
		 if (PS3.getButtonPress(LEFT))
		 {
			 buttondata[0] |= 1 << 7;
		 }
		 if (PS3.getButtonPress(DOWN))
		 {
			 buttondata[0] |= 1 << 6;
		 }
		 if (PS3.getButtonPress(RIGHT))
		 {
			 buttondata[0] |= 1 << 5;
		 }
		 if (PS3.getButtonPress(UP))
		 {
			 buttondata[0] |= 1 << 4;
		 }
		 if (PS3.getButtonPress(START))
		 {
			 buttondata[0] |= 1 << 3;
		 }
		 if (PS3.getButtonPress(SELECT))
		 {
			 buttondata[0] |= 1 << 0;
		 }
		 if (PS3.getButtonPress(R3))
		 {
			 buttondata[0] |= 1 << 2;
		 }
		 if (PS3.getButtonPress(L3))
		 {
			 buttondata[0] |= 1 << 1;
		 }
		 if (PS3.getButtonPress(SQUARE))
		 {
			 buttondata[1] |= 1 << 7;
		 }
		 if (PS3.getButtonPress(CROSS))
		 {

			 buttondata[1] |= 1 << 6;
		 }
		 if (PS3.getButtonPress(CIRCLE))
		 {
			 buttondata[1] |= 1 << 5;
		 }
		 if (PS3.getButtonPress(TRIANGLE))
		 {

			 buttondata[1] |= 1 << 4;
		 }
		 if (PS3.getButtonPress(R1))
		 {
			 buttondata[1] |= 1 << 3;
		 }
		 if (PS3.getButtonPress(L1)) {
			 buttondata[1] |= 1 << 2;
		 }
		 if (PS3.getAnalogButton(R2) > 180)
		 {

			 buttondata[1] |= 1 << 1;
		 }
		 if (PS3.getAnalogButton(L2) > 180)
		 {

			 buttondata[1] |= 1 << 0;
		 }
		 if (PS3.getAnalogHat(RightHatX))
		 {

			 buttondata[2] = PS3.getAnalogHat(RightHatX);
		 }
		 if (PS3.getAnalogHat(RightHatY))
		 {
			 buttondata[3] = PS3.getAnalogHat(RightHatY);
		 }
		 if (PS3.getAnalogHat(LeftHatX)) {
			 buttondata[4] = PS3.getAnalogHat(LeftHatX);
		 }
		 if (PS3.getAnalogHat(LeftHatY))
		 {
			 buttondata[5] = PS3.getAnalogHat(LeftHatY);
		 }

		 CAN.sendMsgBuf(0x75, 1, 6, buttondata);
		 Serial.println("send ok");
	 }

	 else if (id ==0x70)
	 {
		 CAN.sendMsgBuf(0x71, 1, 0, 0);
	 }
 }
}
