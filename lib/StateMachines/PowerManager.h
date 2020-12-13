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

    void powerNeeded();
    void powerOn();
    void powerOff();

private:
    enum State { POWERED_OFF, POWERING_OFF, POWERING_ON, POWERED_ON, POWER_UNMANAGED, POWER_LOST };
    RateLimiter powerCheck;
    RateLimiter autoShutoff;

    void enterPoweredOff();
    void whilePoweredOff();
    void enterPoweringOff();
    void whilePoweringOff();
    void enterPoweredOn();
    void whilePoweredOn();
    void enterPoweringOn();
    void whilePoweringOn();
    void enterUnmanaged();
    void whileUnmanaged();
    void enterPowerLost();
    void whilePowerLost();
};

#endif
