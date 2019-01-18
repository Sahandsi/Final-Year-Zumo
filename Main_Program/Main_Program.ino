/*------------------------------------------------------------------------
  external libraries
  -------------------------------------------------------------------------*/
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>
/*------------------------------------------------------------------------
  global variable declarations
  -------------------------------------------------------------------------*/
#define LED 13

// these might need to be tuned for different motor types
#define NUM_SENSORS        6  // 
#define REVERSE_SPEED     50 // 
#define TURN_SPEED        100
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     150 // ms
#define MAX_DISTANCE      30  // 
#define TRIGGER_PIN        2  // 
#define ECHO_PIN           6  // 
#define STRING_TERMINATOR "!" // used as the last char to be sent over serial connection

int robotStatus = 0;
int motorSpeed;
int speed = 100;

ZumoMotors motors;
ZumoReflectanceSensorArray sensors;
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int incomingByte;      // a variable to read incoming serial data into


void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  //incomingByte = Serial.read();

  while(!Serial);
Serial.begin (9600);


}

void loop() {

// if (incomingByte == 'u')
//{
////int speed = Serial.parseInt();
//speed = 50;
////if (speed >= 0 && speed <= 255)
////Serial.println(speed);
////digitalWrite(ledPin, HIGH);
////analogWrite(5, speed);
////analogWrite(6, speed);
////    motors.setRightSpeed(speed);
////    motors.setLeftSpeed(speed);
//}
// if (incomingByte != 'u')
//{
//  speed=100;
//}

  
  switch (robotStatus) {
    case 0:
      manual();
       if (incomingByte == 'u')
{
//int speed = Serial.parseInt();
speed = 0;
Serial.println("hello");
//if (speed >= 0 && speed <= 255)
//Serial.println(speed);
//digitalWrite(ledPin, HIGH);
//analogWrite(5, speed);
//analogWrite(6, speed);
//    motors.setRightSpeed(speed);
//    motors.setLeftSpeed(speed);
}
 else if (incomingByte != 'u')
{
  speed=100;

}
    case 1:
      autonomous();
  }

}


void manual() {
  incomingByte = Serial.read();
//
//  if (incomingByte == 'U') {
//    motorSpeed = 50;
//    Serial.print("moving 20");
//    delay(1);
//  }
//  else if (incomingByte == 'P'){
//    motorSpeed = 100;
//    Serial.print("moving 100");
//    delay(1);
//  }

  if ((incomingByte == 'W') || (incomingByte == 'w')) {

    Serial.print("Moving Forward");
    motors.setRightSpeed(speed);
    motors.setLeftSpeed(speed);
//    delay(2);

  }

  if ((incomingByte == 'S') || (incomingByte == 's')) {

    Serial.print("Moving Backward");
    motors.setLeftSpeed(-motorSpeed);
    motors.setRightSpeed(-motorSpeed);
    delay(2);

  }

  if ((incomingByte == 'A') || (incomingByte == 'a')) {

    Serial.print("Moving Left");
    motors.setLeftSpeed(0);
    motors.setRightSpeed(motorSpeed);
    delay(2);
  }

  if ((incomingByte == 'D') || (incomingByte == 'd')) {

    Serial.print("Moving Right");
    motors.setLeftSpeed(motorSpeed);
    motors.setRightSpeed(0);
    delay(2);
  }

  if ((incomingByte == 'K') || (incomingByte == 'k')) {

    Serial.print("Stop!");
    motors.setSpeeds(0, 0);

  }

}

void autonomous() {
}
