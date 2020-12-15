#ifndef _LEDSTRING_MANAGER_H_
#define _LEDSTRING_MANAGER_H_

#include "ConnectionManager.h"
#include "PowerManager.h"
#include "LightShow.h"

class LEDStringManager{
public:
  LEDStringManager(PowerManager *power, ConnectionManager *connection);

  void setup();
  void loop();
  void setPattern(const char *pattern);

private:
  PowerManager *power;
  ConnectionManager *connection;
  Renderer *renderer;
  long millisStarted;
  long millisInRender;
  long millisInUpdate;
  long frames;
  long lastStats;
  long lastLit;

  void resetStats();
  void sendStats(bool final = false);
  bool isLit();
};

#endif