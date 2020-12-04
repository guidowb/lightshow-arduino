#include "LEDStringManager.h"

#include <FastLED.h>

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

class LEDString : public Canvas {
public:
  LEDString();
  virtual int getSize();
  virtual long getTime();
  virtual void setPixel(int pixel, RGBA color);

private:
  CRGB leds[NUM_LEDS];
};

static LEDString canvas;

#define LED(i) leds[ ((i) < NUM_LEDS_LEFT) ? (NUM_LEDS_LEFT - (i) - 1) : (i) ]

LEDString::LEDString() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LEFT >(leds, 0, NUM_LEDS_LEFT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RIGHT>(leds, NUM_LEDS_LEFT, NUM_LEDS_RIGHT);
}

int LEDString::getSize() {
  return NUM_LEDS;
}

long LEDString::getTime() {
  return millis();
}

void LEDString::setPixel(int pixel, RGBA color) {
  int r = (color >> 24) & 0x0ff;
  int g = (color >> 16) & 0x0ff;
  int b = (color >>  8) & 0x0ff;
  int a = (color      ) & 0x0ff;
  r = ((r * a) + (LED(pixel).r * (255 - a))) / 255;
  g = ((g * a) + (LED(pixel).g * (255 - a))) / 255;
  b = ((b * a) + (LED(pixel).b * (255 - a))) / 255;
  LED(pixel) = CRGB(r, g, b);
}

LEDStringManager::LEDStringManager(PowerManager *power) {
  this->power = power;
  this->renderer = NULL;
  this->string = &canvas;
}

void LEDStringManager::setup() {
  resetStats();
}

void LEDStringManager::resetStats() {
  if (this->frames != 0) sendStats(true);
  this->millisStarted = millis();
  this->frames = 0;
  this->millisInRender = 0;
  this->millisInUpdate = 0;
}

void LEDStringManager::sendStats(bool final) {
}

void LEDStringManager::loop() {
  if (renderer) {
    long startTime = millis();
    renderer->render(string);
    millisInRender += millis() - startTime;
  }
  if (power->hasPower()) {
    long startTime = millis();
    FastLED.show();
    millisInUpdate += millis() - startTime;
    frames++;
  }
  else {
    resetStats();
    digitalWrite(DATA_PIN_RIGHT, LOW);
    digitalWrite(DATA_PIN_LEFT, LOW);
  }
}

void LEDStringManager::setPattern(const char *pattern) {
  renderer = compile("arduino", pattern);
  resetStats();
}
