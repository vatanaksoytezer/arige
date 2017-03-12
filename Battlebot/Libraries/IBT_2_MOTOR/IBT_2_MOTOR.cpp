#include "Arduino.h" // include Arduino.h as it is essential
#include "IBT_2_MOTOR.h" // include the header file of the library

// Define and Initialize Motor Pins
IBT_2_MOTOR::IBT_2_MOTOR(int R_EN,int L_EN,int R_PWM,int L_PWM)
{
  // All the used pins are output, as follows:
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  // R_EN and L_EN should be HIGH all the time
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);
  // Passing the variables to other functions
  _R_EN = R_EN;
  _L_EN = L_EN;
  _R_PWM = R_PWM;
  _L_PWM = L_PWM;
}

// If L_PWM is zero and R_PWM has an integer value motor will move forward
void IBT_2_MOTOR::turnForward(int speed)
{
  analogWrite(_R_PWM, speed); 
  analogWrite(_L_PWM, 0);
}

// If R_PWM is zero and L_PWM has an integer value motor will move backward 
void IBT_2_MOTOR::turnBackward(int speed)
{
  analogWrite(_R_PWM, 0);
  analogWrite(_L_PWM, speed);  
}