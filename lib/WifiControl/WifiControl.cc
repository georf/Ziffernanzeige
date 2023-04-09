#include "WifiControl.h"

void WifiControl::Startup(Display *d, SerialControl *sc, bool channelSwitch)
{
    serialControl = sc;
    display = d;

    if (channelSwitch)
        WiFi.begin(WIFI_SSID_A, WIFI_SSID_A_PASSPHRASE);
    else
        WiFi.begin(WIFI_SSID_B, WIFI_SSID_B_PASSPHRASE);
}

bool WifiControl::handleOrBooting()
{
    if (!connectionPossible)
        return false;

    if (!wifiClient.connected())
    {
        return true;
    }

    if (wifiClient.available())
        serialControl->handle(wifiClient.read());

    return false;
}

bool WifiControl::bootingFinish()
{

    if (connectionPossible || connectionRetries < 100)
    {
        display->setOutput(0, " conn", 0b0000);
        if (WiFi.status() != WL_CONNECTED)
        {
            connectionRetries++;
            if (connectionRetries % 2)
                display->setOutput(1, "    -", 0b0000);
            else
                display->setOutput(1, "   - ", 0b0000);
            delay(100);
        }
        else
        {
            display->setOutput(1, "  yes", 0b0000);
            wifiClient.connect(WIFI_SERVER, WIFI_PORT);
            connectionPossible = true;
            delay(3000);
            return true;
        }
    }
    else
    {
        display->setOutput(1, "   no", 0b0000);
        connectionPossible = false;
        delay(3000);
        return true;
    }

    return false;
}