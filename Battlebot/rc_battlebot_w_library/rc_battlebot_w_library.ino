// DEBUG DEFINITION
#define DEBUG 0 // TURN 0 to 1 for DEBUGGING

#include <ARDUINO_RC.h>
#include <IBT_2_MOTOR.h>

// Initialize Receiver Pins and Define Variables
int receiverPins[2] = {11,12}; // ch1 , ch2
ARDUINO_RC RC(receiverPins);

// Inıtialize Motors and Variables
IBT_2_MOTOR leftMotor(3,4,6,5); // R_EN, L_EN, R_PWM, L_PWM
IBT_2_MOTOR rightMotor(7,8,10,9); // R_EN, L_EN, R_PWM, L_PWM
int forwardSpeed; // Forward and Backward Speed
int directionalSpeed; // Right and Left Speed
int rightMotorSpeed; // Right Motor Speed
int leftMotorSpeed; // Left Motor Speed
const int deadZoneSpeed = 20; // deadZone for better processing data 


void setup() {
  
  if (DEBUG)
  {
    Serial.begin(115200);
  }

}

void loop() {
  
  forwardSpeed = RC.readAndMap(1, 1075, 1925, -255 , 255);
  directionalSpeed =  RC.readAndMap(2, 1075, 1925, -255 , 255);


    // Motors will stop between if the pwm is within deadzone values
  if (forwardSpeed < deadZoneSpeed && forwardSpeed > -deadZoneSpeed)
  {
     forwardSpeed = 0;
  }

  if (directionalSpeed < deadZoneSpeed && directionalSpeed > -deadZoneSpeed)
  {
    directionalSpeed = 0;
  }

  // Print the values if you want to debug
    if (DEBUG)
  {
    Serial.print("Forward Speed: ");
    Serial.print(forwardSpeed);
    Serial.print("\tDirectional Speed: ");
    Serial.println(directionalSpeed);    
  }

    // If you want to go forward
    if(forwardSpeed >= 0) 
  {
    // And also want to have a direction to the right
    if(directionalSpeed >= 0)
    {
       // Left wheel goes forward with the speed which comes from elevator channel
       leftMotor.turnForward(forwardSpeed);

       // Now Calculate the Right Motor's Speed
       rightMotorSpeed = forwardSpeed - directionalSpeed;
       // If the speed > 0 right motor goes forward 
       if ( rightMotorSpeed >= 0) 
       {
       rightMotor.turnForward(rightMotorSpeed);
       }
       // ELse right motor goes reverse direction
       else 
       {
       rightMotor.turnBackward(abs(rightMotorSpeed));
       }
    }
    // Or Else if you want to go to the forward and Left
    else
    {
       // Right wheel goes forward with the speed which comes from elevator channel
       rightMotor.turnForward(forwardSpeed);

       // Now Calculate the Left Motor's Speed
       leftMotorSpeed = forwardSpeed - abs(directionalSpeed);
       // If the speed > 0 left motor goes forward 
       if ( leftMotorSpeed >= 0) 
       {
       leftMotor.turnForward(leftMotorSpeed);
       }
       // Else left motor goes reverse direction
       else 
       {
       leftMotor.turnBackward(abs(leftMotorSpeed));
       }
    }    
  }
  // Else you want to go to the backward direction
  else
  {
    // If you want to go to Right Direction as well
    if(directionalSpeed >= 0) 
    {
       // Left wheel goes backward with the speed which comes from elevator channel
       leftMotor.turnBackward(abs(forwardSpeed));

       // Now Calculate the Right Motor's Speed
       rightMotorSpeed = abs(forwardSpeed) - directionalSpeed;
       // If the speed > 0 right motor goes backward 
       if (rightMotorSpeed >= 0) 
       {
       rightMotor.turnBackward(rightMotorSpeed);
       }
       // ELse right motor goes forward direction
       else 
       {
       rightMotor.turnForward(abs(rightMotorSpeed));
       }
    }
    // Else if you want to go to Left Backwards
    else
    {
       // Right wheel goes backward
       rightMotor.turnBackward(abs(forwardSpeed));

       // Now Calculate the Left Motor's Speed
       leftMotorSpeed = abs(forwardSpeed) - abs(directionalSpeed);
       // If the speed > 0 left motor goes backward 
       if ( leftMotorSpeed >= 0) 
       {
       leftMotor.turnBackward(leftMotorSpeed);
       }
       // Else left motor goes forward direction
       else 
       {
       leftMotor.turnForward(abs(leftMotorSpeed));
       }
    }
    
  }
  

}
