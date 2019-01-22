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
#define CALIBERATE_SPEED  150
#define FORWARD_SPEED     150
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     150 // ms
#define MAX_DISTANCE      30  //
#define TRIGGER_PIN        2  // 
#define ECHO_PIN           6  // 
//#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sensor_values[NUM_SENSORS]; // declare number of sensors on the zumo
int robotStatus;
int motorSpeed;
int calibrateData[6];
int speed = 150;
bool sideBorder = false;
bool walldetected = false;
int turn_direction;

ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
ZumoBuzzer buzzer;
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

char incomingByte;      // a variable to read incoming serial data into

#define ABOVE_LINE(sensor)((sensor) > SENSOR_THRESHOLD)


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
    case 2:
      searchRoom();
      break;
  }


}

void manual() {

  incomingByte = Serial.read();


  if (incomingByte == 'w') {

    Serial.print("Moving Forward");
    motors.setRightSpeed(speed);
    motors.setLeftSpeed(speed);
    delay(250);
    motors.setSpeeds(0, 0);
  }
  else if (incomingByte == 's') {

    Serial.print("Moving Backward");
    motors.setLeftSpeed(-speed);
    motors.setRightSpeed(-speed);
    delay(250);
    motors.setSpeeds(0, 0);
  }

  else if (incomingByte == 'a') {

    Serial.print("Moving Left");
    motors.setLeftSpeed(-90);
    motors.setRightSpeed(100);
    delay(250);
    motors.setSpeeds(0, 0);
  }
  else if (incomingByte == 'd') {

    Serial.print("Moving Right");
    motors.setLeftSpeed(100);
    motors.setRightSpeed(-90);
    delay(250);
    motors.setSpeeds(0, 0);
  }

  else if (incomingByte == 'b') {

    Serial.print("Stopped for the room");
    motors.setSpeeds(0, 0);
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

  else if (incomingByte == 'c') {

    Serial.print("Auto on!");
    robotStatus = 1;

  }
  else if (incomingByte == 'k') {

    Serial.print("Stop!");
    motors.setSpeeds(0, 0);

  }
  else if (incomingByte == 'y') {

    Serial.print("Auto on!");
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
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
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
  { motors.setSpeeds(0, 0);
    robotStatus = 0;
  }
  else if (incomingByte == 'b') {

    Serial.print("Stopped for the room");
    motors.setSpeeds(0, 0);
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
    motors.setSpeeds(0, 0);
    robotStatus = 0;
  }
  else if (sensor_values[5] >= calibrateData[5])
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[0] >= calibrateData[0]) {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else
  {
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }


}

void searchRoom() {


  incomingByte = Serial.read();

  if (incomingByte == 'w') {

    Serial.print("Moving Forward");
    motors.setRightSpeed(speed);
    motors.setLeftSpeed(speed);
    delay(250);
    motors.setSpeeds(0, 0);
  }
  else if (incomingByte == 's') {

    Serial.print("Moving Backward");
    motors.setLeftSpeed(-speed);
    motors.setRightSpeed(-speed);
    delay(250);
    motors.setSpeeds(0, 0);
  }

  else if (incomingByte == 'a') {

    Serial.print("Moving Left");
    motors.setLeftSpeed(-20);
    motors.setRightSpeed(100);
    delay(250);
    motors.setSpeeds(0, 0);
  }
  else if (incomingByte == 'd') {

    Serial.print("Moving Right");
    motors.setLeftSpeed(100);
    motors.setRightSpeed(-20);
    delay(250);
    motors.setSpeeds(0, 0);
  }
  else if (incomingByte == 'z') {

    Serial.print("Scanning");


    for (int i = 0; i < 180; i++)
    {
      if ((i > 20 && i <= 40) || (i > 60 && i <= 80) || (i > 100 && i <= 120) ||  (i > 140 && i <= 160) ) //extension of line follower and maze solver to higher numbers using trial and error
      {
        // On above numbers spin right
        motors.setSpeeds(-CALIBERATE_SPEED, CALIBERATE_SPEED);
        Serial.println(sonar.ping_cm());
        if (sonar.ping_cm() > 0)
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
        if (sonar.ping_cm() > 0)
        {
          //      personFoundMessage();
          Serial.println("Person Found");
          break;
        }
      }

      delay(40);
    }

    motors.setSpeeds(0, 0);
  }

}
