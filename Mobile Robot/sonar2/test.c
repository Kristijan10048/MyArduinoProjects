/*
  * Project Name:
      HC-SR04
  * Author:
      Mark McLintock
      A rewrite of Robinson Velasquez post on Ultrasonic Sensor on libstock
      I am very new to microcontroller programming, so i couldn't figure out
      what his code was doing so i mickey moused my own.  I commented the heck
      out of this to help other beginners. I left page numbers to the datasheet,
      because i found it very helpful to link them up when learning. Please leave
      some feedback and feel free to fix my butcher job up, or teach me
  * Configuration:
      MCU: PIC18F2550
      Language MikroC:
      Oscillator: 8MHz HS
      No Prescale 4MHz Oscillator drives PLL directly
      Postscaler Primary Oscillator Source / 1
      Oscillator Frequency: 8MHz

      I wanted every clock cycle to equal 1 us
      8MHz Oscillator / 4 = 2MHz
      prescaler set to 1/2 so 2MHz/2 = 1Mhz
      1/1MHz = 1us for each clock pulse
      

      Modules Ext: 2*16 LCD Character Screen
      HC-SR04 Purchased dipmicro.com
      All "Default".
      Watch dog OFF
      Timer up OFF

      RA1= TRIGGER   PIN3
      RC2= ECHO      PIN13
*/

// Lcd module connections
sbit LCD_RS at LATB4_bit;
sbit LCD_EN at LATB5_bit;
sbit LCD_D4 at LATB0_bit;
sbit LCD_D5 at LATB1_bit;
sbit LCD_D6 at LATB2_bit;
sbit LCD_D7 at LATB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End Lcd module connections


char txt1[] = "Distance Sensor";
char txt2[] = "HC-SR04";
char txtCm[] = "Abstand:";
char txtIn[] = "Dist In:";
char chr_Distance1[7];
char chr_Distance2[7];
char chr_Distance3[7];
char edge = 0;
char capture = 0;
unsigned int long twoByte, tOld, tNew, tMathCm, tMathIn ;
int screenNow = 1;
int screenLast = 0;


void interrupt()
{
  if(PIR1.CCP1IF)
  {
      if(!edge)
      {
        CCP1CON = 0x04;              //binary 0100 change to capture every falling edge  datasheet pg141

        twoByte = CCPR1H;            //assign 8 bit high timer bits to 16 bit twoByte variable datasheet pg143
        twoByte = twoByte << 8;      //move the 8 bits from twoByte to the far left of Variable
        twoByte = twoByte | CCPR1L;  //assign 8 bit low timer bits to right 8 bits of twoByte Variable
        tOld = twoByte;              //remember rising edge time
        edge = 1;                    //set to enter calculation for falling edge
      }     else
            {
              twoByte = CCPR1H;
              twoByte = twoByte << 8;
              twoByte = twoByte | CCPR1L;
              tNew = twoByte;
              capture = 1;
              edge = 0;               //set to return to calculation for rising edge
              TMR1L = 0;              //clear timers to zero for next captures
              TMR1H = 0;
              CCPR1L = 0;
              CCPR1H = 0;
            }
      PIR1.CCP1IF = 0;

  }

}

void Calculate_Distance()
{
  if(capture)
  {
        PIE1.CCP1IE = 0;      //interupt enable bit on datasheet pg104
        capture = 0;          //clear capture flag
        CCP1CON = 0x05;       //binary 0101 set to capture rising edge

        tMathCm = (tNew - tOld)/58;   //find distance in cm
        tMathIn = (tNew - tOld)/148;  //find distance in inches

        screenNow = tMathCm;    //if distance has changed flag screen for update

        WordToStr(tMathCm,chr_Distance1);   //change values in variable to string in prep for LCD display
        WordToStr(tMathIn,chr_Distance2);

        PIR1.CCP1IF = 0;           // clear pheripheral interupt register flag datasheet pg143
        PIE1.CCP1IE = 1;           //set pheripheral interupt enable bit datasheet pg104
        Delay_ms(100);

        if(screenNow != screenLast)
         {
          Lcd_Out(1,10,chr_Distance1);
          Lcd_Out(1,15,"cm");
          Lcd_Out(2,10,chr_Distance2);
          Delay_ms(500);
         }
         screenLast = screenNow;
    }
}

void main()
{

  PCFG3_bit=1;    //ADCON register, set all anolog/digital to digital
  PCFG2_bit=1;    //see datasheet pg260
  PCFG1_bit=1;
  PCFG0_bit=1;

  TRISA = 0;       //port A set all outputs
  PORTA = 0x00;    //port A cleared
  TRISC = 0;       //port C set all outputs
  PORTC=0x00;      //port C all cleared
  CCP1CON = 0x05;  //set ccp to capture on rising edge datasheet pg141
  TRISC.F2 = 1;    //set specific port C bit 2 as input this is CCP1

  T1CON = 0x11;     //set timer prescale to 1/2 which is binary 0001 0001 datasheet pg129
  INTCON.GIE = 1;   //set general interupt bit datasheet pg24/25
  INTCON.PEIE =1;   //set pheripheral interupt bit
  PIE1.CCP1IE = 1;  //set pheripheral interupt enable bit
  PIR1.CCP1IF = 0;  //clear flag for pheripheral interupt register

  Lcd_Init();       //initialize LCD screen
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);
  Lcd_Out(1,1,txt1);
  Lcd_Out(2,6,txt2);
  Delay_ms(2000);
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1,1,txtCm);
  Lcd_Out(2,1,txtIn);


  while(1)
  {

        delay_ms(100);
        //TRISC.F2 = 0; delay_ms(100);PORTC.F2=1; Delay_us(10); PORTC.F2 = 0; TRISC.F2 = 1;delay_ms(100);    //set specific port C bit 2 as input this is CCP1
        PORTA.F1=1; Delay_us(10); PORTA.F1 = 0;       //PIN 3 SEND 10uS PULSE FOR TRIGGER OF ULTRASONIC SENSOR
        Calculate_Distance();                         //http://elecfreaks.com/store/download/HC-SR04.pdf
                                                      //THIS IS LINK TO ULTRASONIC SENSOR DATASHEET
  }

}