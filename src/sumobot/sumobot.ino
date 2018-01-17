/*  Name: Jasmine, Laura, Jennifer
 *  Date: April
 *  Purpose: Control the sumobot
*/

#include <NewPing.h> //Import the NewPing Library to handle multiple Ultrasonic sensors efficiently and accurately 

#define E1 10  // Enable Pin for motor 1
#define E2 11  // Enable Pin for motor 2

#define I1 9  // Control pin 1 for motor 1
#define I2 7  // Control pin 2 for motor 1
#define I3 4  // Control pin 1 for motor 2
#define I4 8  // Control pin 2 for motor 2

int frontLDR, backLDR, rightLDR, leftLDR; //Stores the LDR readings.

NewPing ultrasonic1(2,3,50); // The constructor for the NewPing class for initializing the front ultrasonic sensor by passing in the trig pin, echo pin and maximum distance
NewPing ultrasonic2(6,5,50); // The constructor for the NewPing class for initializing the back ultrasonic sensor by passing in the trig pin, echo pin and maximum distance

int frontUltrasonic; //Stores distance in cm from the results of the front ultrasonic sensor readings
int backUltrasonic; //Stores distance in cm from the results of the back ultrasonic sensor readings

const int numReadings = 10; // The number of readings to average out the Ultrasonic readings

int readings [numReadings]; // The array of the readings for the front ultrasonic
int readings1 [numReadings]; // The array of the readings for the back ultrasonic
int readIndex, readIndex1 = 0; // Integer storing the index of the readings array for smoothing values
int total, total1 = 0; // Sum of all the values in the readings array to find the average value for smoothing
int average, average1 = 0; // The average value of the readings array

int normSpeed = 255; // The normal speed of the sumbot.

/* The serial monitor is set up to take LDR readings.
   The enable pins for the motor are set to output.
   The readings and readings 1 arrays are intialized to 0.
*/
void setup ()
{
  Serial.begin (9600);

  pinMode (E1, OUTPUT);
  pinMode (E2, OUTPUT);
  
  for (int thisReading = 0 ; thisReading < numReadings ; thisReading++)
    {
        readings [thisReading] = 0;
        readings1 [thisReading] = 0;
    }
}

/* For every iteration of the loop, readings of the 4 LDRs and 2 ultrasonic sensors are taken.
 * The ultrasonic sensor values are smoothed by calling smoothing();
 * The readings are printed to the serial monitor by calling printSerial();
 * 
 * If the front LDR sensor detects white, then the sumobot moves backward.
 * Else if the back LDR sensor detects white, then the sumobot moves forward.
 * Else if the right LDR sensor detects white, then the sumobot turns left.
 * Else if the left LDR sensor detects white, then the sumobot turns right.
 * Else, if none of the LDR sensors detect white, the ultrasonic sensors are used to guide movement.
 * If the front ultrasonic sensor detects its opponent so that the opponent is closer to the front sensor than the back, the sumobot moves forward.
 * Else if the back ultrasonic sensor detects its opponent so that the opponent is closer to the back sensor than the front, the sumobot moves backward.
 * Else the sumobot spins on the spot.
*/
void loop ()
{  
  frontLDR = analogRead (A3);
  backLDR = analogRead (A1);
  rightLDR = analogRead (A4);
  leftLDR = analogRead (A0);
  
  frontUltrasonic = ultrasonic1.ping_cm();
  backUltrasonic = ultrasonic2.ping_cm();
  
  //smoothing();
   
  printSerial();

  //spin();

  if (frontLDR < 500)
  {
    backward();
  } 
  else if (backLDR < 500)
  {
    analogWrite (E1, normSpeed);
    analogWrite (E2, normSpeed);
    forward();
  }
  else if (rightLDR < 500)
  {
    analogWrite (E1, normSpeed);
    forward();
  }
  else if (leftLDR < 500)
  {
    analogWrite (E2, normSpeed);
    forward();
  }
  else
  {
    if ((frontUltrasonic<backUltrasonic && frontUltrasonic!=0) ||(frontUltrasonic>backUltrasonic && backUltrasonic==0))
    {
      Serial.print("f ");
      forward();
    }
    else if ((backUltrasonic<frontUltrasonic && backUltrasonic!=0)||(backUltrasonic>frontUltrasonic&& frontUltrasonic==0))
    {
      Serial.print("b ");
      backward();
    } 
    else 
    {
      Serial.print("spin ");
      spin();
    }
  }
}

//The smoothing function averages a specified number of readings (numReadings) of each ultrasonic sensor in order to obtain smooth values.
void smoothing()
{
  total = total - readings [readIndex];
  readings [readIndex] = frontUltrasonic;
  total = total + readings [readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings)
      readIndex = 0;
  frontUltrasonic = total / numReadings;
  
   total1 = total1 - readings1 [readIndex1];
  readings1 [readIndex1] = backUltrasonic;
  total1 = total1 + readings1 [readIndex1];
  readIndex1 = readIndex1 + 1;
  if (readIndex1 >= numReadings)
      readIndex1 = 0;
  backUltrasonic = total1 / numReadings;
}

//The printSerial function prints the readings for the 4 LDRs and 2 ultrasonic sensors on the serial monitor.
void printSerial()
{
  Serial.print (frontLDR);
  Serial.print (" ");
  Serial.print (backLDR);
  Serial.print (" ");
  Serial.print (rightLDR);
  Serial.print (" ");
  Serial.print(leftLDR);
  Serial.print (" ");
  
  Serial.print("Front = ");
  Serial.print(frontUltrasonic);
  Serial.print("    Back = ");
  Serial.println(backUltrasonic);
}

//The forward function moves the sumobot forward.
void forward()
{
    digitalWrite (I1, HIGH);
    digitalWrite (I2, LOW);
    digitalWrite (I3, HIGH);
    digitalWrite (I4, LOW);
}


//The backward function moves the sumobot backward.
void backward()
{
    digitalWrite (I1, LOW);
    digitalWrite (I2, HIGH);
    digitalWrite (I3, LOW);
    digitalWrite (I4, HIGH);
}

//The right function turns the sumobot to the right.
void right()
{
    analogWrite (E1, normSpeed-10);
    analogWrite (E2, normSpeed);
    digitalWrite (I1, HIGH);
    digitalWrite (I2, LOW);
    digitalWrite (I3, HIGH);
    digitalWrite (I4, LOW);
}

//The left function turns the sumobot to the left.
void left()
{
    analogWrite (E1, normSpeed-10);
    analogWrite (E2, normSpeed);
    digitalWrite (I1, LOW);
    digitalWrite (I2, HIGH);
    digitalWrite (I3, LOW);
    digitalWrite (I4, HIGH);
}

//The spin function spins the sumobot.
void spin()
{
    analogWrite (E1, normSpeed-200);
    analogWrite (E2, normSpeed-200);
    digitalWrite (I1, HIGH);
    digitalWrite (I2, LOW);
    digitalWrite (I3, LOW);
    digitalWrite (I4, HIGH);
}

