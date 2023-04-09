#include "Arduino.h"

class SerialControl
{
public:
    void handle();
    SerialControl(void (*pCallbackStartF)(), void (*pCallbackWaitF)(), void (*pCallbackShowF)(uint32_t lastDeciTime, uint8_t lastLine));
    void Startup();

private:
    boolean stoppable[2] = {true, true};
    byte currentLine[50];
    uint8_t currentLineIndex = 0;
    uint32_t recentReceivedTime[4] = {0, 0, 0, 0};
    uint32_t lastDeciTime = 0;
    void (*pCallbackStart)();
    void (*pCallbackWait)();
    void (*pCallbackShow)(uint32_t lastDeciTime, uint8_t lastLine);
    void HandleTeamComputerLong(uint8_t start);
    void HandleShortCommands();
    void HandlePlatz();
    boolean HandlePlatzTime(uint8_t part, byte a, byte b, byte c);
};
