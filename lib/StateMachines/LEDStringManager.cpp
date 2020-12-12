#include "LEDStringManager.h"

#include <FastLED.h>

#ifdef FRONT_STRINGS
#define NUM_LEDS_PER_STRING 150
#define NUM_STRINGS_LEFT      4
#define NUM_STRINGS_RIGHT     3
#define NUM_BUFFER_LEDS       1
#else
#define NUM_LEDS_PER_STRING 201
#define NUM_STRINGS_LEFT      0
#define NUM_STRINGS_RIGHT     1
#define NUM_BUFFER_LEDS       0
#endif

#ifdef MEGA
#define DATA_PIN_LEFT  7
#define DATA_PIN_RIGHT 6
#else
#define DATA_PIN_LEFT  D6
#define DATA_PIN_RIGHT D5
#endif

#define NUM_LEDS_LEFT  (NUM_STRINGS_LEFT  * NUM_LEDS_PER_STRING)
#define NUM_LEDS_RIGHT (NUM_STRINGS_RIGHT * NUM_LEDS_PER_STRING)
#define NUM_LEDS       (NUM_LEDS_LEFT + NUM_LEDS_RIGHT)
#define SIZE_LEDS      (NUM_LEDS + NUM_BUFFER_LEDS * 2)

class LEDString : public Canvas {
public:
  LEDString();
  virtual int getSize();
  virtual long getTime();
  virtual void setPixel(int pixel, RGBA color);

private:
  friend class LEDStringManager;
  CRGB leds[SIZE_LEDS];
};

static LEDString canvas;

// Without buffer:
// LED(0)    -> 600 -   0 - 1 -> 599   The fartest to the left
// LED(599)  -> 600 - 599 - 1 ->   0   The nearest to the left
// LED(600)  -> 600                    The nearest to the right
// LED(1049) -> 1049                   The fartest to the right

// With buffer:
// LED(0)    -> 601 -   0 - 1 ->  600   The fartest visible to the left
// LED(599)  -> 601 - 599 - 1 ->    1   The nearest visible to the left
// LED(600)  -> 600       + 2 ->  602   The nearest visible to the right
// LED(1049) -> 1049      + 2 -> 1051   The fartest visible to the right
// Left  buffer ->   0
// Right buffer -> 601

#define LED(i) leds[ ((i) < NUM_LEDS_LEFT) ? (NUM_LEDS_LEFT + NUM_BUFFER_LEDS - (i) - 1) : ((i) + NUM_BUFFER_LEDS * 2) ]

LEDString::LEDString() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LEFT >(leds, 0, NUM_LEDS_LEFT + NUM_BUFFER_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RIGHT>(leds, NUM_LEDS_LEFT + NUM_BUFFER_LEDS, NUM_LEDS_RIGHT + NUM_BUFFER_LEDS);
  if (NUM_BUFFER_LEDS > 0) {
    if (NUM_LEDS_LEFT  > 0) leds[0]                 = CRGB(0);
    if (NUM_LEDS_RIGHT > 0) leds[NUM_LEDS_LEFT + 1] = CRGB(0);
  }
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

LEDStringManager::LEDStringManager(PowerManager *power, ConnectionManager *connection) {
  this->power = power;
  this->connection = connection;
  this->renderer = NULL;
  this->string = &canvas;
}

void LEDStringManager::setup() {
  resetStats();
}

bool LEDStringManager::isLit() {
  for (int p = 0; p < NUM_LEDS; p++) {
    if (canvas.LED(p) != CRGB(0)) return true;
  }
  return false;
}

void LEDStringManager::resetStats() {
  if (this->frames != 0) sendStats(true);
  this->millisStarted = millis();
  this->frames = 0;
  this->millisInRender = 0;
  this->millisInUpdate = 0;
}

void LEDStringManager::sendStats(bool final) {
  if (!connection) return;
  long millisCurrent = millis();
  long millisTotal = millisCurrent - millisStarted;
  if (millisTotal == 0) millisTotal = 1; // Avoid division by zero
  int fps = (frames * 1000) / millisTotal;
  int render = (millisInRender * 100) / millisTotal;
  int update = (millisInUpdate * 100) / millisTotal;
  connection->send("stats%s fps=%d, render%%=%d, update%%=%d", final ? " (final)" : "", fps, render, update);
}

void LEDStringManager::loop() {

  // Render
  if (renderer) {
    long startTime = millis();
    renderer->render(string);
    millisInRender += millis() - startTime;
  }

  // Manage power based on lit LEDs
  if (isLit()) power->isNeeded();

  // Update the string
  if (power->isPowered()) {
    long startTime = millis();
    FastLED.show();
    millisInUpdate += millis() - startTime;
    frames++;
  }
  else {
    resetStats();
    digitalWrite(DATA_PIN_RIGHT, LOW);
    digitalWrite(DATA_PIN_LEFT, LOW);
    delay(100);
  }

  // Update server stats
  long currentTime = millis();
  if (currentTime - lastStats >= 30000) {
    sendStats();
    lastStats = currentTime;
  }
}

void LEDStringManager::setPattern(const char *pattern) {
  renderer = compile("arduino", pattern);
  resetStats();
}
