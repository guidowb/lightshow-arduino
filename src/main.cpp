#include <Arduino.h>
#include <FastLED.h>

#include <PowerManager.h>
#include <LEDStringManager.h>
#include <WiFiManager.h>
#include <ConnectionManager.h>
#include <CommandHandler.h>

/**************************************************************************************/
/* Patterns                                                                           */
/**************************************************************************************/

const char *twinkle = "solid #001\ntwinkle #fff 1200";
const char *xmas1 = "dots 4 #00f #a0a #d40 #d00 #0d0";
const char *xmas2 = "dots 4 #d00 #0d0 #d61";
const char *office  = "solid #d61";

/**************************************************************************************/
/* Main                                                                               */
/**************************************************************************************/

PowerManager powerManager;
LEDStringManager ledStringManager(&powerManager);
WiFiManager wifiManager;
ConnectionManager connectionManager("wss://api.lightshow.guidowb.online:443/connect");
CommandHandler commandHandler(&ledStringManager);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Started");
  powerManager.setup();
  ledStringManager.setup();
  wifiManager.setup();
  connectionManager.add(&commandHandler);
  connectionManager.setup();

#ifdef MEGA
  ledStringManager.setPattern(xmas2);
#else
  ledStringManager.setPattern(office);
#endif

  powerManager.powerOn();
}

void loop() {
  // put your main code here, to run repeatedly:
  powerManager.loop();
  ledStringManager.loop();
}