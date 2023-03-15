#include <led_ctrl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <log.h>
#include <String.h>
#define iled PIN_PE2 // D5
#define vout PIN_PD6 // A0
#define RXD2 PIN_PF5
#define TXD2 PIN_PF4
#define USART_RXMODE0_bm  (1<<1)  /* Receiver Mode bit 0 mask. */
Adafruit_BME280 bme; // I2C

void setup(void) {
  LedCtrl.begin();
  LedCtrl.startupCycle();
  LedCtrl.on(Led::USER);
  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW); //iled default closed
  Serial3.begin(9600);

  // for USART comms with the sensor
  Serial2.pins(TXD2, RXD2);
  Serial2.begin(9600);//, SERIAL_8N1);//, RXD2, TXD2);

  // initialization for the BME sensor
  unsigned status;
  status = bme.begin(0x77, &Wire1);
  if (!status) {
      Serial3.print("Starting initialization of MQTT Polling for AWS\r\n");
      Serial3.print("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial3.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial3.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial3.print("        ID of 0x60 represents a BME 280.\n");
      Serial3.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }
  
  delay(50);
}

void loop(void) {
  powerUpZMOD();
  algoOpt();
  getDust();
  printValues();
  Serial3.println(getAQI());
  Serial3.println(getNO2());
  Serial3.println(getO3());
  powerDownZMOD();
  delay(1000);
}