// The following seven supporting subroutines were downloaded from the 
// controller's Product Page at the link below and used here unchanged:
// http://letsmakerobots.com/commotion-shield-omni-and-mecanum-wheel-robots

#include <ComMotion.h>
#include <Arduino.h>
#include <Wire.h> 

#define COMMOTION_ADDR  30       // ComMotion Shield I²C address with all dip switches off
#define COMMOTION_OFFS   0 

//----- Basic Configuration, only need to send once ------------------------------------------------------------------

void BasicConfig(byte mode, byte chassis, byte lowbat, byte maxcur1, byte maxcur2, byte maxcur3, byte maxcur4, byte i2coffset, byte i2cmaster)
{
  Wire.beginTransmission(COMMOTION_ADDR+COMMOTION_OFFS);  // Initialize I²C communications with ComMotion shield
  Wire.write(1);                 // Specify that data packet is basic configuration data
  Wire.write(mode);              // 0=normal, 1=demo
  Wire.write(chassis);           // 0=3xOmni, 1=4xOmni, 2=Mecanum, 3=Individual. Add 16 to disable encoder feedback
  Wire.write(lowbat);            // 0-255    55=5.5V
  Wire.write(maxcur1);           // 0-255   255=2.55A
  Wire.write(maxcur2);           // 0-255   255=2.55A
  Wire.write(maxcur3);           // 0-255   255=2.55A
  Wire.write(maxcur4);           // 0-255   255=2.55A
  Wire.write(i2coffset);         // 0-95    a value of 32 changes ComMotion dipswitch address range to 32-63
  Wire.write(i2cmaster);         // I²C address for I²C master - used for emergency reports such as motor overload
  Wire.endTransmission();        // transmit data from I²C buffer to ComMotion shield
}

//----- Encoder Configuration, only need to send once ----------------------------------------------------------------

void EncoderConfig(int maxrpm, int encres, byte reserve, byte maxstall)
{
  Wire.beginTransmission(COMMOTION_ADDR+COMMOTION_OFFS);   // Initialize I²C communications with ComMotion shield
  Wire.write(2);                 // Specify that data packet is encoder configuration data
  Wire.write(highByte(maxrpm));  // high byte of motor RPM - 13500rpm for Scamper
  Wire.write( lowByte(maxrpm));  //  low byte of motor RPM - 8500rpm for Rover 5
  Wire.write(highByte(encres));  // high byte of encoder resolution x100 - 800 for Scamper
  Wire.write( lowByte(encres));  //  low byte of encoder resolution x100 - 200 for Rover 5
  Wire.write(reserve);           // 0-50%     reserve power - use when constant speed under variable load is critical
  Wire.write(maxstall);          // 1-255μS   number of μS between encoder pulses before stall is assumed  10 for Scamper, 25 for Rover 5
  Wire.endTransmission();       // transmit data from I²C buffer to ComMotion shield
}

//----- Motor Control for Omni and Mecanum wheels -------------------------------------------------------------------
  
void MotorControl(int velocity, int angle, int rotation)
{
  Wire.beginTransmission(COMMOTION_ADDR+COMMOTION_OFFS); // Initialize I²C communications with ComMotion shield
  Wire.write(3);                    // Specify that data packet is motor control data
  Wire.write(highByte(velocity));   // -255 to +255  velocity/direction for preconfigured chassis
  Wire.write( lowByte(velocity));   // negative values reverse direction of travel
  Wire.write(highByte(angle));      // -255 to +255  angle of travel for chassis using omni or mecanum wheels
  Wire.write( lowByte(angle));      // negative values reverse direction of travel by 180°
  Wire.write(highByte(rotation));   // -255 to +255  desired rotation speed
  Wire.write( lowByte(rotation));   // negative values reverse direction of rotation
  Wire.endTransmission();           // transmit data from I²C buffer to ComMotion shield
}

//----- Individual Motor Control ------------------------------------------------------------------------------------

void IndividualMotorControl(int m1, int m2, int m3, int m4)
{
  Wire.beginTransmission(COMMOTION_ADDR+COMMOTION_OFFS); // Initialize I²C communications with ComMotion shield
  Wire.write(3);              // Specify that data packet is motor control data
  Wire.write(highByte(m1));   // -255 to +255  speed for motor 1
  Wire.write( lowByte(m1));   // negative values reverse direction of travel
  Wire.write(highByte(m2));   // -255 to +255  speed for motor 2
  Wire.write( lowByte(m2));   // negative values reverse direction of travel
  Wire.write(highByte(m3));   // -255 to +255  speed for motor 3
  Wire.write( lowByte(m3));   // negative values reverse direction of travel
  Wire.write(highByte(m4));   // -255 to +255  speed for motor 4
  Wire.write( lowByte(m4));   // negative values reverse direction of travel
  Wire.endTransmission();     // transmit data from I²C buffer to ComMotion shield
}

//----- Serial Port Configuration, only need to send once if required ----------------------------------------------
  
void SerialConfig(int baud1, int baud2, byte smode)
{
  Wire.beginTransmission(COMMOTION_ADDR+COMMOTION_OFFS);  // Initialize I²C communications with ComMotion shield
  Wire.write(4);                 // Specify that data packet is serial configuration data
  Wire.write(highByte(baud1));   // Baud rate for ComMotion serial port 1
  Wire.write( lowByte(baud1));   // Common baud rates are: 1200,2400,4800,9600,14400,28800,38400,57600,115200
  Wire.write(highByte(baud2));   // Baud rate for ComMotion serial port 2 (Xbee / WiFly)
  Wire.write( lowByte(baud2));   // Common baud rates are: 1200,2400,4800,9600,14400,28800,38400,57600,115200
  Wire.write(smode);             // 0-4  Determines how ComMotion shield handles incoming data (default=4)
  Wire.endTransmission();        // transmit data from I²C buffer to ComMotion shield
}

//----- Send Serial Data, destination depends on serial config -----------------------------------------------------

void SendSerialData(byte port, String sdata)
{
  if(sdata.length()>31) return;        // Serial data cannot be more than 31 bytes in length due to size of I²C buffer
  Wire.beginTransmission(COMMOTION_ADDR+COMMOTION_OFFS);  // Initialize I²C communications with ComMotion shield
  Wire.write(5);                       // Specify that data packet is serial data to be sent
  for (byte i=0;i<sdata.length();i++)  // Scan through string 1 byte at a time (ignores trailing null charactor)
  {
    Wire.write(sdata.charAt(i));       // stores data in I²C buffer (maximum 31 charactors)
  }
  Wire.endTransmission();              // transmit data from I²C buffer to ComMotion shield
}



//------ Status Request can be directed to MCU 1 or MCU 2 ----------------------------------------------------------

void ComMotionStatus(byte mcu, byte request) // value for mcu must be 0 or 1. Each bit of the request byte is a separate request.
{
  Wire.beginTransmission(COMMOTION_ADDR + COMMOTION_OFFS + mcu);  // Initialize I²C communications with ComMotion shield MCU 1 or 2
  Wire.write(6);                             // Specify that data packet is a status request
  Wire.write(request);                       // Each bit of the request byte returns a different status report
  Wire.endTransmission();                    // transmit data from I²C buffer to ComMotion shield
}
