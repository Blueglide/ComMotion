/**   ComMotion3.ino - Like ComMotion2 but support I2C functiona are kept 
      in a separate include file, 
   
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
            
   Note 2: To use the I2C default address assignments, make sure the four address 
           selection DIP switches are all in the OFF position.
           
   Note 3: See the May 2015 comment exchange at the link below between OddBot, 
           the board developer and gadgetry, a knowledgeable user regarding what works
           on this controller and what doesn't. (Short summary: Motor control such as 
           start/stop/reverse/forward/speed work fine. gadgetry reports that the board 
           properly uses encoder feedback for motor speed control. But, returning battery 
           voltage, motor current and the like via the ComMotionStatus() function DOES 
           NOT WORK, and probably will never be fixed.  
           
           http://letsmakerobots.com/commotion-shield-omni-and-mecanum-wheel-robots?page=1
           To view exchange, scroll down to the comment section at the bottom of the page.

    STATUS: Version 1.0          
             
*/

#include <Wire.h>
#include <ComMotion.h>

void setup() {

   Wire.begin (1);         // Required to support I2C communication
   Serial.begin(9600);
   
   // Send Basic Configuration packet to controller
   // See ComMotion.h for arguement definitiion list 
   BasicConfig(0,19,60,250,250,250,250,0,1);      
   
   Serial.println("Starting ComMotion3 ...");
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
