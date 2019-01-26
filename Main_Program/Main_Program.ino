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
//Speed Declarations
#define REVERSE_SPEED     50
#define TURN_SPEED        100
#define CALIBERATE_SPEED  200
#define BACKWARD_SPEED    150
#define FORWARD_SPEED     120

#define LED 13
#define STOP_SPEED        0                             // Setting engine speed to zero
#define MINIMUM_PING      0                             // The minimum for ultrasonic sensor
#define TRIGGER_PIN       2                             // Number of the wire on the ultrasonic sensor connected to the board
#define ECHO_PIN          6                             // Number of the wire on the ultrasonic sensor connected to the board
#define NUM_SENSORS       6                             // Number of sensors available on Zumo
#define MAX_DISTANCE      40                            // Max distance for ultrasonic sensor

ZumoMotors motors;                                      // Zumo Morts Setup
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); // Reflector Sensor setup
ZumoBuzzer buzzer;                                      // Buzzer Setup
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);     // NewPing setup of pins and maximum distance.

unsigned int sensor_values[NUM_SENSORS];                // Declare number of sensors on the zumo and making an array
int robotStatus;                                        // For changing the state of the robot
int roomNumber = 0;                                     // Room Counter for room array
int calibrateData[6];                                   // Array of sensors on Zumo and their caliberation stored
String foundRooms[50];                                  // Array for storing room position and number
String room;                                            // Initialiser for room side message
char incomingByte;                                      // Read incoming serial data
int endCounter;                                         // Initialiser for end counter

void setup() {
  Serial.begin (9600);
  incomingByte = Serial.read();
  Serial.println("Zumo ready");
  while (incomingByte != 'h')                           // Ignore all the messages apart from calibrate
  {
    incomingByte = (char) Serial.read();
  }

  calibrateRobot();

  robotStatus = 0;                                     // Default value for the switch statement

}

/*------------------------------------------------------------------------
  This is the loop function for the robot. It will loop the function in the
  switch statement unless changed by the user.
  -------------------------------------------------------------------------*/

void loop() {

  switch (robotStatus) {
    case 0:
      mainControl();
      break;
    case 1:
      autonomous();
      break;
    case 2:
      junction();
      break;
  }
}


/*------------------------------------------------------------------------
  This funtion sends the signall to robot to sweep from left to right and
  calibrate all the sensors and store the value into the array.
  -------------------------------------------------------------------------*/
