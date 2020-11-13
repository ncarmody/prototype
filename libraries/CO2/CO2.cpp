#include "CO2Sensor.h"
#include "CO2.h"
// extern int test;
#define GPIOPIN 35
// #define GPIOPIN A7
// was A4 for lora32u4II
CO2Sensor co2Sensor(GPIOPIN, 0.99, 100);

void setupCO2() {
	Serial.println("CO2-Sensor: ");
	// Serial.begin(9600);
	Serial.println("=== Initialized ===");
	co2Sensor.calibrate();

}

void loopCO2(int &co2Reading) {

	// Serial.println(test);
	Serial.println("in CO2.cpp");

	// for(int i=0; i<=5;i++){

	co2Reading = co2Sensor.read();
	Serial.print("CO2 value: ");
	Serial.println(co2Reading);
	delay(1000);
	// }
}
