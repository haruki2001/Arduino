#include <PS3BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>


USB Usb;BTD Btd(&Usb);
PS3BT PS3(&Btd);

/********** Pin 定義 **********/
/********** モータシステム **********/

#define PWMpin1A 11  //V1 IN_A
#define PWMpin1B 12  //V1 IN_B
#define PWMpin2A 2  //V2 IN_A
#define PWMpin2B 3 //V2 IN_B
#define PWMpin3A 5  //V3 IN_A
#define PWMpin3B 6  //V3 IN_B
#define PWMpin4A 7 //V4 IN_A
#define PWMpin4B 8 //V4 IN_B


/*********** シンプルモタドラ **********/
//#define PWMpin1 3 //V1
//#define PWMpin2 5 //V2
//#define PWMpin3 6 //V3
//#define PWMpin4 7 //V4

//#define Directionpin1 2  //V1
//#define Directionpin2 42  //V2
//#define Directionpin3 40  //V3
//#define Directionpin4 53  //V4

void setup() {
  // put your setup code here, to run once:

  /********** Arduino MEGA PWM周波数　変更 **********/
  TCCR1B = (TCCR1B & 0b11111000) | 0x01; //31.37255 [kHz] //pin 11,12
  TCCR3B = (TCCR3B & 0b11111000) | 0x01; //change pwm_Hz to 31.37255 [kHz] //pin2,3,5
  TCCR4B = (TCCR4B & 0b11111000) | 0x01; //change pwm_Hz to 31.37255 [kHz]//pin6,7,8
  
  
  /********** pin setup **********/

  /********** シンプルモタドラ **********/
  //pinMode(PWMpin1,OUTPUT);
  //pinMode(PWMpin2,OUTPUT);
  //pinMode(PWMpin3,OUTPUT);
  //pinMode(PWMpin4,OUTPUT);
  
  //pinMode(Directionpin1,OUTPUT);
  //pinMode(Directionpin2,OUTPUT);
  //pinMode(Directionpin3,OUTPUT);
  //pinMode(Directionpin4,OUTPUT);

  /********** モータシステム **********/
  pinMode(PWMpin1A,OUTPUT);
  pinMode(PWMpin1B,OUTPUT);
  pinMode(PWMpin2A,OUTPUT);
  pinMode(PWMpin2B,OUTPUT);
  pinMode(PWMpin3A,OUTPUT);
  pinMode(PWMpin3B,OUTPUT);
  pinMode(PWMpin4A,OUTPUT);
  pinMode(PWMpin4B,OUTPUT);
  
/********** controller setup start **********/
  Serial.begin(115200);
  #if !defined(_MIPSEL_)
  while(!Serial);
  #endif
  if(Usb.Init()==-1){
    Serial.print(F("\r\nOSC did not start\n"));
    while(1);//halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started\n"));
}

/********* setup *********/
float V1 = 0;
float V2 = 0;
float V3 = 0;
float V4 = 0;

int xx = 0;
int yy = 0;
int spin = 0;

float XX = 0.f;
float YY = 0.f;
float Spin =0.f;

int up = 0;
int down = 0;


/********** 逆運動学 **********/

void distribute_speed(float XX,float YY,float RR){
  /********** 三輪オムニ **********/
  /*
  V1=-XX*cos(2*PI/3)+YY*sin(2*PI/3)+RR;
  V2=XX*cos(4*PI/3)-YY*sin(4*PI/3)-RR;
  V3=XX-RR;
  */

  /********** 四輪オムニ **********/
  V1 = XX*cos(1.f*PI/4)+YY*sin(1.f*PI/4)+RR;
  V2 = XX*cos(3.f*PI/4)+YY*sin(3.f*PI/4)+RR;
  V3 = XX*cos(5.f*PI/4)+YY*sin(5.f*PI/4)+RR;
  V4 = XX*cos(7.f*PI/4)+YY*sin(7.f*PI/4)+RR;
}

/********** モータシステム **********/

void SetDutyM1(float duty){
  if(duty >= 0){
    analogWrite(PWMpin1A,(int)((duty/100.f)*127.f));
    analogWrite(PWMpin1B,0);
  }

  if(duty < 0){
    duty = -duty;
    analogWrite(PWMpin1A,0);
    analogWrite(PWMpin1B,(int)((duty/100.f)*127.f));
  }
}

void SetDutyM2(float duty){
  if(duty >= 0){
    analogWrite(PWMpin2A,(int)((duty/100.f)*127.f));
    analogWrite(PWMpin2B,0);
  }

  if(duty < 0){
    duty = -duty;
    analogWrite(PWMpin2A,0);
    analogWrite(PWMpin2B,(int)((duty/100.f)*127.f));
  }
}

void SetDutyM3(float duty){
  if(duty >= 0){
    analogWrite(PWMpin3A,(int)((duty/100.f)*127.f));
    analogWrite(PWMpin3B,0);
  }

  if(duty < 0){
    duty = -duty;
    analogWrite(PWMpin3A,0);
    analogWrite(PWMpin3B,(int)((duty/100.f)*127.f));
  }
}

void SetDutyM4(float duty){
  if(duty >= 0){
    analogWrite(PWMpin4A,(int)((duty/100.f)*127.f));
    analogWrite(PWMpin4B,0);
  }

  if(duty < 0){
    duty = -duty;
    analogWrite(PWMpin4A,0);
    analogWrite(PWMpin4B,(int)((duty/100.f)*127.f));
  }
}


/********** シンプルモタドラ *********/
/*
void M1_duty(float xx){//V1
  digitalWrite(Directionpin1,HIGH);
  if(xx<0){
    digitalWrite(Directionpin1,LOW);
    xx=-xx;
  }
  analogWrite(PWMpin1,(int)((xx/100.f)*127.f));
}


void M2_duty(int xx){//V2
  digitalWrite(Directionpin2,HIGH);
  if(xx<0){
    digitalWrite(Directionpin2,LOW);
    xx=-xx;
  }
  analogWrite(PWMpin2,xx);
}

void M3_duty(int xx){//V3
  digitalWrite(Directionpin3,HIGH);
  if(xx<0){
    digitalWrite(Directionpin3,LOW);
    xx=-xx;
  }
  analogWrite(PWMpin3,xx);
}

void M4_duty(int xx){//V4
  digitalWrite(Directionpin4,HIGH);
  if(xx<0){
    digitalWrite(Directionpin4,LOW);
    xx=-xx;
  }
  analogWrite(PWMpin4,xx);
}
*/


/********** その他 **********/
/*
void up_duty(int xx){
  digitalWrite(Directionpin4,HIGH);
  analogWrite(PWMpin4,xx);
}

void down_duty(int xx){
  digitalWrite(Directionpin4,LOW);
  analogWrite(PWMpin4,xx);
}
*/

void loop(){
  // put your main code here, to run repeatedly:
  Serial.println("you have to push START_button for moving robot.");
  while(1){
    Usb.Task(); //recieve controller signal
    if(PS3.getButtonClick(START)){
      break;
    }
  }

  while(1){
    Usb.Task(); //recieve controller signal

    xx = PS3.getAnalogHat(LeftHatX)- 127;
    if(xx>-10&&xx<10){
      xx=0;
    }
    yy = PS3.getAnalogHat(LeftHatY) -127;
    if(yy>-10&&yy<10){
      yy=0;
    }
    spin = PS3.getAnalogHat(RightHatX)- 127;
    if(spin>-10&&spin<10){
      spin=0;
    }

    /********** Max Duty **********/
    XX = (float)(xx/127.f*50.f);
    YY = -(float)(yy/127.f*50.f);
    Spin = -(float)(spin/127.f*25.f);

    /********** PS3 確認 **********/
    /*
    Serial.print(xx);
    Serial.print(" ");
    Serial.print(yy);
    Serial.print(" ");
    Serial.print(spin);
    Serial.print("\n");
    */

    /********** Duty 確認 **********/
    
    Serial.print(XX);
    Serial.print(" ");
    Serial.print(YY);
    Serial.print(" ");
    Serial.print(Spin);
    Serial.print("\n");
    
    
    distribute_speed(XX,YY,Spin);
    
    /*duty_setup*/
    SetDutyM1(V1);
    SetDutyM2(V2);
    SetDutyM3(V3);
    SetDutyM4(V4);
    //M1_duty(XX);
    //M2_duty(V2/2);
    // M3_duty(V3/2);
    // M4_duty(V4/2);

  }
}
  
