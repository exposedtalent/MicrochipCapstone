#define SIZE 8
char returnFromMemset[SIZE]; // biggest return value from zmod should be xxxx.x\n
char* zmod_response = returnFromMemset;

void initZmod() {
  // for USART comms with the sensor
  Serial2.pins(TXD2, RXD2);
  Serial2.begin(9600);
}

void powerUpZMOD() {
  if(Serial2)  {
    Serial3.println("Powering Up ZMOD");
    memset (zmod_response, 0, SIZE);
    String ID = "";
      while (ID != "S")  {
        Serial2.print ("PU\n");
        Serial2.readBytesUntil('\n', zmod_response, SIZE);
        ID = String(zmod_response);
      }
    Serial3.println("Powered Up\n");
  }
}

void powerDownZMOD() {
  if(Serial2)  {
    Serial3.println("Powering Down ZMOD");
    memset (zmod_response, 0, SIZE);
    String ID = "";
      while (ID != "S")  {
        Serial2.print ("PD\n");
        Serial2.readBytesUntil('\n', zmod_response, SIZE);
        ID = String(zmod_response);
      }
    Serial3.println("Powered Down\n");
  }
}

String getAQI() {
  if(Serial2)  {
    Serial3.println("Getting AQI");
    memset (zmod_response, 0, SIZE);
    String AQI = "F";
      while (AQI == "F")  {
        Serial2.print ("AQ\n");
        Serial2.readBytesUntil('\n', zmod_response, SIZE);
        AQI = String(zmod_response);
      }
    Serial3.println("Returning AQI");
    return AQI;
  }
  return "Serial 2 not running";
}

void algoOpt() {
  if(Serial2)  {
    Serial3.println("Doing ZMOD Optimizations");
    memset (zmod_response, 0, SIZE);
    String ID = "";
      // will return S\n if succesful
      while (ID != "S")  {
        // current auto optimiztion OPx x = A for auto C O for O3 and N for NO2
        Serial2.print ("OPA\n");
        Serial2.readBytesUntil('\n', zmod_response, SIZE);
        ID = String(zmod_response);
      }
    Serial3.println("Optimization Complete");
  }
}

// TODO: currently string bc i dont want to do conversion rn
String getNO2() {
  if(Serial2)  {
    Serial3.println("Getting NO2");
    memset (zmod_response, 0, SIZE);
    String NO2 = "F";
      // will return F\n if failure
      while (NO2 == "F")  {
        Serial2.print ("NO\n");
        Serial2.readBytesUntil('\n', zmod_response, SIZE);
        NO2 = String(zmod_response);
      }
    Serial3.println("Returning NO2");
    return NO2;
  }
  return "Serial 2 not running";
}

// TODO: currently string bc i dont want to do conversion rn
String getO3() {
  if(Serial2)  {
    Serial3.println("Getting O3");
    memset (zmod_response, 0, SIZE);
    String O3 = "F";
      // will return F\n if failure
      while (O3 == "F")  {
        Serial2.print ("O3\n");
        Serial2.readBytesUntil('\n', zmod_response, SIZE);
        O3 = String(zmod_response);
      }
    Serial3.println("Returning O3");
    return O3;
  }
  return "Serial 2 not running";
}