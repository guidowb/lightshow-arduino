#ifndef _WIFI_MANAGER_H_
#define _WIFI_MANAGER_H

class WiFiManager {
public:
    void setup();
    void loop();

private:
    bool connected;
    long lastConnect;
    
    void connect();
};

#endif
