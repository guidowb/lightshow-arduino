#include <Arduino.h>
#include <FastLED.h>

#include <PowerManager.h>
#include <LEDStringManager.h>
#include <WiFiManager.h>
#include <ConnectionManager.h>
#include <CommandHandler.h>
#include <Patterns.h>

WiFiManager wifiManager;
ConnectionManager connectionManager("https://api.lightshow.guidowb.online/connect");
PowerManager powerManager;
LEDStringManager ledStringManager(&powerManager, NULL);
CommandHandler commandHandler(&ledStringManager);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Started");
  Serial.printf("Free Memory: %d\n", ESP.getFreeHeap());

  powerManager.setup();
  ledStringManager.setup();
  wifiManager.setup();
  connectionManager.add(&commandHandler);
  connectionManager.setup();

  ledStringManager.setPattern(PATTERN);
  powerManager.powerOn();
}

void loop() {
  // put your main code here, to run repeatedly:
  wifiManager.loop();
  connectionManager.loop();
  powerManager.loop();
  ledStringManager.loop();
}