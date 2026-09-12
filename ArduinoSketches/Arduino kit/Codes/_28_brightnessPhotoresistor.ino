/***********************************************************
File name:   28_brightnessPhotoresistor.ino
Description: Arduino and processing interactive
             Phototresistor control the brightness of the photo
Website: www.epictac.com
E-mail: info.epictac@gmail.com
Author: Tom
Date: 2015/05/02 
***********************************************************/
#define photoresistorPin 0
int sensorValue;
int outputValue;

void setup() 
{
   Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps
} 

void loop() 
{ 
   sensorValue = analogRead(photoresistorPin);
   outputValue = map(sensorValue,0,1023,0,255);//Transform data
   Serial.write(outputValue); //send data to the serial monitor
//   Serial.println(outputValue); //send data to the serial monitor
   delay(100);
}



        
        
        
        
      
