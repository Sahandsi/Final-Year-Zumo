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
#define STOP_SPEED        0
#define MINIMUM_PING      0
#define TRIGGER_PIN       2
#define ECHO_PIN          6
#define NUM_SENSORS       6
#define MAX_DISTANCE      30
#define REVERSE_SPEED     50
#define TURN_SPEED        100
#define CALIBERATE_SPEED  150
#define FORWARD_SPEED     150


ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sensor_values[NUM_SENSORS]; // declare number of sensors on the zumo
int robotStatus;
int calibrateData[6];
char incomingByte;      // a variable to read incoming serial data into


void setup() {
  Serial.begin (9600);
  incomingByte = Serial.read();
  Serial.println("Zumo ready");
  while (incomingByte != 'h')
  {
    incomingByte = (char) Serial.read();
  }

  caliberate();

  robotStatus = 0;

}

void loop() {

  switch (robotStatus) {
    case 0:
      manual();
      break;
    case 1:
      autonomous();
      break;
  }
}

void manual() {

  incomingByte = Serial.read();


  if (incomingByte == 'w') {

    Serial.print("Moving Forward");
    motors.setRightSpeed(FORWARD_SPEED);
    motors.setLeftSpeed(FORWARD_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }
  else if (incomingByte == 's') {

    Serial.print("Moving Backward");
    motors.setLeftSpeed(-FORWARD_SPEED);
    motors.setRightSpeed(-FORWARD_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  else if (incomingByte == 'a') {

    Serial.print("Moving Left");
    motors.setLeftSpeed(-TURN_SPEED);
    motors.setRightSpeed(TURN_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }
  else if (incomingByte == 'd') {

    Serial.print("Moving Right");
    motors.setLeftSpeed(TURN_SPEED);
    motors.setRightSpeed(-TURN_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  else if (incomingByte == 'b') {

    Serial.print("Stopped for the room");
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    while ((incomingByte != 'a') && (incomingByte != 'd'))
    {
      incomingByte = (char) Serial.read();
    }
    if (incomingByte == 'a') {
      Serial.print("Room is on the left");
    }
    else {
      Serial.print("Room is on the right");
    }

  }

  else if (incomingByte == 'z') {

    searchRoom();

  }

  else if (incomingByte == 'c') {

    Serial.print("Auto on!");
    robotStatus = 1;

  }
  else if (incomingByte == 'k') {

    Serial.print("Stop!");
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);

  }
  else if (incomingByte == 'y') {
    Serial.print("Auto on!");
    // Sound off buzzer to denote Zumo is finished calibrating
    buzzer.play("L16 cdegreg4");
    robotStatus = 1;

  }
}

void caliberate() {

  sensors.init();

  for (int i = 0; i < 160; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70) || (i > 90 && i <= 110) ||  (i > 130 && i <= 150) ) //extension of line follower and maze solver to higher numbers using trial and error
    {
      // On above numbers spin right
      motors.setSpeeds(-CALIBERATE_SPEED, CALIBERATE_SPEED);

    }
    else
    {
      // on other numbers spin left
      motors.setSpeeds(CALIBERATE_SPEED, -CALIBERATE_SPEED);
    }

    sensors.calibrate();
    delay(10);
  }

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    calibrateData[i] = sensors.calibratedMaximumOn[i];
  }
  motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  buzzer.play(">g32>>c32");
  Serial.println("Calibration complete");

}

void autonomous() {

  robotStatus = 1;
  incomingByte = Serial.read();

  sensors.read(sensor_values);
  //    Serial.print(sensor_values[5]);
  //    Serial.print(calibrateData[5]);

  if ( (incomingByte == 'k'))
  { motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    robotStatus = 0;
  }
  else if (incomingByte == 'b') {

    Serial.print("Stopped for the room");
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    while ((incomingByte != 'a') && (incomingByte != 'd'))
    {
      incomingByte = (char) Serial.read();
    }
    if (incomingByte == 'a') {
      Serial.print("Room is on the left");
    }
    else {
      Serial.print("Room is on the right");
    }
    robotStatus = 2;
  }
  else if (sensor_values[3] > calibrateData[3] ) {
    // if the middle sensors detect line, stop
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    robotStatus = 0;
  }
  else if (sensor_values[5] >= calibrateData[5])
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(200);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(150);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[0] >= calibrateData[0]) {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(200);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(150);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else
  {
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }

}

void searchRoom() {

  for (int i = 0; i < 180; i++)
  {
    if ((i > 20 && i <= 40) || (i > 60 && i <= 80) || (i > 100 && i <= 120) ||  (i > 140 && i <= 160) ) //extension of line follower and maze solver to higher numbers using trial and error
    {
      // On above numbers spin right
      motors.setSpeeds(-CALIBERATE_SPEED, CALIBERATE_SPEED);
      Serial.println(sonar.ping_cm());
      if (sonar.ping_cm() > MINIMUM_PING)
      {

        Serial.println("Person Found");
        break;
      }
    }
    else
    {
      // on other numbers spin left
      motors.setSpeeds(CALIBERATE_SPEED, -CALIBERATE_SPEED);
      Serial.println(sonar.ping_cm());
      if (sonar.ping_cm() > MINIMUM_PING)
      {
        Serial.println("Person Found");
        break;
      }
    }

    delay(40);
  }

  motors.setSpeeds(STOP_SPEED, STOP_SPEED);
}
