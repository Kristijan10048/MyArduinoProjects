/***************************************************************************
  This is a library example for the HMC5883 magnentometer/compass

  Designed specifically to work with the Adafruit HMC5883 Breakout
  http://www.adafruit.com/products/1746
 
  *** You will also need to install the Adafruit_Sensor library! ***

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries with some heading example from
  Love Electronics (loveelectronics.co.uk)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


//Right sonar
#define rEchoPin 11 // Blue Echo Pin
#define rTrigPin 12 //Purp Trigger Pin

//Left sonar
#define lEchoPin 7 // Echo Pin
#define lTrigPin 8 // Trigger Pin

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
    
    sprintf(buffer, "Left: %02d ", -1 );
    Serial.print(buffer);
    
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */       
    sprintf(buffer, "Left: %02d ", distance );    
    Serial.print(buffer);    
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
    sprintf(buffer, "Right: %02d ", -1 );    
    Serial.print(buffer);
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    sprintf(buffer, "Right: %02d ", distance );    
    Serial.println(buffer);
    digitalWrite(LEDPin, LOW);
  }
}


void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void GetOrientationToSerialFromDigitalCompas()
{
    /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  Serial.print("Heading : "); 
  Serial.print(headingDegrees); 
  Serial.print(" Deg. ");
    
  delay(250);
}

/**************************************************************************************************
Function:   setup
Parameters: none
Return val: none
Desc:       Initializes pins
**************************************************************************************************/
void setup(void) 
{
  //set right sonar pins
  pinMode(rTrigPin, OUTPUT);
  pinMode(rEchoPin, INPUT);

  //set left sonar pins
  pinMode(lTrigPin, OUTPUT);
  pinMode(lEchoPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  /*DIgital Compas HMC5883 : Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();

}


void loop(void) 
{
  //display on serial port heading data x,y,z, heading
  GetOrientationToSerialFromDigitalCompas();
  
  digitalWrite(rTrigPin, LOW);  
  digitalWrite(lTrigPin, LOW);
  delay(100);
  
  DistanceOnLeftSonar(); 
  
  digitalWrite(rTrigPin, LOW);  
  digitalWrite(lTrigPin, LOW);
  delay(100);

  DistanceOnRightSonar();
  delay(500);
  Serial.println();
}
