/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v
 GND to arduino GND
 Echo to Arduino pin 7
 Trig to Arduino pin 8

 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */

//Right sonar
#define rEchoPin 7 // Blue Echo Pin
#define rTrigPin 6 //Purp Trigger Pin

//Left sonar
#define lEchoPin 5 // Echo Pin
#define lTrigPin 4 // Trigger Pin

#define LEDPin 13 // Onboard LED

//sonar vars
int maximumRange = 400; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

//comand parser vars;
static char C_STR_START[] = "start";
static char C_STR_STOP[] = "stop";
const int C_INT_BUFF_LEN = 64;
char buffer[C_INT_BUFF_LEN];
int index;
char inChar;

/**************************************************************************************************
Function:   Comp
Parameters: unsigned short int nbr - number
            unsigned long int tout - time out value
Return val: none
Desc:       Initializes given timer to given time-out
**************************************************************************************************/
char ParseCommand(char* This) 
{
    index=0;
    while (Serial.available() > 0) // Don't read unless
                                   // there you know there is data
    {
        if(index < C_INT_BUFF_LEN) // One less than the size of the array
        {
            buffer[index] = Serial.read(); // Read a character
            index++; // Increment where to write next            
        }
    }
    
    // Null terminate the string
    buffer[index] = '\0';
    
     //sprintf(buffer, "Left: %02d", -1 );
    Serial.println("input");
    Serial.println(buffer);
    
    if (strcmp(buffer, This)  == 0) 
    {
        for (int i=0; i<C_INT_BUFF_LEN; i++) 
        {
            buffer[i]=0;
        }
        index=0;
        return(0);
    }
    else 
    {
        return(1);
    }
}

/**************************************************************************************************
Function:   setup
Parameters: none
Return val: none
Desc:       Initializes pins
**************************************************************************************************/
void setup() {
  Serial.begin (9600);

  //set right sonar pins
  pinMode(rTrigPin, OUTPUT);
  pinMode(rEchoPin, INPUT);

  //set left sonar pins
  pinMode(lTrigPin, OUTPUT);
  pinMode(lEchoPin, INPUT);

  //set LED
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
}


/**************************************************************************************************
Function:   DistanceOnLeftSonar
Parameters: none
Return val: none
Desc:       Read distance on left sonar
**************************************************************************************************/
void DistanceOnLeftSonar()
{
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */

  //left sonar
  digitalWrite(lTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(lTrigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(lTrigPin, LOW);
  duration = pulseIn(lEchoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;

  if (distance >= maximumRange || distance <= minimumRange) {
    /* Send a negative number to computer and Turn LED ON
    to indicate "out of range" */
    
    sprintf(buffer, "Left: %02d", -1 );
    Serial.println(buffer);
    
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */       
    sprintf(buffer, "Left: %02d", distance );    
    Serial.println(buffer);    
    digitalWrite(LEDPin, LOW);
  }

}

/**************************************************************************************************
Function:   DistanceOnRightSonar
Parameters: none
Return val: none
Desc:       Read distance on right sonar
**************************************************************************************************/
void DistanceOnRightSonar()
{
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */

  //left sonar
  digitalWrite(rTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(rTrigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(rTrigPin, LOW);
  duration = pulseIn(rEchoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;

  if (distance >= maximumRange || distance <= minimumRange) {
    /* Send a negative number to computer and Turn LED ON
    to indicate "out of range" */
    sprintf(buffer, "Right: %02d", -1 );    
    Serial.println(buffer);
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    sprintf(buffer, "Right: %02d", distance );    
    Serial.println(buffer);
    digitalWrite(LEDPin, LOW);
  }
}

/**************************************************************************************************
Function:   loop
Parameters: none
Return val: none
Desc:       Main loop
**************************************************************************************************/
void loop()
{
  
  while(!ParseCommand(C_STR_START));
  
  digitalWrite(rTrigPin, LOW);  
  digitalWrite(lTrigPin, LOW);
  delay(500);
  
  DistanceOnLeftSonar(); 
  
  digitalWrite(rTrigPin, LOW);  
  digitalWrite(lTrigPin, LOW);
  delay(500);
  
  DistanceOnRightSonar();
  
  //Delay 50ms before next reading.  
}
