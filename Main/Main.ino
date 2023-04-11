#include <led_ctrl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <log.h>
#include <String.h>
#include <RTClib.h>
#include <lte.h>
#include <mqtt_client.h>
#include <ecc608.h>
#include <ArduinoJson.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <stdlib.h> 

#define iled PIN_PE2 // D5
#define vout PIN_PD6 // A0
#define RXD2 PIN_PF5 // RX
#define TXD2 PIN_PF4 // TX
#define USART_RXMODE0_bm  (1<<1)  /* Receiver Mode bit 0 mask. */
#define AQDataSize 144 // 12 readings per hour * 12 hours
#define location 0 // used as a unique id for each location so we can track data
#define TIME_TO_SLEEP_IN_SECS 280

Adafruit_BME280 bme; // I2C
RTC_PCF8523 rtc; // I2C

struct airQuality { // 18 bytes in total, memory block is 20 bytes 20 bytes * 144 = 2880 bytes
  float NO2; // 4 bytes
  float O3; // 4 bytes
  uint32_t time; // 4 bytes
  unsigned short dust; // 2 bytes
  int humidity; // 2 bytes
  byte temp; // 1 byte
  byte id = location; // 1 byte
};

airQuality AQData[AQDataSize]; // global array structure for the data that is going to be sent to AWS
int counter = 0; // global counter that will be used to itterate through AQData

void setup(void) {
  // using serial3 for printing for debuging
  Serial3.begin(9600);

  // initialization of dust sensor
  initDust();

  // initialization of Zmod sensor
  initZmod();

  // initialization of the PFC 8523 RTC
  initRTC();

  // initialization for the BME sensor
  initBME();

  // Initalize onboard RTC
  RTC_init();

  // set type of sleep mode, this is the one that uses the minimum amount of power
  // Only module that will be on is the RTC, everything else is off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable(); // enables the sleep mode functionality

  // initializaiton of AWS... also ensures that the application doesnt move on if there was no connection to AWS established
  int setup = -1;
  while (setup != 0) {
    setup = setupAWS();
  }
  
  // power up zmod
  powerUpZMOD();

  // 30 minute warm up period
  warmUpZmod();
}

void loop(void) {
  uint32_t time = getTime() + 300;
  if (counter < AQDataSize) {
    fillData();
  } else {
    // call function to send data to AWS
    sendData();
    counter = 0; // will reset counter and have all the data overwritten
    fillData();
  }
  
  delay(100);
  
  for(int i = 0; i < TIME_TO_SLEEP_IN_SECS; i++) // sleep for 10 seconds
  {
    sleep_cpu();
  }

  while (time > getTime()) {
    delay(1000);
  }
}

// function that will slowly fill the data structure throughout the day
void fillData() {
  //powerUpZMOD();
  //found this to be the only way to get consistently accurate data from the dust sensor
  warmUpDust();
  AQData[counter].dust = static_cast<unsigned short>(getDust());
  AQData[counter].NO2 = getNO2().toFloat();
  AQData[counter].O3 = getO3().toFloat();
  AQData[counter].humidity = getHumidity();
  AQData[counter].temp = getTemp();
  AQData[counter].time = getTime();
  //powerDownZMOD();
  counter++;
  printData();
}

String stringify(int lower, int upper) { // currently we have to send data in 24 data point blocks
  DynamicJsonDocument jsonDoc(3000); // size limited due to memory constraints so the data will have to serialized in portions
  JsonArray jsonArray = jsonDoc.to<JsonArray>();
  for (int ii = lower; ii < upper; ii++) {
    JsonObject jsonItem = jsonArray.createNestedObject();
    jsonItem["n"] = AQData[ii].NO2;
    jsonItem["o"] = AQData[ii].O3;
    jsonItem["d"] = AQData[ii].dust;
    jsonItem["h"] = AQData[ii].humidity;
    jsonItem["c"] = AQData[ii].temp;
    jsonItem["t"] = AQData[ii].time;
    jsonItem["i"] = AQData[ii].id;
  }
  String stringJson;
  serializeJson(jsonArray, stringJson);
  return stringJson;
}

void printData() {
  Serial3.print("Counter: ");
  Serial3.println(counter);
  Serial3.print("Dust: ");
  Serial3.println(AQData[counter - 1].dust);
  Serial3.print("NO2: ");
  Serial3.println(AQData[counter - 1].NO2);
  Serial3.print("O3: ");
  Serial3.println(AQData[counter - 1].O3);
  Serial3.print("Humidity: ");
  Serial3.println(AQData[counter - 1].humidity);
  Serial3.print("Temperature: ");
  Serial3.println(AQData[counter - 1].temp);
  Serial3.print("Time: ");
  Serial3.println(AQData[counter - 1].time);
  Serial3.print("ID: ");
  Serial3.println(AQData[counter - 1].id);   
  Serial3.println();
}