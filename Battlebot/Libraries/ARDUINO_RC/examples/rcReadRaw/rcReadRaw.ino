#include <ARDUINO_RC.h>

int receiverPins[5] = {12,11,10,9,8};
ARDUINO_RC RC(receiverPins);
int rawValue;
int mappedValue; 

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  rawValue = RC.readRaw(1); // read directly the raw value from channel
  mappedValue = RC.readAndMap(1, 1000, 2000, -255 , 255); // Map the raw value between desired values

  Serial.print("Raw: ");
  Serial.print(rawValue);
  Serial.print("\t Mapped: ");
  Serial.println(mappedValue);

}
