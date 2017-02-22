/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper SolStep(stepsPerRevolution, 5, 6);
// Stepper SagStep(stepsPerRevolution, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  SolStep.setSpeed(60);
 // SolStep.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  SolStep.step(stepsPerRevolution);
//  SagStep.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  SolStep.step(-stepsPerRevolution);
//  SagStep.step(-stepsPerRevolution);
  delay(500);
}
