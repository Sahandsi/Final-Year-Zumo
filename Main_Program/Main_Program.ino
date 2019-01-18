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
bool sideBorder = false;
bool walldetected = false;

ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int incomingByte;      // a variable to read incoming serial data into


void setup() {
  Serial.begin (9600);
  // put your setup code here, to run once:
  //  Serial.begin(9600);
  incomingByte = Serial.read();
  while (incomingByte != 'h')
  {
    incomingByte = (char) Serial.read();
  }

  caliberate();


}

void loop() {

  switch (robotStatus) {
    case 0:
      manual();
      Serial.println("in Manual mode");

    case 1:
      autonomous();
      Serial.println("in Auto mode");
  }
}

void manual() {


  while ((sideBorder == false) && (walldetected == false))
  {
    incomingByte = Serial.read();

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

    if ((incomingByte == 'Y') || (incomingByte == 'y')) {

      Serial.print("Auto on!");
      robotStatus = 1;
      break;
    }

  }
}

void caliberate() {

  int i;
  for (i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    lineSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);

  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  buzzer.play(">g32>>c32");

}

void autonomous() {

}
