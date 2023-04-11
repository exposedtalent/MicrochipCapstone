void initRTC() {
  while (!rtc.begin(&Wire1)) {
    Serial3.println("RTC Not Connected");
    delay(100);
  }

  if (!rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // Set the RTC to the date & time this sketch was compiled
    // NOTE: since there isnt a battery in the RTC we will need to recompile once we put the batteries in to keep an accurate time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

uint32_t getTime() {
  DateTime now = rtc.now();
  return now.unixtime();
}

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

  RTC.PITCTRLA = RTC_PERIOD_CYC16384_gc /* RTC Clock Cycles 16384, resulting in 32.768kHz/16384 = 2Hz = 0.5 sec.*/
  | RTC_PITEN_bm;                       /* Enable PIT counter: enabled */
}

ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;          /* Clear interrupt flag by writing '1' (required) */
}