#include <Arduino.h>
#include <FastLED.h>

extern void initializePower();
extern void powerOn();
extern void powerOff();

extern void initializeString();
extern void updateString();
extern void solid(CRGB color);
extern void dots(CRGB color, int interval, int offset = 0);

/**************************************************************************************/
/* Colors                                                                             */
/**************************************************************************************/

CRGB black     = CRGB(0x00000000);
CRGB darkgrey  = CRGB(0x00202020);
CRGB halloween = CRGB(0x00FF3F00);
CRGB darkred   = CRGB(0x00C00000);
CRGB purple    = CRGB(0x00400090);
CRGB limegreen = CRGB(0x0080FF00);
CRGB warm      = CRGB(0x00FF9922);

/**************************************************************************************/
/* Main                                                                               */
/**************************************************************************************/

void setup() {
  // put your setup code here, to run once:
  initializePower();
  initializeString();
  // solid(black);
  // solid(warm);
  // dots(limegreen, 4, 2);
  // dots(halloween, 4);
  // dots(darkred, 8);
  // dots(purple, 8, 4);
  dots(warm, 4);
  powerOn();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateString();
}