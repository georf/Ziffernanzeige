#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <SerialControl.h>
#include <Display.h>

#define WIFI_SERVER "10.0.112.42"
#define WIFI_PORT 11242

#define WIFI_SSID_A "Ziffernanzeige-A"
#define WIFI_SSID_A_PASSPHRASE "usi3taQuoh6mahyaiSha"
#define WIFI_SSID_B "Ziffernanzeige-B"
#define WIFI_SSID_B_PASSPHRASE "yai2inohch4OhP1du5iu"

class WifiControl
{
public:
    bool handleOrBooting();
    void Startup( Display *display, SerialControl *serialControl,bool channelSwitch);
    bool bootingFinish();

private:
    SerialControl *serialControl;
    Display *display;
    WiFiClient wifiClient;
    uint8_t connectionRetries = 0;
    bool connectionPossible = false;
};
