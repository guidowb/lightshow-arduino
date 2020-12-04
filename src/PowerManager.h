#ifndef _POWER_MANAGER_H_
#define _POWER_MANAGER_H_

#include "ConnectionManager.h"

class PowerManager {
public:
    void setup();
    void loop();

    bool hasPower();
    void powerOn();
    void powerOff();

private:
    bool powered;
};

#endif
