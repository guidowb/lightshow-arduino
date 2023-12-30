#include <Arduino.h>
#include <FastLED.h>

#include <LogManager.h>
#include <PowerManager.h>
#include <LEDStringManager.h>
#include <WiFiManager.h>
#include <ClockManager.h>
#include <ConnectionManager.h>
#include <UpdateManager.h>
#include <Variables.h>

WiFiManager wifiManager;
ClockManager clockManager;
ConnectionManager connectionManager(&wifiManager);
PowerManager powerManager;
LEDStringManager ledStringManager(&powerManager, &clockManager, &connectionManager);
LogManager logManager(&connectionManager);
UpdateManager updateManager(&wifiManager);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  powerManager.setup();
  ledStringManager.setup();
  wifiManager.setup();
  clockManager.setup();
  updateManager.setup();
  connectionManager.subscribe("lightshow/pattern", &ledStringManager);
  connectionManager.setup();

  ledStringManager.setPattern(PATTERN);
}

void loop() {
  // put your main code here, to run repeatedly:
  wifiManager.loop();
  clockManager.loop();
  connectionManager.loop();
  powerManager.loop();
  ledStringManager.loop();
  updateManager.loop();
}
