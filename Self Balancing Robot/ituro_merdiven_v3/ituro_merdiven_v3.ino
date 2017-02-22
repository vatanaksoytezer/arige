#include <Servo.h>
Servo myservo;
const int sagileri = 9;
const int saggeri = 10;
const int solileri = 8;
const int solgeri = 7;
const int solenable = 5;
const int sagenable = 6;
unsigned long timer;
unsigned long timer2;
unsigned long timer3;
unsigned long timer4;
const int digermotorileri = 12;

const int digermotorgeri = 13;

const int digermotorenable = 11;

void digermotorforward() {

analogWrite(digermotorenable, 175); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
digitalWrite(digermotorileri, LOW);
digitalWrite(digermotorgeri, HIGH);
}
void ileri(){
analogWrite(sagenable, 175); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 175);
digitalWrite(sagileri,LOW);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, LOW);
digitalWrite(solgeri, HIGH);
}

void sag()
{
analogWrite(solenable, 100);
analogWrite(sagenable, 220);
digitalWrite(sagileri, HIGH);
digitalWrite(saggeri, LOW);
digitalWrite(solileri, LOW);
digitalWrite(solgeri, HIGH);
}

void dur() //CIFT BANT SOGUTUCU
{
digitalWrite(sagileri, HIGH);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, HIGH);
}
void sol()
{
analogWrite(solenable, 220);
analogWrite(sagenable, 0);
digitalWrite(sagileri, LOW);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, LOW);
}


void geri(){
analogWrite(sagenable, 255); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 255);
digitalWrite(sagileri,HIGH);
digitalWrite(saggeri, LOW);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, LOW);
}







#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))
void setup(){
Serial.begin(9600);
myservo.attach(3);
pinMode(sagileri,OUTPUT);
pinMode(saggeri,OUTPUT);
pinMode(solileri,OUTPUT);
pinMode(solgeri,OUTPUT);
pinMode(sagenable,OUTPUT);
pinMode(solenable,OUTPUT);
myservo.write(130);
delay(1000);
}


void loop() {

    for ( timer = 0; timer < 30000 ; timer++) {
  ileri();
  digermotorforward();
    }
   
  sag();
  delay(3000);

    for ( timer2 = 0; timer2 < 30000 ; timer2++) {
  ileri();
  digermotorforward();
    }
    sag();
    delay(6000);
    

    for( timer3 = 0; timer3 < 30000; timer3 ++) {
      geri();
      digermotorforward();
    }
    sol();
    delay(3000);
    
    
    for( timer4 = 0; timer4 < 30000; timer4 ++) {
      geri();
      digermotorforward();  
    }
}



