
// Serial3 is used for debug log printing
// Serial2 is used for ZMOD4510 sensor interface
#include <Wire.h>
#include <avr/io.h>

#define BAUDRATE 9600
#define UART_TX_PIN PIN_PB11
#define UART_RX_PIN PIN_PB10
// Set up WiFi and Serial3 ports
void setup()
{
  Serial3.begin (9600);
// Serial1.begin(0, SERIAL_8N1, -1, -1, true, 11000UL)
  // Set up UART for ZMOD4510 module, 9600-8-N-1
  Serial2.begin(9600);
// Serial2.begin(BAUDRATE, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);    
  Serial2.setTimeout(1000);

}

void loop()
{
  char zmod_response[16];
  String ID, AQI, NO2, O3, AQIjson;

  // Loop until module responds to power up command
  while (1)
  {
    // Issue PU command and read response
    Serial2.print ("PU\n");
    memset (zmod_response, 0, sizeof(zmod_response));
    Serial2.readBytesUntil('\n', zmod_response, 16);
    Serial3.println("The response:" + zmod_response[0]);
    if (zmod_response[0] != 'S')
    {
      // Could not get desired response to PU command
      Serial3.println ("Trying to initialize ZMOD4510 module...");
      delay (1000);
      continue;
    }
    else
    {
      // All good!
      Serial3.println ("ZMOD4510 is powered up!");
      break;
    }
  }

  // Issue ID command and read response
  Serial2.print ("ID\n");
  memset (zmod_response, 0, sizeof(zmod_response));
  Serial2.readBytesUntil('\n', zmod_response, 16);
  ID = zmod_response;
  Serial3.println ("ZMOD4510 module ID is 0x" + ID);

  // Get data from module
  while (1)
  {
    // Issue AQ command and read AQI value
    Serial2.print ("AQ\n");
    memset (zmod_response, 0, sizeof(zmod_response));
    Serial2.readBytesUntil('\n', zmod_response, 16);
    Serial3.printf ("Debug AQI $: %s\n", zmod_response);
    AQI = zmod_response;

    // Issue NO command and read NO2 value
    Serial2.print ("NO\n");
    memset (zmod_response, 0, sizeof(zmod_response));
    Serial2.readBytesUntil('\n', zmod_response, 16);
    Serial3.printf ("Debug NO $: %s\n", zmod_response);
    NO2 = zmod_response;

    // Issue O3 command and read O3 value
    Serial2.print ("O3\n");
    memset (zmod_response, 0, sizeof(zmod_response));
    Serial2.readBytesUntil('\n', zmod_response, 16);
    Serial3.printf ("Debug O3 $: %s\n", zmod_response);
    O3 = zmod_response;

    // {"id": "AABBCCDD", "aqi": 10.0, "no2": 20.0, "o3": 30.0}
    AQIjson = "{\"id\": " + ID + ", \"aqi\": " + AQI + ", \"no2\": " + NO2 + ", \"o3\": " + O3 + "}";
    Serial3.println ("JSON: \n" + AQIjson);

    delay (1000);  // 10 * 60 seconds between updates
  }
}
