#include <Arduino.h>
#include <MsTimer2.h>
#include "SevSeg.h"

SevSeg sevseg;            //给数码管命名
byte set=COMMON_CATHODE;  //若为共阳极数码管则为COMMON_ANODE 
byte number=4;            //数码管显示位数-4位
byte COM[]={10,11,12,13};
byte pins[]={3,4,5,6,7,8,9,10}; 

int time = 10;            //倒计时时间
int boomStatus = 0;       //X_boom状态
int isStarted = 0;        //判断倒计时是否开始
int isBeeping = 0;         //判断蜂鸣器是否开始

void flash()
{
  time--;
} 

void statusChange(void)
{
  Serial.println("中断开始");
  if(boomStatus < 3)
  {
    boomStatus++;
  }else{
    boomStatus = 1;
  }
}

void setup()
{ 
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(2), statusChange, FALLING);
  pinMode(2,INPUT_PULLUP);
  sevseg.begin(set,number,COM,pins);
  MsTimer2::set(1000, flash); // 1000ms period

}
 
void loop()
{
  Serial.println(digitalRead(2));
  Serial.println(boomStatus);
  sevseg.setNumber(time);
  sevseg.refreshDisplay();  //刷新显示 
  switch (boomStatus) {
    case 1:
    {
      if(isStarted == 0)
      {
        isStarted = 1;
        MsTimer2::start();
        Serial.println("倒计时开始");
      }      
      break;
    }
    case 2:
    {
      MsTimer2::stop();
      break;
    }
    case 3:
    {
      time = 10;
      isStarted = 0;
      break;
    }
    default:
      break;
  }
  
  if(time==0)
  {
    MsTimer2::stop();
    //蜂鸣器
  }
   
}