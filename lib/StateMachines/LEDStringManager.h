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

  virtual bool handleMessage(const char *topic, const byte *message, u_int32_t length);

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
