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