#include <TM1638.h>
#include <TM1638QYF.h>
#include "Joystick.h"

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 27, 0,
  true, true, false, false, false, false,
  true, true, false, false, false);

#define DIO 8
#define CLK 9
#define STB 10

TM1638QYF module(DIO, CLK, STB);
word mode;

//int pushBtn = 0;
int currVal = -1;
const int C_INT_MAX_VAL = 9999;

// digital pin 2 has a pushbutton attached to it. Give it a name:
#define pintBtn  11
#define pintBtn  12

void setup() 
{
  // make the pushbutton's pin an input:
  // Initialize Button Pins
  pinMode(pintBtn, INPUT_PULLUP);
  pinMode(pintBtn, INPUT_PULLUP);
  
  //init joystic lib
  Joystick.begin();

  //init  QYF module 
  module.setupDisplay(true, 7);
  mode = 0;

  module.setDisplayToString("BIT", 1);
  delay(2500);
  module.setDisplayToDecNumber(0, 1);
}

void SingleButtonPush(byte button)
{
    Joystick.pressButton(button);
    delay(250); //500
    Joystick.releaseButton(button ); 
}

void AppendNumercValue(int param)
{
  if(currVal < 0 )
  {
    currVal = param;
    return ;
  } 
  
  if(currVal > C_INT_MAX_VAL)
  {
     currVal = param;
     return ;
  }

  int newVal = (currVal * 10) + param;
  if(newVal <= C_INT_MAX_VAL )
    currVal = newVal;
  else
    currVal = param;
}

void PushBtnAndDisplayIt(TM1638QYF* module, byte buttonNo)
{
  
   AppendNumercValue(buttonNo);          
   module->setDisplayToDecNumber(currVal, 1 << 4);

   //send button push
   if(buttonNo == 0)
    SingleButtonPush(10);
   else
    SingleButtonPush(buttonNo -1);
}

void update(TM1638QYF* module, word* mode) 
{

  
  //get buttons state  
  word buttons = module->getButtons();  

  // button pressed - change mode
  if (buttons != 0) 
  {
    //Serial.print("Button:");
    //Serial.print(buttons);
    //Serial.print("\n");
       
    *mode = buttons >> 1;
    //Serial.print("Mode :");
    //Serial.print(*mode);
    //Serial.print("\n curValue");
    //Serial.print(currVal);
     switch (*mode) 
     {
        //button 1
          case 0: 
            PushBtnAndDisplayIt(module, 1);
          break;
            
          //button 2
          case 1:
            PushBtnAndDisplayIt(module, 2);            
            break;
            
          //button 3
          case 2:
            PushBtnAndDisplayIt(module, 3);            
            break;
            //empty
          case 4:                 
            break;

            //button 4
          case 8:
              PushBtnAndDisplayIt(module, 4);            
            break;

            //button 5
          case 16:
             PushBtnAndDisplayIt(module, 5);      
            break;
            
            //button 6
          case 32:
              PushBtnAndDisplayIt(module, 6);
            break;
          case 64:            
            break;

             //button 7
          case 128:
             PushBtnAndDisplayIt(module, 7);     
            break;

             //button 8
          case 256:
             PushBtnAndDisplayIt(module, 8);       
            break;
            
             //button 9
          case 512:
             PushBtnAndDisplayIt(module, 9);       
            break;

             //button 9
          case 4096:
             PushBtnAndDisplayIt(module, 0);       
            break; 
          
          //clear  
          case 2048:
            currVal = 0;
            module->setDisplayToDecNumber(currVal, 1 << 4);
            SingleButtonPush(12);
           break; 
            //enter
         case 8192:
            currVal = 0;
            module->setDisplayToDecNumber(currVal, 1 << 4);
            SingleButtonPush(13);
            break;
             
          default:
            break;
     }
  }
}
// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap = 11;


#define C_INT_MAX_BTNS 2

// Last state of the button
int lastButtonState[4] = {0,0,0,0};
void loop() {

  //qyf module
  update(&module, &mode);

  // Read pin values
  for (int index = 0; index < C_INT_MAX_BTNS; index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      //Joystick.setButton(index, currentButtonState);
      SingleButtonPush(17 + index );
      lastButtonState[index] = currentButtonState;
    }
  }
  delay(1); 
}
