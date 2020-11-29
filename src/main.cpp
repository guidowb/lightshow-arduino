#include <Arduino.h>
#include <FastLED.h>

extern void initializePower();
extern void powerOn();
extern void powerOff();

extern void initializeString();
extern void updateString();
extern void setPattern(const char *pattern);
extern void solid(CRGB color);
extern void dots(CRGB color, int interval, int offset = 0);

/**************************************************************************************/
/* Colors                                                                             */
/**************************************************************************************/

CRGB black     = CRGB(0x00000000);
CRGB darkgrey  = CRGB(0x00202020);
CRGB halloween = CRGB(0x00FF3F00);
CRGB red       = CRGB(0x00DD0000);
CRGB darkred   = CRGB(0x00C00000);
CRGB purple    = CRGB(0x00400090);
CRGB limegreen = CRGB(0x0080FF00);
CRGB warm      = CRGB(0x00DD7722);
CRGB office    = CRGB(0x00DD6611);

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
#ifdef MEGA
  // dots(warm, 8);
  // dots(red, 8, 4);
  setPattern("solid #000001\ntwinkle #fff");
#else
  setPattern("solid #d61");
  // solid(office);
#endif
  powerOn();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateString();
}