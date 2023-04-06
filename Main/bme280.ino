#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

void initBME() {
  unsigned status = bme.begin(0x77, &Wire1);

  if (!status) {
      Serial3.print("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial3.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial3.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial3.print("        ID of 0x60 represents a BME 280.\n");
      Serial3.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }
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

byte getTemp() {
  return static_cast<byte>(bme.readTemperature());
}

int getHumidity() {
  return static_cast<int>(100 * bme.readHumidity());
}