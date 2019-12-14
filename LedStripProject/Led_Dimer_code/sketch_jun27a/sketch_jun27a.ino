/*
 * LED Dimer with IRremote: Dims led strip using arduino nano and IR receiver to control led brightness.
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 1.o Octobeber, 2016
 * Copyright 2016 Kristijan Petkov
 * 
 */
#include <IRremote.h>

// the pin that the LED strip transistor is attached to
const int ledStripPin = 10;
const int ledPin = 13;

// the pins that  push buttons are attached to
const int btnMinusPin = 4;
const int btnPlusPin = 3;     

int btnMinusState;
int btnPlusState;

int brightness = 10;

const int interval = 1000;
unsigned long currentMillis;
unsigned long previousMillis;

int ledState = LOW;

//IR
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

//IR remote button values
#define buttonPlus 0x32E9
#define altButtonPlus 0x490

#define buttonMinus 0x72E9
#define altButtonMinus 0xC90


/**
 *  Set up function to initialize pins
 */
void setup()
{
  // initialize the serial communication:
  //Serial.begin(19200);
  
  // initialize the ledPin as an output:
  pinMode(ledStripPin, OUTPUT);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledStripPin, LOW);

   // initialize the pushbutton pin as an input:
  pinMode(btnMinusPin, INPUT);
  pinMode(btnPlusPin, INPUT);

  //IR
  irrecv.enableIRIn(); // Start the receiver
  
  //TODO
  //pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

/**
 *  Increase or decrease brightness of the LEDs based on IR remote key value
 *	Parameters :
 *		- buttonValue : IR remote button  value 
 */
void ParseIRButtons(unsigned long buttonValue)
{
    switch(buttonValue)
    {
      case altButtonPlus:
      case buttonPlus:
      {      
        if(brightness + 5 <= 255)     
          brightness +=5;

        //Serial.print("Brightness =");
        //Serial.println(brightness);
      }
      break;
      case altButtonMinus:
      case buttonMinus:
      {
        if(brightness >= 5)
          brightness -=5;
          
        //Serial.print("Brightness =");
        //Serial.println(brightness);
      }
      break;
    }
}

void loop() 
{
  // set the brightness of the LED:
  analogWrite(ledStripPin, brightness);  

  // read the state of the pushbutton value:
  btnMinusState = digitalRead(btnMinusPin);
  btnPlusState = digitalRead(btnPlusPin);
  delay(10);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (btnMinusState == HIGH) 
  {
    if(brightness - 5 >=0)
      brightness -=5;
      
    //Serial.println("Brightness =");
    //Serial.println(brightness);
  }

  if (btnPlusState == HIGH) 
  {
    if(brightness + 5 <= 255)
      brightness +=5;
      
    //Serial.println("Brightness =");
    //Serial.println(brightness);
  }  

  //Blink led on pin 13
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) 
    {
      ledState = HIGH;
    }
    else 
    {
      ledState = LOW;
    }
    
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState); 
  }

   //capture button from IR 
   if (irrecv.decode(&results)) 
   {
    //Serial.println(results.value, HEX);

    int tmpVal = results.value;   
    delay(5);
    
     // Receive the next value
    irrecv.resume();

    delay(5);
    ParseIRButtons(tmpVal);
   }
}