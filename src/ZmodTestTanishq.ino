//#include <avr/ioavr128db48.h>

#define RXD2 PIN_PF5
#define TXD2 PIN_PF4

#define Serial Serial3

#define USART_RXMODE0_bm  (1<<1)  /* Receiver Mode bit 0 mask. */


char returnFromMemset[16];
char* zmod_response = returnFromMemset;

// function prototypes
void InterrogateSensor(void);
bool PowerUpSensor(void);
void ReadAQI(void);

void setup() {
  // for printf output
  Serial.begin(115200);

  // for USART comms with the sensor
  Serial2.pins(TXD2, RXD2);
  Serial2.begin(9600);//, SERIAL_8N1);//, RXD2, TXD2);

  Serial.println("AVR-IoT Sensor Test");

    // keep sending Power Up command until it returns true ("S")
    if(Serial2)  {
    Serial.print("Power Up Sensor: ");
    while (!PowerUpSensor())  {
      Serial.println(PowerUpSensor());
    }
    Serial.println(PowerUpSensor());
    }
}

void loop() {

    InterrogateSensor();
    delay(500);

}



void InterrogateSensor(void)  {
  // Issue ID command and read response
    Serial2.print ("ID\n");
    memset (zmod_response, 0, sizeof(zmod_response));
    Serial2.readBytesUntil('\n', zmod_response, 16);
    String ID = String(zmod_response);
    Serial.println ("ZMOD4510 module ID is 0x" + ID);
}

bool PowerUpSensor(void)  {
    Serial2.print ("PU\n");
    memset (zmod_response, 0, sizeof(zmod_response));
    Serial2.readBytesUntil('\n', zmod_response, 16);
    String ID = String(zmod_response);
    Serial.println ("Powerup return" + ID);

    if (ID == "S") return true;
    return false;

}


