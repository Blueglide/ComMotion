/*
   ComMotion.h   
   Support I2C functions

*/

#ifndef COM_MOTION_H
#define COM_mOTION_H

#include <Arduino.h>

void BasicConfig(byte mode, byte chassis, byte lowbat, byte maxcur1, byte maxcur2, 
                 byte maxcur3, byte maxcur4, byte i2coffset, byte i2cmaster);
void EncoderConfig(int maxrpm, int encres, byte reserve, byte maxstall);
void MotorControl(int velocity, int angle, int rotation);
void IndividualMotorControl(int m1, int m2, int m3, int m4);
void SerialConfig(int baud1, int baud2, byte smode);
void SendSerialData(byte port, String sdata);
void ComMotionStatus(byte mcu, byte request);

#endif
  
