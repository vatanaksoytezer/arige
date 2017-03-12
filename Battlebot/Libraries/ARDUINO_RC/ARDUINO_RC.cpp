#include "Arduino.h" // include Arduino.h as it is essential
#include "ARDUINO_RC.h" // include the header file of the library

// Inıtialize the pins as input pins
ARDUINO_RC::ARDUINO_RC(int channels[])
{
	// Acquire how many channels are used
	_channelCount = sizeof(channels)/sizeof(*channels);

	// Initialize pins
	for (int i = 0; i < _channelCount; i++)
	{
		pinMode(channels[i], INPUT);
		_channels[i] = channels[i];
	}
}

// Just read the raw values directly from the receiver and return the channels value
int ARDUINO_RC::readRaw(int ch)
{
	// The 0th element of the array is 1st channel
  _rawValue = pulseIn(_channels[ch-1], HIGH, 25000);
  return _rawValue;
}

// Read and Map the readings as you desire
int ARDUINO_RC::readAndMap(int ch,int rawMin,int rawMax,int valMin,int valMax)
{
  _mappedValue = pulseIn(_channels[ch-1], HIGH, 25000);
  // rawMin and rawMax should be obtained through readRaw function than they can be implemented through this function
  // A receiver to pwm mapping values shoudl be:
  // rawMin = 1000
  // rawMax = 2000
  // valMin = -255
  // valMax = 255
  _mappedValue = map(_mappedValue, rawMin,rawMax, valMin, valMax); 
  return _mappedValue;

}