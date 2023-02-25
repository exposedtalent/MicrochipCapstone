#include <Arduino.h>
#include <Wire.h>
#include <String.h>
#include <log.h>

void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
  Log.begin(115200);
  Log.setLogLevel(LogLevel::INFO);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial2.write("ID\n");

  String reply = Serial2.readString();
  char* replyChar = new char[reply.length() + 1];
  strcpy(replyChar, reply.c_str());
  Log.infof("%s\n", replyChar);
}

