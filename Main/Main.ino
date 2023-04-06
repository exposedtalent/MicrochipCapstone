#include <led_ctrl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <log.h>
#include <String.h>
#include <RTClib.h>

#define iled PIN_PE2 // D5
#define vout PIN_PD6 // A0
#define RXD2 PIN_PF5
#define TXD2 PIN_PF4
#define USART_RXMODE0_bm  (1<<1)  /* Receiver Mode bit 0 mask. */

Adafruit_BME280 bme; // I2C
RTC_PCF8523 rtc; // I2C

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

  // initializaiton of AWS
  //initAWS();
  
  // really not sure if this is needed to be called or not
  // algoOpt();
}

void loop(void) {
  powerUpZMOD();
  getDust();
  printValues();
  Serial3.println(getAQI());
  Serial3.println(getNO2());
  Serial3.println(getO3());
  powerDownZMOD();
  delay(1000);
}