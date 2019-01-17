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

int robotStatus;


ZumoMotors motors;
ZumoReflectanceSensorArray sensors;
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int incomingByte;      // a variable to read incoming serial data into


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  incomingByte = Serial.read();

}

void loop() {
  // put your main code here, to run repeatedly:
  switch (robotStatus) {
    case 0:
      manual();
    case 1:
      autonomous();
  }

}


void manual() {
  incomingByte = Serial.read();

  if ((incomingByte == 'W') || (incomingByte == 'w')) {

    Serial.print("Moving Forward");
    motors.setRightSpeed(100);
    motors.setLeftSpeed(100);
    delay(2);

  }

  if ((incomingByte == 'S') || (incomingByte == 's')) {

    Serial.print("Moving Backward");
    motors.setLeftSpeed(-100);
    motors.setRightSpeed(-100);
    delay(2);

  }

  if ((incomingByte == 'A') || (incomingByte == 'a')) {

    Serial.print("Moving Left");
    motors.setLeftSpeed(0);
    motors.setRightSpeed(100);
    delay(2);
  }

  if ((incomingByte == 'D') || (incomingByte == 'd')) {

    Serial.print("Moving Right");
    motors.setLeftSpeed(100);
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
