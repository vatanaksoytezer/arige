#include <IBT_2_MOTOR.h>

IBT_2_MOTOR motor1(3,4,5,6); // R_EN, L_EN, R_PWM, L_PWM
IBT_2_MOTOR motor2(7,8,10,9); // R_EN, L_EN, R_PWM, L_PWM


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

motor1.turnForward(128);
motor2.turnForward(128);
delay(5000);
motor1.turnBackward(128);
motor2.turnBackward(128);
delay(5000);
}
