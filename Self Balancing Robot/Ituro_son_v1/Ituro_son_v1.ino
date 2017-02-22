// http://www.bajdi.com
// Self balancing bot:
// µController = ATmega328
// 2 Pololu micro metal gear motors with 60mm wheels + DRV8833 motor controller
// 6DOF MPU6050 sensor 

#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter

Kalman kalmanX; // Create the Kalman instance

/* IMU Data */
int16_t accX, accY, accZ;
int16_t tempRaw;
int16_t gyroX, gyroY, gyroZ;

float accXangle;//, accYangle; // Angle calculate using the accelerometer
float gyroXangle;//, gyroYangle; // Angle calculate using the gyro
float kalAngleX;//, kalAngleY; // Calculate the angle using a Kalman filter

unsigned long timer;
uint8_t i2cData[14]; // Buffer for I2C data
float CurrentAngle;

// Motor controller pins
const int sagileri = 5;
const int saggeri = 4;
const int solileri = 10;
const int solgeri = 13;
const int solenable = 11;
const int sagenable = 3; 

int hiz;
unsigned long gozununferi;
int qtr;
#define qtrpin A2

//Fan pins
#define fanpin1 A0
#define fanpin2 A1
const int fanileripin =   8;
const int fangeripin  =   7;
const int fanenable   =   9;
// PID
//const float Kp = 12; //Çok İyi gibi
//const float Ki = 1.2; //Çok İyi gibi
//const float Kd = 0.72; //Çok İyi gibi
const float Kp = 11.72; //Çok İyi gibi
const float Ki = 1.37; //Çok İyi gibi
const float Kd = 3.575;
//const float Kp = 12; 
//const float Kp = 6;  //İyi gibi
//const float Ki = 1; //İyi gibi
//const float Kd = 0.05; //İyi gibi
//const float Kp = 12;  
//const float Ki = 0.6; //Çokİyi gibi
//const float Kd = 0.05; //Çokİyi gibi
//const float Kp = 12; 
//const float Ki = 0.2;
//const float Kd = 0.12;
//const float Kp = 210.91; 
//const float Ki = 56.75;
//const float Kd = 0.65;
float pTerm, iTerm, dTerm, integrated_error, last_error, error;
//const float K = 1.9*1.12;
//const float K = 2;
const float K = 1.4*1.12; 
// #define   GUARD_GAIN   10.0
// #define   GUARD_GAIN   5.0
// #define   GUARD_GAIN   12.0

void azileri(){
analogWrite(sagenable, 35); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 30);
digitalWrite(sagileri,LOW);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, LOW);
digitalWrite(solgeri, HIGH);
}
void ileri(){
analogWrite(sagenable, 70); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 65);
digitalWrite(sagileri,LOW);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, LOW);
digitalWrite(solgeri, HIGH);
}
void geri(){
analogWrite(sagenable, 65); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, 70);
digitalWrite(sagileri,HIGH);
digitalWrite(saggeri, LOW);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, LOW);
}

#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

const uint8_t IMUAddress = 0x68; // AD0 is logic low on the PCB
const uint16_t I2C_TIMEOUT = 1000; // Used to check for errors in I2C communication

uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop) {
  return i2cWrite(registerAddress, &data, 1, sendStop); // Returns 0 on success
}

uint8_t i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t length, bool sendStop) {
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  Wire.write(data, length);
  uint8_t rcode = Wire.endTransmission(sendStop); // Returns 0 on success
  if (rcode) {
    Serial.print(F("i2cWrite failed: "));
    Serial.println(rcode);
  }
  return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
}

uint8_t i2cRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes) {
  uint32_t timeOutTimer;
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  uint8_t rcode = Wire.endTransmission(false); // Don't release the bus
  if (rcode) {
    Serial.print(F("i2cRead failed: "));
    Serial.println(rcode);
    return rcode; // See: http://arduino.cc/en/Reference/WireEndTransmission
  }
  Wire.requestFrom(IMUAddress, nbytes, (uint8_t)true); // Send a repeated start and then release the bus after reading
  for (uint8_t i = 0; i < nbytes; i++) {
    if (Wire.available())
      data[i] = Wire.read();
    else {
      timeOutTimer = micros();
      while (((micros() - timeOutTimer) < I2C_TIMEOUT) && !Wire.available());
      if (Wire.available()) {
        data[i] = Wire.read();
      }
      else {
        Serial.println(F("i2cRead timeout"));
        return 5; // This error value is not already taken by endTransmission
      }
    }
  }
  return 0; // Success
}


void setup() {  
pinMode(sagileri,OUTPUT);
pinMode(saggeri,OUTPUT);
pinMode(solileri,OUTPUT);
pinMode(solgeri,OUTPUT);
pinMode(sagenable,OUTPUT);
pinMode(solenable,OUTPUT);
pinMode(fanpin1, OUTPUT);
pinMode(fanpin2, OUTPUT);
  Serial.begin(57600);
  Wire.begin();
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode 

    while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while(1);
  }

  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while(i2cRead(0x3B,i2cData,6));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;

  kalmanX.setAngle(accXangle); // Set starting angle
  gyroXangle = accXangle;
  timer = micros();
}

