/**************************************************************************************/
/* LED String handling                                                                */
/**************************************************************************************/

#include <FastLED.h>

#ifdef MEGA
#define NUM_LEDS_PER_STRING 150
#define NUM_STRINGS_LEFT      4
#define NUM_STRINGS_RIGHT     3
#define DATA_PIN_LEFT  7
#define DATA_PIN_RIGHT 6
#else
#define NUM_LEDS_PER_STRING 300
#define NUM_STRINGS_LEFT      1
#define NUM_STRINGS_RIGHT     0
#define DATA_PIN_LEFT  D5
#define DATA_PIN_RIGHT D6
#endif

#define NUM_LEDS_LEFT  (NUM_STRINGS_LEFT  * NUM_LEDS_PER_STRING)
#define NUM_LEDS_RIGHT (NUM_STRINGS_RIGHT * NUM_LEDS_PER_STRING)
#define NUM_LEDS       (NUM_LEDS_LEFT + NUM_LEDS_RIGHT)

extern bool checkPower();

CRGB leds[NUM_LEDS];

#define LED(i) leds[ ((i) < NUM_LEDS_LEFT) ? (NUM_LEDS_LEFT - (i) - 1) : (i) ]

void initializeString() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LEFT >(leds, 0, NUM_LEDS_LEFT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RIGHT>(leds, NUM_LEDS_LEFT, NUM_LEDS_RIGHT);
}

void updateString() {
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
