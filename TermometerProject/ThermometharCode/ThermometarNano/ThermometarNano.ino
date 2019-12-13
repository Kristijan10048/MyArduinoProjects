#include "debug.h"
#include "Kalman.h"

//7 segment display
#define SEGMENT_ON  HIGH 
#define SEGMENT_OFF LOW   

#define DIGIT_ON  HIGH
#define DIGIT_OFF LOW  
 
//Global variables for displaying 4 digit  numbers
int digits[4];

//digit pins (1-digit, 2-digit .... on the Display)
const byte digitPins[] = {10,13,12, 11};

//segments pins
const byte segA = 7; //Display pin 14  
const byte segB = 9; //Display pin 16  
const byte segC = 3; //Display pin 13  
const byte segD = 5; //Display pin 3  
const byte segE = 6; //Display pin 5  
const byte segF = 8; //Display pin 11  
const byte segG = 2; //Display pin 15  
const byte segDot = 4;

//TEMP Sensor
// Define the A0 pin as “sensor”
#define tempSensor A0 

// in case if there is offset between actual temperature and readed temperature
//55 for usb power supply
#define offset 134

//Global  variables to read the value from the Arduino’s pin A0
int rawvoltage; 
float kelvin;
float tmp;
float oldTempValue = 0;

//configure kalman filter
Kalman filter = Kalman(0.125,32,1023,0);

//Given a number, turns on those segments  
//If number == 10, then turn off number  
void LightNumber(int numberToDisplay) 
{    
   switch (numberToDisplay)
   {    
   case 0:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_ON);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_OFF);  
    break;
    
   case 1:  
    digitalWrite(segA, SEGMENT_OFF);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_OFF);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_OFF);  
    digitalWrite(segG, SEGMENT_OFF);  
    break;  
    
   case 2:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_OFF);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_ON);  
    digitalWrite(segF, SEGMENT_OFF);  
    digitalWrite(segG, SEGMENT_ON);  
    break;  
    
   case 3:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_OFF);  
    digitalWrite(segG, SEGMENT_ON);  
    break;  
    
   case 4:  
    digitalWrite(segA, SEGMENT_OFF);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_OFF);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_ON);  
    break;  
    
   case 5:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_OFF);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_ON);  
    break;  
    
   case 6:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_OFF);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_ON);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_ON);  
   break;  
    
   case 7:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_OFF);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_OFF);  
    digitalWrite(segG, SEGMENT_OFF);  
    break;  
    
   case 8:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_ON);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_ON);  
    break;  
    
   case 9:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_ON);  
    digitalWrite(segC, SEGMENT_ON);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_ON);  
    break;  

   case -10:  
    digitalWrite(segA, SEGMENT_ON);  
    digitalWrite(segB, SEGMENT_OFF);  
    digitalWrite(segC, SEGMENT_OFF);  
    digitalWrite(segD, SEGMENT_ON);  
    digitalWrite(segE, SEGMENT_ON);  
    digitalWrite(segF, SEGMENT_ON);  
    digitalWrite(segG, SEGMENT_OFF);  
    break;
    
   case 10:
   default:
    digitalWrite(segA, SEGMENT_OFF);  
    digitalWrite(segB, SEGMENT_OFF);  
    digitalWrite(segC, SEGMENT_OFF);  
    digitalWrite(segD, SEGMENT_OFF);  
    digitalWrite(segE, SEGMENT_OFF);  
    digitalWrite(segF, SEGMENT_OFF);  
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(segDot, SEGMENT_OFF);  
    break;  
   }  
}

void TurnOffDigitPins()
{
  digitalWrite(digitPins[0], DIGIT_OFF);
  digitalWrite(digitPins[1], DIGIT_OFF);
  digitalWrite(digitPins[2], DIGIT_OFF);
  digitalWrite(digitPins[3], DIGIT_OFF);
}
  
//
void DisplayNumber(int number, int noOfDigits, int floatPointPosition = -1) 
{
    
  int  numDigits[noOfDigits] ;  
  int divider = 10;
  int shift   = 1;
  int i;     

   //Turn off all segments
   //LightNumber(10);
  
  for(i = 0 ; i < noOfDigits; i++)
  {
    //HACK disp C for celsius :)
    if(i == 0)
      numDigits[i] = -10;
    else
      numDigits[i] = (number % divider) / shift;
    
    shift   *= 10;
    divider *=10;
    number -=  numDigits[i];    
   }       
    
    for(i = 0; i < noOfDigits; i++)
    {
       //Turn off all segments
       LightNumber(10);
       
       //Turn off digit pins
       TurnOffDigitPins();

       digitalWrite(digitPins[i], DIGIT_ON);        
       LightNumber(numDigits[i]);
              
       //display floating point value
       if(floatPointPosition == i)       
          digitalWrite(segDot, SEGMENT_ON);
        else
          digitalWrite(segDot, SEGMENT_OFF);
       delay(1);

       //Turn off all segments  
       LightNumber(10);       
       digitalWrite(digitPins[i], DIGIT_OFF);
    }
    
   //Turn off all segments  
   LightNumber(10);

   //Turn off digit pins
   TurnOffDigitPins();  
}  


void DisplayTempValue(float temp)
{
  //add two zeros at the end
  if(temp < 100)
  {
    temp *= 100.0;   
    DisplayNumber((int)temp, 4, 2);
  }
  else
  {
    DisplayNumber((int)temp, 4);
  }
}

/* The function above declares which Arduino’s pins will be used for controlling the LCD */
void setup()
{
   pinMode(segA, OUTPUT);  
   pinMode(segB, OUTPUT);  
   pinMode(segC, OUTPUT);  
   pinMode(segD, OUTPUT);  
   pinMode(segE, OUTPUT);  
   pinMode(segF, OUTPUT);  
   pinMode(segG, OUTPUT);  
   pinMode(segDot, OUTPUT);
     
   pinMode(digitPins[0], OUTPUT);  
   pinMode(digitPins[1], OUTPUT);  
   pinMode(digitPins[2], OUTPUT);  
   pinMode(digitPins[3], OUTPUT); 

   // open the serial port at 9600 bps:
  DEBUG_INIT(9600);   
}

//Gets temperature in kelvins
float GetTemperature()
{
   rawvoltage = analogRead(tempSensor);
   //kelvin = (((rawvoltage / 1023.0) * 5.0) * 100.0);
   kelvin = (rawvoltage - offset) * 0.48828125;

   //DEBUG_PRINT("Input voltage:");
   //DEBUG_PRINTLN(rawvoltage);
      
   return kelvin;
}

const byte time_interval = 10;
const byte time_delay = 10;

float filterdTmp = 0;

void loop()
{
   //get temperature readings     
   tmp = GetTemperature();
   
   //convert to celsuis
   tmp = tmp - 273.15;

   //apply Kalman filter
   filterdTmp = filter.getFilteredValue(tmp);   
  
   if( abs(filterdTmp - oldTempValue) < 0.5)
    tmp = oldTempValue;
   else
    tmp= filterdTmp;
    
  for(int i=0; i<=time_interval; i++)
  {
    if(tmp >0)
      DisplayTempValue(tmp);
    delay(time_delay);
  }

  oldTempValue = filterdTmp;

 // delay(1);  
  //DEBUG_PRINT("Temp:");
  //DEBUG_PRINTLN(tmp);
  //DEBUG_PRINT(" ");
  //DEBUG_PRINTLN(filterdTmp);
}