// Fanın Kodları
void fanileri(){
analogWrite(fanenable, 10);
digitalWrite(fanileripin,HIGH);
digitalWrite(fangeripin, LOW);
}
void fangeri(){
analogWrite(fanenable, 10);
digitalWrite(fanileripin,LOW);
digitalWrite(fangeripin, HIGH);
}



// Motorun Kodları
void Motors(){
  if (hiz > 0)
  { 
    //forward 
analogWrite(sagenable, hiz); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, hiz);
digitalWrite(sagileri,LOW);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, LOW);
digitalWrite(solgeri, HIGH);
  }
  else
  { 
    // backward
hiz = map(hiz,0,-255,0,255);
analogWrite(sagenable, hiz); //220 degeri pwm olarak motor hızını ayarlıyor 255 e kadar çıkabilir analogWrite komutunu bunun icin kullaniyoruz
analogWrite(solenable, hiz);
digitalWrite(sagileri,HIGH);
digitalWrite(saggeri, LOW);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, LOW);
}
}
void dur()
{
digitalWrite(sagileri, HIGH);
digitalWrite(saggeri, HIGH);
digitalWrite(solileri, HIGH);
digitalWrite(solgeri, HIGH);
}

void Pid(){
  error = 180 - CurrentAngle;  // 180 = level
  pTerm = Kp * error;
  integrated_error += error;
  iTerm = Ki * integrated_error;
  dTerm = Kd * (error - last_error);
//  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
//  iTerm = Ki * integrated_error * 0.025;
//  dTerm = Kd * (error - last_error) / 0.025;
  last_error = error;
// dTerm = 0;
  hiz = constrain(K*(pTerm + iTerm + dTerm), -255, 255);
  // hiz = constrain(abs(K*(pTerm + iTerm + dTerm)), 0, 255);
}
void ronaldoyahizlazim() {
  if(hiz > 0) {
  analogWrite(fanpin1, hiz*3.5);
  analogWrite(fanpin2, 0);
  }
  else {
  hiz =  map(hiz,0,-255,0,255);
    analogWrite(fanpin1, 0);
  analogWrite(fanpin2, hiz*3.5);
  }

  }


void dof()
{
  while(i2cRead(0x3B,i2cData,14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = ((i2cData[6] << 8) | i2cData[7]);  
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroY = ((i2cData[10] << 8) | i2cData[11]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;
  double gyroXrate = (double)gyroX/131.0;
  CurrentAngle = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000);
  timer = micros();
}

void robotileri() {
    runEvery(25)  // run code @ 40 Hz(normali 25)
  {
  //  fanileri();
  //esiyo();
  //  Serial.println(CurrentAngle);
      geri();
  delay(3);
    dof();
    ronaldoyahizlazim();

  
 //   Serial.print( "Error:");
 //   Serial.println(error);
    if (CurrentAngle <= 180.52 && CurrentAngle >= 179.48)
    {
      dur();
    }
    else{
    if (CurrentAngle < 230 && CurrentAngle > 130)
    {
    Pid();
    Motors();
    }
    else
    {
      dur();
    }
  }
  }
}
void robotgeri(){
    runEvery(25)  // run code @ 40 Hz(normali 25)
  {
  //  fanileri();
  //esiyo();
  //  Serial.println(CurrentAngle);
      ileri();
  delay(3);
    dof();
    ronaldoyahizlazim();

  
 //   Serial.print( "Error:");
 //   Serial.println(error);
    if (CurrentAngle <= 180.52 && CurrentAngle >= 179.48)
    {
      dur();
    }
    else{
    if (CurrentAngle < 230 && CurrentAngle > 130)
    {
    Pid();
    Motors();
    }
    else
    {
      dur();
    }
  }
  }
}
void robotdengede() {
   runEvery(23)  // run code @ 40 Hz(normali 25)
  {
   // Serial.println(CurrentAngle);
    dof();
 //   Serial.print( "Error:");
 //   Serial.println(error);
    if (CurrentAngle <= 180.52 && CurrentAngle >= 179.48)
    {
      dur();
    }
    else{
    if (CurrentAngle < 230 && CurrentAngle > 130)
    {
    Pid();
    Motors();
    }
    else
    {
      dur();
    }
  }
  }
}
void loop() {

  if(digitalRead(7) == HIGH){
    robotdengede();
  }
  else {
    while(1){
qtr = analogRead(qtrpin);
robotileri();
if ( qtr > 700) {
  while(1){
     if (CurrentAngle <= 180.52 && CurrentAngle >= 179.48){
      azileri();
      delay(1);

      
      for( int k = 0; k < 5000; k++) {
        robotdengede();
      }
         while(1){
      robotgeri();  
    }
   }
   else{
    robotileri();
   }
 
  }
}

}
}
}

