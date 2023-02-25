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