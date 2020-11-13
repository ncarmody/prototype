#include "DHT.h"
//here we use pin IO14 of ESP32 to read data
// #define DHTPIN 17esp32
//was set for lora32u4 II #define DHTPIN 18
#define DHTPIN 4
//our sensor is DHT22 type
#define DHTTYPE DHT22
//create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);
void setupDHT() {

  Serial.println("DHT22 sensor setup!");
  //call begin to start sensor
  dht.begin();
}

void loopDHT(float &hRead, float &tRead) {
  //use the functions which are supplied by library.
  Serial.println("in dgtForEsp32.cpp");

  Serial.print("Humidity: ");
  Serial.println(hRead);
  hRead = dht.readHumidity();
  // Read temperature as Celsius (the default)
  
  tRead = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(tRead);

  // Check if any reads failed and exit early (to try again).
  if (isnan(hRead) || isnan(tRead)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // print the result to Terminal
  Serial.print("Humidity: ");
  Serial.print(hRead);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(tRead);
  Serial.println(" *C ");
  hRead;
  tRead;
}