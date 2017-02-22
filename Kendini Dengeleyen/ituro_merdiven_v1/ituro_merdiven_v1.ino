const int sagileri = 9;
const int saggeri = 10;
const int solileri = 8;
const int solgeri = 7;
const int solenable = 5;
const int sagenable = 6;

void ileri(){
analogWrite(sagenable, 150); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 150);
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

void dur()
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

void setup(){
Serial.begin(9600);
pinMode(sagileri,OUTPUT);
pinMode(saggeri,OUTPUT);
pinMode(solileri,OUTPUT);
pinMode(solgeri,OUTPUT);
pinMode(sagenable,OUTPUT);
pinMode(solenable,OUTPUT);
}

void geri(){
analogWrite(sagenable, 255); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 255);
digitalWrite(sagileri,HIGH);
digitalWrite(saggeri, LOW);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, LOW);
}
void loop(){

ileri();

}
