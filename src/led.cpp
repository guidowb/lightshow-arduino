/**************************************************************************************/
/* LED String handling                                                                */
/**************************************************************************************/

#include <FastLED.h>

#ifdef LIGHTSHOW
#include "LightShow.h"
#endif

#ifdef MEGA
#define NUM_LEDS_PER_STRING 150
#define NUM_STRINGS_LEFT      4
#define NUM_STRINGS_RIGHT     3
#define DATA_PIN_LEFT  7
#define DATA_PIN_RIGHT 6
#else
#define NUM_LEDS_PER_STRING 201
#define NUM_STRINGS_LEFT      0
#define NUM_STRINGS_RIGHT     1
#define DATA_PIN_LEFT  D6
#define DATA_PIN_RIGHT D5
#endif

#define NUM_LEDS_LEFT  (NUM_STRINGS_LEFT  * NUM_LEDS_PER_STRING)
#define NUM_LEDS_RIGHT (NUM_STRINGS_RIGHT * NUM_LEDS_PER_STRING)
#define NUM_LEDS       (NUM_LEDS_LEFT + NUM_LEDS_RIGHT)

extern bool checkPower();

CRGB leds[NUM_LEDS];

#define LED(i) leds[ ((i) < NUM_LEDS_LEFT) ? (NUM_LEDS_LEFT - (i) - 1) : (i) ]

#ifdef LIGHTSHOW
class LEDString : public Canvas {
public:
  virtual int getSize() { return NUM_LEDS; }
  virtual long getTime() { return millis(); }
  virtual void setPixel(int pixel, RGBA color) {
    int r = (color >> 24) & 0x0ff;
    int g = (color >> 16) & 0x0ff;
    int b = (color >>  8) & 0x0ff;
    int a = (color      ) & 0x0ff;
    r = ((r * a) + (LED(pixel).r * (255 - a))) / 255;
    g = ((g * a) + (LED(pixel).g * (255 - a))) / 255;
    b = ((b * a) + (LED(pixel).b * (255 - a))) / 255;
    LED(pixel) = CRGB(r, g, b);
  }
} canvas;

static Renderer *renderer = NULL;

void setPattern(const char *pattern) {
  renderer = compile("arduino", pattern);
}
#endif

void initializeString() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LEFT >(leds, 0, NUM_LEDS_LEFT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RIGHT>(leds, NUM_LEDS_LEFT, NUM_LEDS_RIGHT);
}

void updateString() {
#ifdef LIGHTSHOW
  if (renderer) renderer->render(&canvas);
#endif
  if (checkPower()) {
    FastLED.show();
  }
  else {
    digitalWrite(DATA_PIN_RIGHT, LOW);
    digitalWrite(DATA_PIN_LEFT, LOW);
  }
}

void solid(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) LED(i) = color;
}

void dots(CRGB color, int distance, int offset = 0) {
  for (int i = offset; i < NUM_LEDS; i += distance) LED(i) = color;
}
