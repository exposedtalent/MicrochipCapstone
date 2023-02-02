/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <log.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
    Log.begin(115200);
    Serial3.begin(115200);
    while(!Serial);    // time to get serial running

    unsigned status;
    
    // default settings
    // status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
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
    
    Serial3.print("-- Default Test --");
    delayTime = 5000;

}


void loop() { 
    printValues();
    delay(delayTime);
}


void printValues() {

    Serial3.print("Temperature = ");
    Serial3.print(bme.readTemperature());
    Serial3.print(" °C");
    Serial3.println();
    
    Serial3.print("Pressure = ");
    Serial3.print(bme.readPressure() / 100.0F);
    Serial3.print(" hPa");
    Serial3.println();

    Serial3.print("Approx. Altitude = ");
    Serial3.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial3.print(" m");
    Serial3.println();


    Serial3.print("Humidity = ");
    Serial3.print(bme.readHumidity());
    Serial3.print(" %");
    Serial3.println();


    Serial3.println();
    Serial3.println();
    Serial3.println();

}
