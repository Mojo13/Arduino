#include <EEPROM.h>                                // Include the EEPROM library

/**************************************************************************************************************
       Settings
**************************************************************************************************************/
// #define crossTime 3000
// #define changeTime 1500
// #define waitTime 3000



// PIN CONFIGUTRATION
#define NSRedLightCar  13
#define NSYellowLightCar  12
#define NSGreenLightCar  11
#define EWRedLightCar  10
#define EWYellowLightCar  9
#define EWGreenLightCar  8
#define PedGreen  7
#define PedRed  6
                         
/***************************************************************************************************************
       End of settings
**************************************************************************************************************/

int NSCross = 1;                             // North South Crossing Flag 1= on  & 0 = off
int EWCross = 0;                             // East West Crossing Flag 1= on  & 0 = off
int TestHW = 1;                              // Test Hardware Flag 1= on  & 0 = off
int LightDuration =0;                        // Count 0
int LightLimit = 2000 ;                      // 2 seconds
int PushButton = 2;                          // Pedestrian Crossing button
int ButtonState = digitalRead(PushButton);   // read the input pin:
int PedPress = 0;
int CrossTime = 10000;
int PedBlink = 5000;
int Count = 0;

/**************************************************************************************************************
       Setup runs once at beginning of code
**************************************************************************************************************/

void setup()
{
  Serial.begin(9600);                           // Setup for serial communuication at 9600bps    
  pinMode(NSRedLightCar, OUTPUT);               // Define pin modes
  pinMode(NSYellowLightCar, OUTPUT);
  pinMode(NSGreenLightCar, OUTPUT);
  pinMode(EWRedLightCar, OUTPUT);                      
  pinMode(EWYellowLightCar, OUTPUT);
  pinMode(EWGreenLightCar, OUTPUT);
  pinMode(PedGreen, OUTPUT);
  pinMode(PedRed, OUTPUT);
  pinMode(PushButton, INPUT);                   // make the pushbutton's pin an input
  
}

/**************************************************************************************************************
       Subroutine - Checks for button press for Pelican
**************************************************************************************************************/
void Pelican()
{
           ButtonState = digitalRead(PushButton);     // read Pelican button
           if(ButtonState == 1) {PedPress =1;}  
           Serial.print("PedPress Button Flag ");         // Serial confirmation of Button State
           Serial.println(PedPress,DEC);  
}



/**************************************************************************************************************
       The loop routine runs over and over again forever:
**************************************************************************************************************/
 void loop()                                    
{
  
//*************************************************************************** Test hardware


    
    
 if (TestHW == 1) {
        Serial.print("Before Hardware Test received: ");   // Serial confirmation of flag settings
        Serial.println(TestHW, DEC);
    
        digitalWrite(NSRedLightCar, HIGH);                 // Turn on lights
        digitalWrite(NSYellowLightCar, HIGH);              
        digitalWrite(NSGreenLightCar, HIGH);               
        digitalWrite(EWRedLightCar, HIGH);                 
        digitalWrite(EWYellowLightCar, HIGH);              
        digitalWrite(EWGreenLightCar, HIGH);               
        digitalWrite(PedRed, HIGH);                        
        digitalWrite(PedGreen, HIGH);                      

        delay (1000);

        digitalWrite(NSRedLightCar, LOW);                  // Turn off lights
        digitalWrite(NSYellowLightCar, LOW);               
        digitalWrite(NSGreenLightCar, LOW);                  
        digitalWrite(EWRedLightCar, LOW);                  
        digitalWrite(EWYellowLightCar, LOW);               
        digitalWrite(EWGreenLightCar, LOW);                 
        digitalWrite(PedRed, LOW);                        
        digitalWrite(PedGreen, LOW);   
        
        delay (1000);
        TestHW = 0;                                        // Set Hardware Flag test to off
        
        Serial.print("End of Hardware Test received: ");   // Serial confirmation of flag settings
        Serial.println(TestHW, DEC);
        Serial.print("Button State: ");                   // Serial confirmation of Button State
        Serial.println(PushButton,INPUT);
 }
 
//***************************************************************************  Trafic light sequence
 
if (NSCross== 1) {                                    // traffic Flag North South
           digitalWrite(PedRed, HIGH); 
           digitalWrite(PedGreen, LOW);  
           Pelican();
           digitalWrite(EWRedLightCar, HIGH);        //  Set East west to Stop
           Pelican();
           digitalWrite(NSRedLightCar, HIGH);
           delay (LightLimit);
           digitalWrite(NSRedLightCar, LOW);
           Pelican();
           digitalWrite(NSYellowLightCar, HIGH);
           delay (LightLimit);
           digitalWrite(NSYellowLightCar, LOW);
           Pelican();          
           digitalWrite(NSGreenLightCar, HIGH);
           delay (LightLimit); 
           digitalWrite(NSGreenLightCar, LOW);
           Pelican();
           NSCross=0;
           EWCross=1;
}
   
if (EWCross== 1) {                                     // traffic flag East West
           digitalWrite(PedRed, HIGH); 
           digitalWrite(PedGreen, LOW);  
           digitalWrite(NSRedLightCar, HIGH);          // Set North South to Stop
           Pelican();
           digitalWrite(EWRedLightCar, HIGH);
           delay (LightLimit);
           digitalWrite(EWRedLightCar, LOW);
           Pelican();          
           digitalWrite(EWYellowLightCar, HIGH);
           delay (LightLimit);
           digitalWrite(EWYellowLightCar, LOW);
           Pelican();         
           digitalWrite(EWGreenLightCar, HIGH);
           delay (LightLimit); 
           digitalWrite(EWGreenLightCar, LOW);
           Pelican();
           EWCross=0;
           NSCross=1;
}


if (PedPress== 1) {                                     // Pelican Flag set

           digitalWrite(EWRedLightCar, HIGH);        //  Set East west to Stop
           digitalWrite(NSRedLightCar, HIGH);        //  Set North South to Stop
           delay (5000);
           digitalWrite(PedRed, LOW);                        
           digitalWrite(PedGreen, LOW);  
           
           digitalWrite(PedGreen, HIGH); 
           delay (5000);
           digitalWrite(PedGreen, LOW); 
           Count=0;
           do{
                 Count++;
                 digitalWrite(PedGreen, LOW);  
                 delay (500);
                 digitalWrite(PedGreen, HIGH); 
                 delay (500);
                 Serial.print("Count");         // Serial confirmation of Button State
                 Serial.println(Count,DEC);  
           }while (Count < 5);
           
           digitalWrite(PedRed, HIGH);  
           digitalWrite(PedGreen, LOW); 
           PedPress=0;
           delay (5000);
           
}
 
  

}



