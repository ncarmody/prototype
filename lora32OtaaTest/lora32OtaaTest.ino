/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network. It's pre-configured for the Adafruit
 * Feather M0 LoRa.
 *
 * This uses OTAA (Over-the-air activation), where where a DevEUI and
 * application key is configured, which are used in an over-the-air
 * activation procedure where a DevAddr and session keys are
 * assigned/generated for use with all further communication.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!

 * To use this sketch, first register your application and device with
 * the things network, to set or generate an AppEUI, DevEUI and AppKey.
 * Multiple devices can use the same AppEUI, but each device has its own
 * DevEUI and AppKey.
 *
 * Do not forget to define the radio type correctly in
 * arduino-lmic/project_config/lmic_project_config.h or from your BOARDS.txt.
 *
 *******************************************************************************/
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
// #include "CO2.h"
// #include "dhtForEsp32.h"
#include "structList.h"
#include "fillMyMsg.h"
// #include "structList.h"
#include "fillMyData.h"
// #include "TimerWakeUp.h"

bool GOTO_DEEPSLEEP = false;



// extern int test;
//
// For normal use, we require that you edit the sketch to replace FILLMEIN
// with values assigned by the TTN console. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN to a non-
// working but innocuous value.
//

#ifdef COMPILE_REGRESSION_TEST
# define FILLMEIN 0
#else
# warning "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
# define FILLMEIN (#dont edit this, edit the lines that use FILLMEIN)
#endif

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
// static const u1_t PROGMEM APPEUI[8]= { 0x14, 0x78, 0x03, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
// BE 7A 00 00 00 00 2F EF
static const u1_t PROGMEM APPEUI[8]= { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// static const u1_t PROGMEM APPEUI[8]= { 0xBE, 0x7A, 0x22, 0xED, 0x00, 0x00, 0x00, 0x00 };zusammen
// static const u1_t PROGMEM APPEUI[8]= { 0xBE, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x22, 0xED };ursp
// static const u1_t PROGMEM APPEUI[8]= { 0xED, 0x22, 0x00, 0x00, 0x00, 0x00, 0x7A, 0xBE };little
// static const u1_t PROGMEM APPEUI[8]= { 0xED, 0x22, 0x00, 0x00, 0x00, 0x00, 0x7A, 0xBE };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
// static const u1_t PROGMEM DEVEUI[8]= { 0xC9, 0x0D, 0xE7, 0x57, 0xD8, 0x89, 0xC6, 0x00 };
// EF2F000000007ABE
// static const u1_t PROGMEM DEVEUI[8]= { 0xBE, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x2F, 0xEF };ursp

// static const u1_t PROGMEM DEVEUI[8]= { 0xEF, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x7A, 0xBE };little
static const u1_t PROGMEM DEVEUI[8]= { 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
// static const u1_t PROGMEM APPKEY[16] = { 0xE0, 0x03, 0xA3, 0xF9, 0x78, 0xFC, 0x11, 0x38, 0x4B, 0x37, 0xB2, 0x22, 0x88, 0x0B, 0xEC, 0xF3 };
// 30BF897F
// 05AAC6B95C2184943E299B2B168F8C8D
// liitleendian{0x8D,0x8C, 0x8F, 0x16, 0x2B, 0x9B, 0x29, 0x3E, 0x94, 0x84, 0x21, 0x5C, 0xB9, 0xC6, 0xAA, 0x05}
// static const u1_t PROGMEM APPKEY[16] = { 0x05, 0xAA, 0xC6, 0xB9, 0x5C, 0x21, 0x84, 0x94, 0x3E, 0x29, 0x9B, 0x2B, 0x16, 0x8F, 0x8C, 0x8D };ursp
static const u1_t PROGMEM APPKEY[16] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

static uint8_t mydata[] = "Hello, world!";
static uint8_t txBuffer[n];
RTC_DATA_ATTR struct_message myData;
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 300;
RTC_DATA_ATTR lmic_t RTC_LMIC;
// Pin mapping
//
// Adafruit BSPs are not consistent -- m0 express defs ARDUINO_SAMD_FEATHER_M0,
// m0 defs ADAFRUIT_FEATHER_M0
//
// #if defined(ARDUINO_SAMD_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0)
// // Pin mapping for Adafruit Feather M0 LoRa, etc.
// const lmic_pinmap lmic_pins = {
//     .nss = 8,
//     .rxtx = LMIC_UNUSED_PIN,
//     .rst = 4,
//     .dio = {3, 6, LMIC_UNUSED_PIN},
//     .rxtx_rx_active = 0,
//     .rssi_cal = 8,              // LBT cal for the Adafruit Feather M0 LoRa, in dB
//     .spi_freq = 8000000,
// };
// #elif defined(ARDUINO_AVR_FEATHER32U4)
// // Pin mapping for Adafruit Feather 32u4 LoRa, etc.
// // Just like Feather M0 LoRa, but uses SPI at 1MHz; and that's only
// // because MCCI doesn't have a test board; probably higher frequencies
// // will work.
// const lmic_pinmap lmic_pins = {
// 	.nss = 8,
// 	.rxtx = LMIC_UNUSED_PIN,
// 	.rst = 4,
// 	.dio = {/*dio0*/ 7, /*dio1*/ 5, /*dio2*/ LMIC_UNUSED_PIN},  //
// 	.rxtx_rx_active = 0,
// 	.rssi_cal = 8,              // LBT cal for the Adafruit Feather 32U4 LoRa, in dB
// 	.spi_freq = 1000000,
// };
// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = LMIC_UNUSED_PIN, // was "14,"
  .dio = {26, 33, 32},
};
// #elif defined(ARDUINO_CATENA_4551)
// // Pin mapping for Murata module / Catena 4551
// const lmic_pinmap lmic_pins = {
//         .nss = 7,
//         .rxtx = 29,
//         .rst = 8,
//         .dio = { 25,    // DIO0 (IRQ) is D25
//                  26,    // DIO1 is D26
//                  27,    // DIO2 is D27
//                },
//         .rxtx_rx_active = 1,
//         .rssi_cal = 10,
//         .spi_freq = 8000000     // 8MHz
// };
// #else
// # error "Unknown target"
// #endif
// const lmic_pinmap lmic_pins = {
//   .nss = 8,
//   .rxtx = LMIC_UNUSED_PIN,
//   .rst = 4,
//   .dio = {/*dio0*/ 7, /*dio1*/ 5, /*dio2*/ LMIC_UNUSED_PIN}  // 
//     .rxtx_rx_active = 1,
//     .rssi_cal = 10,
// .spi_freq = 8000000     // 8MHz  
// };
void printHex2(unsigned v) {
	v &= 0xff;
	if (v < 16)
		Serial.print('0');
	Serial.print(v, HEX);
}

void onEvent (ev_t ev) {
	Serial.print(os_getTime());
	Serial.print(": ");
	switch(ev) {
		case EV_SCAN_TIMEOUT:
			Serial.println(F("EV_SCAN_TIMEOUT"));
			break;
		case EV_BEACON_FOUND:
			Serial.println(F("EV_BEACON_FOUND"));
			break;
		case EV_BEACON_MISSED:
			Serial.println(F("EV_BEACON_MISSED"));
			break;
		case EV_BEACON_TRACKED:
			Serial.println(F("EV_BEACON_TRACKED"));
			break;
		case EV_JOINING:
			Serial.println(F("EV_JOINING"));
			break;
		case EV_JOINED:
			Serial.println(F("EV_JOINED"));
			{
			  u4_t netid = 0;
			  devaddr_t devaddr = 0;
			  u1_t nwkKey[16];
			  u1_t artKey[16];
			  LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
			  Serial.print("netid: ");
			  Serial.println(netid, DEC);
			  Serial.print("devaddr: ");
			  Serial.println(devaddr, HEX);
			  Serial.print("AppSKey: ");
			  for (size_t i=0; i<sizeof(artKey); ++i) {
				if (i != 0)
				  Serial.print("-");
				printHex2(artKey[i]);
			  }
			  Serial.println("");
			  Serial.print("NwkSKey: ");
			  for (size_t i=0; i<sizeof(nwkKey); ++i) {
					  if (i != 0)
							  Serial.print("-");
					  printHex2(nwkKey[i]);
			  }
			  Serial.println();
			}
			// Disable link check validation (automatically enabled
			// during join, but because slow data rates change max TX
		// size, we don't use it in this example.
			LMIC_setLinkCheckMode(0);
			break;
		/*
		|| This event is defined but not used in the code. No
		|| point in wasting codespace on it.
		||
		|| case EV_RFU1:
		||     Serial.println(F("EV_RFU1"));
		||     break;
		*/
		case EV_JOIN_FAILED:
			Serial.println(F("EV_JOIN_FAILED"));
			break;
		case EV_REJOIN_FAILED:
			Serial.println(F("EV_REJOIN_FAILED"));
			break;
			break;
		case EV_TXCOMPLETE:
			Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
			if (LMIC.txrxFlags & TXRX_ACK)
			  Serial.println(F("Received ack"));
			if (LMIC.dataLen) {
			  Serial.println(F("Received "));
			  Serial.println(LMIC.dataLen);
			  Serial.println(F(" bytes of payload"));
			}
			// Schedule next transmission
			GOTO_DEEPSLEEP = true;
			// os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
			break;
		case EV_LOST_TSYNC:
			Serial.println(F("EV_LOST_TSYNC"));
			break;
		case EV_RESET:
			Serial.println(F("EV_RESET"));
			break;
		case EV_RXCOMPLETE:
			// data received in ping slot
			Serial.println(F("EV_RXCOMPLETE"));
			break;
		case EV_LINK_DEAD:
			Serial.println(F("EV_LINK_DEAD"));
			break;
		case EV_LINK_ALIVE:
			Serial.println(F("EV_LINK_ALIVE"));
			break;
		/*
		|| This event is defined but not used in the code. No
		|| point in wasting codespace on it.
		||
		|| case EV_SCAN_FOUND:
		||    Serial.println(F("EV_SCAN_FOUND"));
		||    break;
		*/
		case EV_TXSTART:
			Serial.println(F("EV_TXSTART"));
			break;
		case EV_TXCANCELED:
			Serial.println(F("EV_TXCANCELED"));
			break;
		case EV_RXSTART:
			/* do not print anything -- it wrecks timing */
			break;
		case EV_JOIN_TXCOMPLETE:
			Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
			break;

		default:
			Serial.print(F("Unknown event: "));
			Serial.println((unsigned) ev);
			break;
	}
}

void do_send(osjob_t* j){
	// Check if there is not a current TX/RX job running
	if (LMIC.opmode & OP_TXRXPEND) {
		Serial.println(F("OP_TXRXPEND, not sending"));
	} else {
		// Prepare upstream data transmission at the next possible time.


		loopFillMyData(myData);
		buildMsg(txBuffer, myData);

		// uint32_t variable = myData.humidity; 
		// loopDHT(myData.humidity, myData.temperature);
		// loopCO2(myData.co2);
		// myData.update = true;

		Serial.println("right before lmic_setTxData");
		Serial.print("Temperature: ");
		Serial.println(myData.temperature);
		Serial.print("humidity: ");
		Serial.println(myData.humidity);
		Serial.print("co2: ");
		Serial.println(myData.co2);
		// LMIC_setTxData2(0,mydata, sizeof(mydata)-1, 0);
		LMIC_setTxData2(1,txBuffer, sizeof(txBuffer)-1, 0);
		Serial.println(F("Packet queued"));
	}
	// Next TX is scheduled after TX_COMPLETE event.
}


void SaveLMICToRTC()
{
    RTC_LMIC = LMIC;
}

void LoadLMICFromRTC()
{
    LMIC = RTC_LMIC;

    // ESP32 can'*'t track millis during DeepSleep and no option to advanced millis after DeepSleep.
    // Therefore reset DutyCyles
    // Respect Fair Access Policy and Maximum Duty Cycle
    // https://www.thethingsnetwork.org/docs/lorawan/duty-cycle.html
    // https://www.loratools.nl/#/airtime
    for (u1_t bi = 0; bi < MAX_BANDS; bi++)
    {
        LMIC.bands[bi].avail = 0;
    }
    LMIC.globalDutyAvail = 0;
}

void GoDeepSleep()
{
    Serial.println(F("Go DeepSleep"));
    Serial.flush();
    esp_sleep_enable_timer_wakeup(TX_INTERVAL * 1000000);
    esp_deep_sleep_start();
}




void setup() {
	// n = 9;
	delay(5000);
	Serial.begin(115200);
	Serial.println(F("Starting"));
	// const int n = nrOfSensorValues*3;

	setupFillMyData(myData);

	// LMIC init
	os_init();
	// Reset the MAC state. Session and pending data transfers will be discarded.
	LMIC_reset();

    if (RTC_LMIC.seqnoUp != 0)
    {
        LoadLMICFromRTC();
    }


	LMIC_setLinkCheckMode(0);
	LMIC_setDrTxpow(DR_SF7,14);
	//LMIC_selectSubBand(1);

	// Start job (sending automatically starts OTAA too)
	do_send(&sendjob);
}

void loop() {
    static unsigned long lastPrintTime = 0;

	os_runloop_once();
    if (!os_queryTimeCriticalJobs(ms2osticksRound((TX_INTERVAL * 1000))) && GOTO_DEEPSLEEP == true)
    {
        SaveLMICToRTC();
        GoDeepSleep();
    }
    else if (lastPrintTime + 1000 < millis())
    {
        Serial.println(F("Cannot sleep"));
        lastPrintTime = millis();
    }
}