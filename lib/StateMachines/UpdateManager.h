#ifndef _UPDATE_MANAGER_H_
#define _UPDATE_MANAGER_H_

#include "StateMachine.h"
#include "WiFiManager.h"

class UpdateManager : private StateMachine {
public:
    UpdateManager(WiFiManager *wifiManager);

    void setup();
    void loop();

    void enable();

private:
    enum State { DISABLED, ENABLED, UPDATING };
    WiFiManager *wifiManager;
};

#endif
