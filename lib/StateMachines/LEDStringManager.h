#ifndef _LEDSTRING_MANAGER_H_
#define _LEDSTRING_MANAGER_H_

#include "ConnectionManager.h"
#include "PowerManager.h"
#include "ClockManager.h"
#include "LightShow.h"

class LEDStringManager : public MessageHandler {
public:
  LEDStringManager(PowerManager *power, ClockManager *clock, ConnectionManager *connection);

  void setup();
  void loop();
  void setPattern(const char *pattern);

  bool handleMessage(const char *message);

private:
  PowerManager *power;
  ClockManager *clock;
  ConnectionManager *connection;
  Renderer *renderer;
  unsigned long millisStarted;
  unsigned long millisInRender;
  unsigned long millisInUpdate;
  unsigned long frames;
  unsigned long lastStats;
  unsigned long lastLit;

  void resetStats();
  void sendStats(bool final = false);
  bool isLit();
};

#endif