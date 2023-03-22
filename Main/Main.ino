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

// DISCLAIMER CODE WONKY AGAIN
// in a file called low_power.cpp located at Arduino\libraries\AVR-IoT-Cellular\src
// there is this function
// ISR(RTC_PIT_vect) {
//     RTC.PITINTFLAGS = RTC_PI_bm;
//     pit_triggered   = true;
// }
// comment it out, or else you will get an error saying "multiple definitions of: __vector_6"
// it's because there are multiple definition for an interrupt and getting rid of one of them
// makes the code work. awesome :)

// RTC code used from DxCore documentation
void RTC_init(void)
{
  /* Initialize RTC: */
  while (RTC.STATUS > 0)
  {
    ;                                   /* Wait for all register to be synchronized */
  }
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    /* 32.768kHz Internal Ultra-Low-Power Oscillator (OSCULP32K) */

  RTC.PITINTCTRL = RTC_PI_bm;           /* PIT Interrupt: enabled */

  RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc /* RTC Clock Cycles 16384, resulting in 32.768kHz/32768 = 1Hz = 1 sec.*/
  | RTC_PITEN_bm;                       /* Enable PIT counter: enabled */
}

ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;          /* Clear interrupt flag by writing '1' (required) */
}

void setup(void) {
  LedCtrl.begin();
  LedCtrl.startupCycle();
  LedCtrl.on(Led::USER);
  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW); //iled default closed
  Serial3.begin(9600);

  RTC_init(); // enable RTC

  // set type of sleep mode, this is the one that uses the minimum amount of power
  // Only module that will be on is the RTC, everything else is off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable(); // enables the sleep mode functionality

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

#define TIME_TO_SLEEP_IN_SECS 10 // edit value to how long you want board to sleep, currently at 10 secs
void loop(void) {
  powerUpZMOD();
  algoOpt();
  getDust();
  printValues();
  Serial3.println(getAQI());
  Serial3.println(getNO2());
  Serial3.println(getO3());
  powerDownZMOD();
  //delay(1000);

  // put board to sleep, shuts down every module except RTC and PIT module
  Serial3.println("I sleep now");
  delay(100);
  for(int i = 0; i < TIME_TO_SLEEP_IN_SECS; i++) // sleep for the time according to TIME_TO_SLEEP_IN_SECS
  {
    sleep_cpu();
  }
  Serial3.println("I awake");
}