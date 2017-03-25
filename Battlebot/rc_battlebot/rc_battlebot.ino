// DEBUG DEFINITION
#define DEBUG 0
// Left MOTOR PIN DEFINITIONS
const int R_EN_Left = 3;
const int L_EN_Left = 4;
const int RPWM_Output_Left = 6;
const int LPWM_Output_Left = 5; 

// Right MOTOR PIN DEFINITONS
const int R_EN_Right = 7;
const int L_EN_Right = 8;
const int RPWM_Output_Right = 10;
const int LPWM_Output_Right = 9;

// RECEIVER PIN DEFINITIONS AND VARIABLES
const int elevator = 11; //ch2 in rc receiver
const int aileron  = 12; //ch1 in rc receiver
const int failsafe = 13; // ch6 in rc receiver, switch A
int elevatorReadings = 0;
int aileronReadings = 0;
int failsafeReadings = 0;

// DIRECTION AND SPEED VARIABLES
int forwardSpeed; // Forward and Backward Speed
int directionalSpeed; // Right and Left Speed
int rightMotorSpeed;
int leftMotorSpeed;

// *** ROBOT MOVEMENT START
void robotMoveExecute()
{
  elevatorReadings = pulseIn(elevator, HIGH, 25000);
  aileronReadings  = pulseIn(aileron, HIGH, 25000);
  failsafeReadings = pulseIn(failsafe, HIGH, 25000);
  // Mapping the elevator readings from the transmitter to usable PWM values to be used in determining forward Speed of the Vehicle
  forwardSpeed =  map(elevatorReadings, 1075,1925, -255 , 255); 
  // Mapping the aileron readings from the transmitter to usable PWM values to be used in determining directional Speed of the Vehicle
  directionalSpeed =  map(aileronReadings, 1075,1925, -255 , 255);

  // Defining a deadzone for the forward speed
  int deadZoneSpeed = 40;
  // Motors will stop between if the pwm is wthin deadzone values
  if (forwardSpeed < deadZoneSpeed && forwardSpeed > -deadZoneSpeed)
  {
     forwardSpeed = 0;
  }

  if (directionalSpeed < deadZoneSpeed && directionalSpeed > -deadZoneSpeed)
  {
    directionalSpeed = 0;
  }

  

  if (DEBUG)
  {
    Serial.print("Forward Speed: ");
    Serial.print(forwardSpeed);
    Serial.print("\tDirectional Speed: ");
    Serial.print(directionalSpeed);
    Serial.print("\tFailsafe Value: ");
    Serial.println(failsafeReadings);    
  }
  
  // If you want to go forward
  if(forwardSpeed >= 0) 
  {
    // And also want to have a direction to the right
    if(directionalSpeed >= 0)
    {
       // Left wheel goes forward with the speed which comes from elevator channel
       analogWrite(LPWM_Output_Left, 0); // Go Forward Direction
       analogWrite(RPWM_Output_Left, forwardSpeed); // With the speed of elevator

       // Now Calculate the Right Motor's Speed
       rightMotorSpeed = forwardSpeed - directionalSpeed;
       // If the speed > 0 right motor goes forward 
       if ( rightMotorSpeed >= 0) 
       {
       analogWrite(LPWM_Output_Right, 0);
       analogWrite(RPWM_Output_Right, rightMotorSpeed);
       }
       // ELse right motor goes reverse direction
       else 
       {
       analogWrite(LPWM_Output_Right, abs(rightMotorSpeed));
       analogWrite(RPWM_Output_Right, 0);
       }
    }
    // Or Else if you want to go to the forward and Left
    else
    {
       // Right wheel goes forward with the speed which comes from elevator channel
       analogWrite(LPWM_Output_Right, 0); // Go Forward Direction
       analogWrite(RPWM_Output_Right, forwardSpeed); // With the speed of elevator

       // Now Calculate the Left Motor's Speed
       leftMotorSpeed = forwardSpeed - abs(directionalSpeed);
       // If the speed > 0 left motor goes forward 
       if ( leftMotorSpeed >= 0) 
       {
       analogWrite(LPWM_Output_Left, 0);
       analogWrite(RPWM_Output_Left, leftMotorSpeed);
       }
       // Else left motor goes reverse direction
       else 
       {
       analogWrite(LPWM_Output_Left, abs(leftMotorSpeed));
       analogWrite(RPWM_Output_Left, 0);
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
       analogWrite(LPWM_Output_Left, abs(forwardSpeed)); // Go Backward Direction
       analogWrite(RPWM_Output_Left, 0); // With the speed of elevator

       // Now Calculate the Right Motor's Speed
       rightMotorSpeed = abs(forwardSpeed) - directionalSpeed;
       // If the speed > 0 right motor goes backward 
       if (rightMotorSpeed >= 0) 
       {
       analogWrite(LPWM_Output_Right, rightMotorSpeed); // Motor goes backward
       analogWrite(RPWM_Output_Right, 0);
       }
       // ELse right motor goes forward direction
       else 
       {
       analogWrite(LPWM_Output_Right, 0);
       analogWrite(RPWM_Output_Right, abs(rightMotorSpeed)); // Motor goes forward
       }
    }
    // Else if you want to go to Left Backwards
    else
    {
       // Right wheel goes forward with the speed which comes from elevator channel
       analogWrite(LPWM_Output_Right, abs(forwardSpeed)); // Go Forward Direction
       analogWrite(RPWM_Output_Right, 0); // With the speed of elevator

       // Now Calculate the Left Motor's Speed
       leftMotorSpeed = abs(forwardSpeed) - abs(directionalSpeed);
       // If the speed > 0 left motor goes backward 
       if ( leftMotorSpeed >= 0) 
       {
       analogWrite(LPWM_Output_Left, leftMotorSpeed);
       analogWrite(RPWM_Output_Left, 0);
       }
       // Else left motor goes forward direction
       else 
       {
       analogWrite(LPWM_Output_Left, 0);
       analogWrite(RPWM_Output_Left, abs(leftMotorSpeed));
       }
    }
    
  }
  
  if (failsafeReadings < 1500) // If failsafe is activated
  {
    while(1)
    {
       failsafeReadings = pulseIn(failsafe, HIGH, 25000);
       if (DEBUG)
       {
        Serial.print("Fail Safe: ");
        Serial.println(failsafeReadings);
       }
       // Stop the motors
       analogWrite(LPWM_Output_Right, 0);
       analogWrite(RPWM_Output_Right, 0);
       analogWrite(LPWM_Output_Left, 0);
       analogWrite(RPWM_Output_Left, 0);
       if ( failsafeReadings > 1500 ) break; // If failsafe is deactivated, go back to normal mode   
    }
    if (DEBUG)
    {
      Serial.print("Deactivating failsafe mode");
    }
  }
  
} 
// ROBOT MOVEMENT END ***
 
// *** SETUP FUNCTION START 
void setup()
{
// Left MOTOR OUTPUTS & ENABLES
pinMode(RPWM_Output_Left, OUTPUT);
pinMode(LPWM_Output_Left, OUTPUT);
pinMode(R_EN_Left, OUTPUT);
pinMode(L_EN_Left, OUTPUT);
digitalWrite(R_EN_Left,HIGH);
digitalWrite(L_EN_Left,HIGH);

// Right MOTOR OUTPUTS & ENABLES
pinMode(RPWM_Output_Right, OUTPUT);
pinMode(LPWM_Output_Right, OUTPUT);
pinMode(R_EN_Right, OUTPUT);
pinMode(L_EN_Right, OUTPUT);
digitalWrite(R_EN_Right,HIGH);
digitalWrite(L_EN_Right,HIGH);

// TRANSMITTER INPUTS

pinMode(elevator, INPUT);
pinMode(aileron, INPUT);

if (DEBUG)
{
  Serial.begin(115200);
}

}
// SETUP FUNCTION START END ***


void loop()
{
    robotMoveExecute();
}
