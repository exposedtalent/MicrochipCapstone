#include <Arduino.h>
#include <Wire.h>
#include <String.h>
#include <log.h>

char returnFromMemset[16];
char* zmod_response = returnFromMemset;

void powerUpZMOD() {
  if(Serial2)  {
    Serial3.print("Powering Up ZMOD");
    memset (zmod_response, 0, sizeof(zmod_response));
    String ID = "";
      while (ID != "S")  {
        Serial2.print ("PU\n");
        Serial2.readBytesUntil('\n', zmod_response, 16);
        ID = String(zmod_response);
      }
    Serial3.println("Powered Up");
  }
}

void powerDownZMOD() {
  if(Serial2)  {
    Serial3.println("Powering Down ZMOD");
    memset (zmod_response, 0, sizeof(zmod_response));
    String ID = "";
      while (ID != "S")  {
        Serial2.print ("PD\n");
        Serial2.readBytesUntil('\n', zmod_response, 16);
        ID = String(zmod_response);
      }
    Serial3.println("Powered Down\n");
  }
}

void getAQI() {
  
}