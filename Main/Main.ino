#include <led_ctrl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <log.h>
#include <String.h>
#include <lte.h> //New
#include <mqtt_client.h> //New
#include <low_power.h> // New
#include <veml3328.h> // New
#include <mcp9808.h> //New


#define iled PIN_PE2 // D5
#define vout PIN_PD6 // A0
#define RXD2 PIN_PF5
#define TXD2 PIN_PF4
#define USART_RXMODE0_bm  (1<<1)  /* Receiver Mode bit 0 mask. */
Adafruit_BME280 bme; // I2C

#define MQTT_SUB_TOPIC_FMT "%s/sensors"
#define MQTT_PUB_TOPIC_FMT "%s/sensors"

// ================================ TODO ================================
// 1. Need to make the data struct that will we will be sending to the AWS. 
// 2. Need to store the values we want and then sent them to sendData() method in AWS.ino that would send the values to the AWS. 
//    Alternatively we can use the sendData() in the main file. Which would  AWS file to be able to connect and the main.ino be able to 
//    store the output from the sensors and send tot he MQTT Topic.
// 3. The low power stuff was worked on a little but 90% it should work. It autmoancially wakes up the board after 12 mins. Have tested with 
//    sleeping for 1 mins and waking itseld up in 1 min.

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
  // The timing is 1min * 12 = 720sec 
  LowPower.configurePeriodicPowerSave(
        PowerSaveModePeriodMultiplier::ONE_MINUTE,
        12);
  delay(50);
}

void loop(void) {
  
  float density;
  powerUpZMOD();
  algoOpt(); // Optimization for Zmod
  density = getDust(); // Dust Sensor
  printValues(); // BME280
  Serial3.println(getAQI()); // Zmod AQI
  Serial3.println(getNO2()); // Zmod NO2
  Serial3.println(getO3()); // Zmod O3
  powerDownZMOD();

  int connection = setupAWS(); // Get a Connection to the broker
  switch (connection){
    case -1:
      Serial3.println("Unable to initialize the MQTT topics. Stopping...");
      break;
    case -2:
      Serial3.println("Failed to connect to operator");
      break;
    case -3:
      Serial3.println("Failed to connect to broker");
      break;
    case 0:
      Serial.println("Evertyhing is good");
      sendData();
      break;

  }
  Serial3.println("Closing MQTT connection");
  MqttClient.end(); // Close the MQTT connection

  // make sure the sensors are turned off
  Veml3328.begin();
  Mcp9808.begin();
  Veml3328.shutdown();
  Mcp9808.shutdown();

  LowPower.powerSave(); // Send the board into low power mode and it will automatically wake up in 12 mins or 720 secs

  delay(1000);
}
