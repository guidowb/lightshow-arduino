#ifndef _POWER_MANAGER_H_
#define _POWER_MANAGER_H_

#include "StateMachine.h"
#include "RateLimiter.h"

class PowerManager : private StateMachine {
public:
    PowerManager();

    void setup();
    void loop();

    bool isPowered();
    void isNeeded();
    void powerOn();
    void powerOff();

private:
    enum State { POWERED_OFF, POWERING_OFF, POWERING_ON, POWERED_ON, POWER_UNMANAGED };
    RateLimiter powerCheck;
    long lastNeeded;

    void checkPowerOn();
    void checkPowerOff();
    void checkPowered();
    void checkNeeded();
};

#endif
