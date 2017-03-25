#include <IBT_2_MOTOR.h>
IBT_2_MOTOR motor1(3,4,5,6); // R_EN, L_EN, R_PWM, L_PWM
IBT_2_MOTOR motor2(7,8,10,9); // R_EN, L_EN, R_PWM, L_PWM

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
}
void loop() {
  if (Serial.available() > 0)
  {
    int yon = Serial.readStringUntil(',').toInt();
    int hiz = Serial.readStringUntil('\n').toInt();


    if (yon == 1 )
    {
      if(hiz <=255 && 128<=hiz) / İLERİ
      { 
      int ilerihiz = map(hiz,255,128,0, 255);
      motor1.turnForward(ilerihiz);
      motor2.turnForward(ilerihiz);
      }
      else if(hiz <=127 && 0<=hiz){  // GERİ
      int gerihiz = map(hiz,0,127,0, 255);        
      motor1.turnBackward(gerihiz);
      motor2.turnBackward(gerihiz);
      }     
    }
    
    else if(yon == 0)
    {
      if(hiz <=255 && 128<=hiz){   //SOLh
      int solhiz = map(hiz,255,128,0, 255);      
      motor1.turnForward(solhiz);
      motor2.turnBackward(solhiz);
      }
      else if(hiz <=127 && 0<=hiz){ //SAG
      int saghiz = map(hiz,0,127,0, 255);              
      motor1.turnBackward(saghiz);
      motor2.turnForward(saghiz);
      }
    }
    else if(yon == 2)
    {
      digitalWrite(11,HIGH);
    }
  
  }
}
