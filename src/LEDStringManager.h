#ifndef _LEDSTRING_MANAGER_H_
#define _LEDSTRING_MANAGER_H_

#include "PowerManager.h"
#include "LightShow.h"

class LEDStringManager{
public:
  LEDStringManager(PowerManager *power);

  void setup();
  void loop();
  void setPattern(const char *pattern);

private:
  PowerManager *power;
  Renderer *renderer;
  Canvas *string;
  long millisStarted;
  long millisInRender;
  long millisInUpdate;
  long frames;

  void resetStats();
  void sendStats(bool final = false);
};

#endif