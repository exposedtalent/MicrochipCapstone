#include <log.h>
#include <led_ctrl.h>
#include <Wire.h>
#include "SparkFunBME280.h"
#define iled PIN_PE2 // D5
#define vout PIN_PD6 // A0
BME280 mySensor;

void setup(void) {
  LedCtrl.begin();
  LedCtrl.startupCycle();
  LedCtrl.on(Led::USER);
  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW); //iled default closed
  Log.begin(115200);
  Log.setLogLevel(LogLevel::DEBUG);
  delay(50);

  //Begin communication over I2C
  /*Wire.begin();
  if (mySensor.beginI2C() == false) 
  {
    Log.info("The sensor did not respond. Please check wiring.");
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }*/
}

void loop(void) {
  //TODO: can collect return data from getDust() later
  getDust();
  delay(1000);
}