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

ZumoMotors motors;
ZumoReflectanceSensorArray sensors;
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.




void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
