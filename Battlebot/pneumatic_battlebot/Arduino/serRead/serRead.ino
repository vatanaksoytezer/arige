#include <SoftwareSerial.h>
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

String dataTransmit;

void setup() {

Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
XBee.begin(9600);
}

void loop() {

if(Serial.available()) 
  {
  //dataTransmit = Serial.readString();// read the incoming data as string
  XBee.write(Serial.read());
  }

}
