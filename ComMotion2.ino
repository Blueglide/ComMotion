/**   ComMotioN2.ino
   
   ComMotion Motor Controller Shield (Sparkfun ROB-13257) Testing.  
   
   The I2C protocol is used to support communication between the ComMotion shield 
   and the Arduino UNO microcontroller. 


   +------------------+              +----------------------+
   |                  |              | ComMotion Controller |                        
   | Arduino UNO      |              | Sparkfun  ROB-13257  |
   |                  |    Normal    | Note 1               |           
   |                  |    Shield    |                      |           +---------+
   |                  |<============>|                 M1 + |<-- Grn -->| Motor 1 |
   |                  |  Connections |                 M1 - |<-- Wht -->|         |
   |                  |              |                      |           +---------+
   |                  |              |                      |           +---------+           
   |                  |              |                 M2 + |<--     -->| Motor 2 |
   |                  |              |                 M2 - |<--     -->|         |
   +------------------+              |                      |           +---------+
                                     |                      |           +---------+                
                                     |                 M3 + |<--     -->| Motor 3 |
                                     |                 M3 - |<--     -->|         |                                 
                                     |                      |           +---------+
                 + 6-16 v -- Red --> | +                    |           +---------+
                     Gnd  ---------> | -               M4 + |<--     -->| Motor 4 |
                                     |                 M4 - |<--     -->|         |
                                     |                      |           +---------+
                                     +----------------------+     
                                   
    Note 1: With the Vin jumper OFF, the PC USB port powers the Arduino and
            the ComMotion controller is powered by the separate 12 V supply. 
            
            If you want to run the Arduino/ComMotion controller combination 
            WITHOUT a connection to the PC USB port, install the jumper on the 
            ComMotion controller card at Vin.  The Arduino will then get 5 volt 
            power from the ComMotion card

    STATUS: Version 1.0          
             
*/

#include <Wire.h>

#define COMMOTION_ADDR  30       // ComMotion Shield I²C address with all dip switches off
#define COMMOTION_OFFS   0  

void setup() {

   Wire.begin (1);         // Required to support I2C communication
   Serial.begin(9600);
   
   // Send Basic Configuration packet to controller
   BasicConfig(0,19,60,250,250,250,250,0,1);      
   
   Serial.println("Starting ComMotion2 ...");
   IndividualMotorControl(0,0,0,0); 
   delay(5000);         
}

void loop (){
      // Speed commands for the four motors. 
      // + = forward, - = reverse
      // Speed = 1..255 with 0 = stop   
      Serial.println("50% forward for 5 sec.");      
      IndividualMotorControl(128,128,128,128); 
      delay(5000);
     
      Serial.println("50% reverse for 5 sec.");      
      IndividualMotorControl(0,0,0,0); 
      delay(2000);      
      IndividualMotorControl(-128,-128,-128,-128); 
      delay(5000);        
      
      Serial.println("Ramp to full speed, 5 sec per step");                 
      for (int i=0; i<255; i+= 25){
         Serial.print("Speed command: "); Serial.println(i);
         IndividualMotorControl(i,i,i,i);
         delay(5000);
      }      
      Serial.println("Start over in 10 sec.\n");      
      IndividualMotorControl(0,0,0,0); 
      delay(10000);            
}

//--------------------------------------------------------------------------------

// The following seven supporting subroutines were downloaded from the 
// controller's Product Page at the link below and used here unchanged:
// http://letsmakerobots.com/commotion-shield-omni-and-mecanum-wheel-robots

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

