#ifndef ARDUINO_RC_H	// ARDUINO RC RECEIVER LIBRARY
#define	ARDUINO_RC_H

#include "Arduino.h" // include Arduino.h as it is essential

class ARDUINO_RC
{
  public:
  	// Initialize the pin, which are given as an array
  	// Should be defined as int channels[] = {ch1,ch2,ch3,ch4}
    ARDUINO_RC(int channels[]);
    int readRaw(int ch);
    int readAndMap(int ch,int rawMin,int rawMax,int valMin,int valMax);
  private:
    int _rawValue;
    int _mappedValue;
    int _channels[];
    int _channelCount;
};

#endif