#ifndef IBT_2_MOTOR_H	// IBT2 MOTOR DRIVER LIBRARY
#define	IBT_2_MOTOR_H

#include "Arduino.h" // include Arduino.h as it is essential

class IBT_2_MOTOR
{
  public:
    // The pins that are on the IBT2 Motor Driver
    // Should be defined in this specific order
    IBT_2_MOTOR(int R_EN,int L_EN,int R_PWM,int L_PWM); 
    void turnForward(int speed);
    void turnBackward(int speed);
  private:
    int _R_EN; 
    int _L_EN; 
    int _R_PWM; 
    int _L_PWM; 
};

#endif