void calibrateRobot() {

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

/*------------------------------------------------------------------------
  This is the main function of the robot and most of the controls are
  coded here.
  Also the manual code resides here.
  -------------------------------------------------------------------------*/
void mainControl() {

  incomingByte = Serial.read();

  if (incomingByte == 'w') {                   // Going forward a bit

    Serial.println("Moving Forward");
    motors.setRightSpeed(FORWARD_SPEED);
    motors.setLeftSpeed(FORWARD_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  else if (incomingByte == 's') {             // Going backward a bit

    Serial.println("Moving Backward");
    motors.setLeftSpeed(-BACKWARD_SPEED);
    motors.setRightSpeed(-BACKWARD_SPEED);
    delay(200);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  else if (incomingByte == 'a') {             // Going left a bit

    Serial.println("Moving Left");
    motors.setLeftSpeed(-TURN_SPEED);
    motors.setRightSpeed(TURN_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }
  else if (incomingByte == 'd') {             // Going right a bit

    Serial.println("Moving Right");
    motors.setLeftSpeed(TURN_SPEED);
    motors.setRightSpeed(-TURN_SPEED);
    delay(250);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  else if (incomingByte == 'r') {           // This will turn the robot on a 90 degree turn on its right axis

    Serial.println("Rotating Right");
    motors.setSpeeds(200, -100);
    delay(700);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  else if (incomingByte == 't') {           // This will turn the robot on a 90 degree turn on its right axis

    Serial.println("Rotating left");
    motors.setSpeeds(-100, 200);
    delay(700);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
  }

  if (incomingByte == 'm') {               // Horn for robot :D

    buzzer.playNote(NOTE_A(5), 250, 90);
  }

  else if (incomingByte == 'b') {         // This tells the robot if the room is either left or right and stores with its number

    Serial.println("Stopped for the room");
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    while ((incomingByte != 'a') && (incomingByte != 'd'))
    {
      incomingByte = (char) Serial.read();
    }

    // Save the room number and the side of it if user presses a is left but in other situations is right
    if (incomingByte == 'a')
    {
      roomNumber++;
      foundRooms[roomNumber] = "left";
    }

    else
    {
      roomNumber++;
      foundRooms[roomNumber] = "right";
    }


    /*------------------------------------------------------------------------
      This is output for the room number and side of it.
      For some starnge reason the IDE has problems compiling them when I joined
      them together. SO there were no choice but to make a seperate output for it.
      -------------------------------------------------------------------------*/

    room = "Room: ";
    Serial.print(room);
    Serial.print(roomNumber);
    Serial.print(" is on the ");
    Serial.print(foundRooms[roomNumber]);
    Serial.println(" side of corridor ");
  }

  else if (incomingByte == 'z') {       // This funtion will activate the search for survivors function

    searchRoom();

  }

  else if (incomingByte == 'e') {       // End counter as required by the spec

    endCounter++;
    Serial.println(" ");
    Serial.print("Reached End Number");
    Serial.print(":");
    Serial.print(endCounter);
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);

  }

  else if ((incomingByte == 'c') && (endCounter == 1)) { // This is what makes the zumo to go to T-Junction auto mode.

    endCounter++;
    robotStatus = 2;

  }

  else if ((incomingByte == 'c') && (endCounter != 1)) { // This tells the robot if search room or turning is complete.

    Serial.println("Turn Completed");
    Serial.println("Auto on!");
    robotStatus = 1;
  }


  else if (incomingByte == 'k') {                       // Emergency Stop setting the motors to 0

    Serial.println("Stop!");
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);

  }

  else if (incomingByte == 'y') {                       // This chnages the status of the robot to 1 so the switch statement on the loop switches to auto mode
    Serial.println("Auto on Mode Engaged!");
    // Sound off buzzer to denote Zumo is finished calibrating
    buzzer.play("L16 cdegreg4");
    robotStatus = 1;

  }
}

/*------------------------------------------------------------------------
  This is the auto funtion and robot will use the values stored in the
  calibrate function to auto steer until it hits a wall.
  -------------------------------------------------------------------------*/

void autonomous() {

  robotStatus = 1;
  incomingByte = Serial.read();

  sensors.read(sensor_values);

  if ( (incomingByte == 'k'))             // Emergency stop in case auto mode broke down
  { motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    robotStatus = 0;
  }
  else if (incomingByte == 'b') {         // This tells the robot if the room is either left or right and stores with its number

    Serial.println("Stopped for the room");
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    while ((incomingByte != 'a') && (incomingByte != 'd'))
    {
      incomingByte = (char) Serial.read();
    }

    // Save the room number and the side of it if user presses a is left but in other situations is right
    if (incomingByte == 'a')
    {
      roomNumber++;
      foundRooms[roomNumber] = "left";
    }

    else
    {
      roomNumber++;
      foundRooms[roomNumber] = "right";
    }

    /*------------------------------------------------------------------------
      This is output for the room number and side of it.
      For some starnge reason the IDE has problems compiling them when I joined
      them together. SO there were no choice but to make a seperate output for it.
      -------------------------------------------------------------------------*/
    room = "Room: ";
    Serial.print(room);
    Serial.print(roomNumber);
    Serial.print(" is on the ");
    Serial.print(foundRooms[roomNumber]);
    Serial.println(" side of corridor ");
    robotStatus = 0;
  }

  else if ((sensor_values[1] >= calibrateData[1] ) || (sensor_values[2] >= calibrateData[2] ) || (sensor_values[3] >= calibrateData[3] ) || (sensor_values[4] >= calibrateData[4] )) {

    // if the middle sensors detect line, stop
    motors.setSpeeds(STOP_SPEED, STOP_SPEED);
    Serial.println("Wall Detected!");
    Serial.println("Manual Mode activated");
    robotStatus = 0;
  }

  else if (sensor_values[5] >= calibrateData[5] || sensor_values[4] >= calibrateData[4])
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(200);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(150);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);

  }
  else if (sensor_values[0] >= calibrateData[0] || sensor_values[1] >= calibrateData[1]) {
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


/*------------------------------------------------------------------------
  This function which is developed to find the survivors in the room is
  Insipired from calibrate function the robot will sweep the floor longer
  and use the ultrasonic sensors.
  If the sensors detect anything more than 0 means there is object and this
  will stop the robot sweeping and output that a survivor has been found.
  -------------------------------------------------------------------------*/

void searchRoom() {

  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  delay(210);
  motors.setSpeeds(STOP_SPEED, STOP_SPEED);

  for (int i = 0; i < 180; i++)
  {
    if ((i > 20 && i <= 40) || (i > 60 && i <= 80) || (i > 100 && i <= 120) ||  (i > 140 && i <= 160) ) //extension of line follower and maze solver to higher numbers using trial and error
    {
      // On above numbers spin right
      motors.setSpeeds(-CALIBERATE_SPEED, CALIBERATE_SPEED);

      if (sonar.ping_cm() > MINIMUM_PING)
      {
        Serial.println("Person Found");
        break;
      }
    }
    else {
      // on other numbers spin left
      motors.setSpeeds(CALIBERATE_SPEED, -CALIBERATE_SPEED);

      if (sonar.ping_cm() > MINIMUM_PING)
      {
        Serial.println("Person Found");
        break;
      }
      if (sonar.ping_cm() < MINIMUM_PING)
      {
        Serial.println("No Survivor Found");
        break;
      }
    }
    delay(40);
  }
  motors.setSpeeds(STOP_SPEED, STOP_SPEED);
}

/*------------------------------------------------------------------------
  This function which is developed to find the survivors in the room is
  Insipired from calibrate function the robot will sweep the floor longer
  and use the ultrasonic sensors.
  If the sensors detect anything more than 0 means there is object and this
  will stop the robot sweeping and output that a survivor has been found.
  -------------------------------------------------------------------------*/


void junction() {

  incomingByte = Serial.read();

  sensors.read(sensor_values);

  if ( (incomingByte == 'v'))
  {
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
