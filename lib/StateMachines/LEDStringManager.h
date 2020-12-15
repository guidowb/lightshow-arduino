#ifndef _LEDSTRING_MANAGER_H_
#define _LEDSTRING_MANAGER_H_

#include "ConnectionManager.h"
#include "PowerManager.h"
#include "ClockManager.h"
#include "LightShow.h"

class LEDStringManager{
public:
  LEDStringManager(PowerManager *power, ClockManager *clock, ConnectionManager *connection);

  void setup();
  void loop();
  void setPattern(const char *pattern);

private:
  PowerManager *power;
  ClockManager *clock;
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