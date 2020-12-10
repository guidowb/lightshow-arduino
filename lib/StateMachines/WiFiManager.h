#ifndef _WIFI_MANAGER_H_
#define _WIFI_MANAGER_H

#include "StateMachine.h"
#include "RateLimiter.h"

class WiFiManager : private StateMachine {
public:
    WiFiManager();

    void setup();
    void loop();

    bool isConnected();

private:
    enum State { WIFI_CONNECTING, WIFI_CONNECTED, WIFI_DISCONNECTED };
    RateLimiter connectionCheck;
    RateLimiter connectionAttempt;
   
    void tryConnecting();
    void checkConnected();
    void checkConnection();
};

#endif
