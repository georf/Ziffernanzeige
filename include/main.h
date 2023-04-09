#include <Arduino.h>
#include <StopWatch.h>
#include <Button.h>
#include <WifiControl.h>
#include <EEPROM.h>

#define SR_OE D4
#define pinBtn D3
#define pinWifi D2

#define empty "     "

enum Modus
{
    BOOTING,
    WAITING,
    SHOWING,
    RUNNING
};

void handleDisplay();

void stopWatchStart();
void stopWatchStop();
void waiting();
void waitingShow();
void showingCallback(uint32_t lastDeciTime, uint8_t lastLine);

void btnCallback();
void btnCallbackLong();