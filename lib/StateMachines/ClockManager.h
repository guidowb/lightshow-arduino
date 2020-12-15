#ifndef _CLOCK_MANAGER_H_
#define _CLOCK_MANAGER_H_

#include <Arduino.h>

class ClockManager {
public:
    void setup();
    void loop();

public:
    bool updated;
    uint32_t getEpochTime();
};

#endif
