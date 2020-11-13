
#include "fillMyData.h"
#include "CO2.h"
#include "dhtForEsp32.h"
#define ID 1
// #include "Arduino.h"
// #include <list.h>


int countForDynamicMonitoring = 0;
bool flag = false;

void setupFillMyData(struct_message &myData)

{	
	myData.id = ID;
	// countForDynamicMonitoring++;

	// Serial.begin(115200);
	// while(!Serial){
	// 	;
	// }
	setupDHT();	
	setupCO2();
	// test = 10000;
	


}

void loopFillMyData(struct_message &myData)
{	


	// if(true){
	// 	//!flag
	// 	delay(1000);
	// // *********************************measure temperature & humidity
	// 	loopDHT(myData.humidity, myData.temperature);
	// 	flag = true;

	// }else if(true){
	// 	//flag
	
	// 	// *********************************measure CO2
	// 	delay(1000);
	// 	loopCO2(myData.co2);
	// 	myData.count = countForDynamicMonitoring;

	// 	flag = false;
	// 	myData.update = true;
	// 	// ******************************sleep


	// }

	delay(1000);
// *********************************measure temperature & humidity
	loopDHT(myData.humidity, myData.temperature);
	


	// *********************************measure CO2
	delay(1000);
	loopCO2(myData.co2);
	myData.count = countForDynamicMonitoring;

	
	myData.update = true;
	// ******************************sleep
	countForDynamicMonitoring++;


	


	
}



