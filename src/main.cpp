#include <Arduino.h>
#include <FastLED.h>

#include <PowerManager.h>
#include <LEDStringManager.h>
#include <WiFiManager.h>
#include <ConnectionManager.h>
#include <CommandHandler.h>
#include <Patterns.h>

WiFiManager wifiManager;
ConnectionManager connectionManager("https://api.lightshow.guidowb.online/connect", PROGRAM);
PowerManager powerManager;
LEDStringManager ledStringManager(&powerManager, &connectionManager);
CommandHandler commandHandler(&ledStringManager);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  powerManager.setup();
  ledStringManager.setup();
  wifiManager.setup();
  connectionManager.add(&commandHandler);
  connectionManager.setup();

  ledStringManager.setPattern(PATTERN);
}

void loop() {
  // put your main code here, to run repeatedly:
  wifiManager.loop();
  connectionManager.loop();
  powerManager.loop();
  ledStringManager.loop();
}