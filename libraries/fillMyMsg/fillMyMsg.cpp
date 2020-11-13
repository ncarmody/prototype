#include "fillMyMsg.h"


uint32_t co2Binary;
uint32_t humidityBinary;
uint32_t temperatureBinary;

void convertLongToByte(uint8_t txBuffer[n], uint32_t data, int &index){
    txBuffer[index] = ( data >> 16 ) & 0xFF;
    index++;
    txBuffer[index] = ( data >> 8 ) & 0xFF;
    index++;
    txBuffer[index] = data & 0xFF;
    index++;
}





void buildMsg(uint8_t txBuffer[n], struct_message &myData)
{
    Serial.print("n ist: ");
    Serial.println(9);
    // txBuffer[3] = ( LongitudeBinary >> 16 ) & 0xFF;
    int index = 0;

    co2Binary = myData.co2*100;
    humidityBinary = myData.humidity*100;
    temperatureBinary = myData.temperature*100;
    convertLongToByte(txBuffer, co2Binary, index);
    convertLongToByte(txBuffer, temperatureBinary, index);
    convertLongToByte(txBuffer, humidityBinary, index);
    Serial.print("index ist: ");
    Serial.println(index);


}


    // More data than PAYLOAD_USE_CAYENNE
// void buildPacket(uint8_t txBuffer[n], listC list)
// {
//     Serial.print("n ist: ");
//     Serial.println(n);
//     // txBuffer[3] = ( LongitudeBinary >> 16 ) & 0xFF;
//     int index = 0;
//     for(int i=0;i<=1;i++){
//         distanceBinary = list[i].distance*100;
//         humidityBinary = list[i].humidity*100;
//         temperatureBinary = list[i].temperature*100;
//         convertLongToByte(txBuffer, distanceBinary, index);
//         convertLongToByte(txBuffer, temperatureBinary, index);
//         convertLongToByte(txBuffer, humidityBinary, index);
//         Serial.print("index ist: ");
//         Serial.println(index);

//     }



// }