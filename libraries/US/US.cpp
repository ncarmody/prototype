#include "US.h"
// defines pins numbers
// const int trigPin = 23;esp32
const int trigPin = 12;
// const int echoPin = 4;esp32
const int echoPin = 13;
// defines variables
long duration;
int distance;
#include <unistd.h>
#include "Arduino.h"
#define HOWMANY 20





void setupUS()
{	
	pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
	pinMode(echoPin, INPUT); // Sets the echoPin as an Input
	/*Serial.begin(9600);*/ // Starts the serial communication
}

void loopUS(float &meanDistance)
{	
	setupUS();
	/*int meanDistance_n = meanDistance;*/
	double mean = 0.0;
	int i = 0;
	while(i<=HOWMANY){
		// Clears the trigPin
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
		// Sets the trigPin on HIGH state for 10 micro seconds
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
		// Reads the echoPin, returns the sound wave travel time in microseconds
		duration = pulseIn(echoPin, HIGH);
		// Calculating the distance
		distance = duration * 0.034/2;


		i++;

		mean+=distance;
	}
	mean = mean/HOWMANY;
	meanDistance = (float)mean;
}
