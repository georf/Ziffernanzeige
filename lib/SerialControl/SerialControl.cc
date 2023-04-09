#include "SerialControl.h"

SerialControl::SerialControl(void (*pCallbackStartF)(), void (*pCallbackWaitF)(), void (*pCallbackShowF)(uint32_t lastDeciTime, uint8_t lastLine))
{
    pCallbackStart = pCallbackStartF;
    pCallbackWait = pCallbackWaitF;
    pCallbackShow = pCallbackShowF;
}

void SerialControl::Startup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
}

void SerialControl::handle()
{
    byte control;
    if (Serial.available())
    {
        control = Serial.read();
        Serial.write(control);

        currentLine[currentLineIndex] = control;
        currentLineIndex++;

        if (currentLineIndex > 49)
        {
            currentLineIndex = 0;
        }
        else if (control == '\r' || control == '\n')
        {
            if (currentLineIndex == 1) {
                currentLineIndex = 0;
                return;
            }

            if (currentLineIndex > 30)
                HandlePlatz();
            else if (currentLineIndex < 19)
                HandleShortCommands();
            else if (currentLine[0] == '#')
                HandleTeamComputerLong(0);
            else if (currentLine[1] == '#')
                HandleTeamComputerLong(1);
            currentLineIndex = 0;
        }
    }
}

void SerialControl::HandlePlatz()
{
    bool stopped;

    stopped = HandlePlatzTime(0, currentLine[8], currentLine[9], currentLine[10]);
    stopped = HandlePlatzTime(1, currentLine[12], currentLine[13], currentLine[14]) && stopped;

    if (stoppable[0] && stopped)
    {
        if (recentReceivedTime[0] < recentReceivedTime[1])
            lastDeciTime = recentReceivedTime[1];
        else
            lastDeciTime = recentReceivedTime[0];
        pCallbackShow(lastDeciTime, 0);
        stoppable[0] = false;

        if (currentLine[5] == 0x02)
        {
            pCallbackShow(lastDeciTime, 1);
            stoppable[1] = false;
            return;
        }
    }

    if (currentLine[5] == 0x04)
    {
        stopped = HandlePlatzTime(2, currentLine[16], currentLine[17], currentLine[18]);
        stopped = HandlePlatzTime(3, currentLine[20], currentLine[21], currentLine[22]) && stopped;
        if (stoppable[1] && stopped)
        {
            if (recentReceivedTime[2] < recentReceivedTime[3])
                lastDeciTime = recentReceivedTime[3];
            else
                lastDeciTime = recentReceivedTime[2];
            pCallbackShow(lastDeciTime, 1);
            stoppable[1] = false;
            return;
        }
    }

    if (recentReceivedTime[0] == 0 && recentReceivedTime[1] == 0 && lastDeciTime != 0)
    {
        lastDeciTime = 0;
        pCallbackWait();
        stoppable[0] = true;
        stoppable[1] = true;
    }
}

boolean SerialControl::HandlePlatzTime(uint8_t part, byte a, byte b, byte c)
{
    uint32_t a_i = (uint8_t)a;
    uint32_t b_i = (uint8_t)b;
    uint32_t c_i = (uint8_t)c;
    uint32_t time = (a_i + b_i * 256 + c_i * 256 * 256) / 10;
    boolean stopped = time != 0 && time == recentReceivedTime[part];
    if (part == 0 && recentReceivedTime[part] == 0 && time > 0)
    {
        pCallbackStart();
        stoppable[0] = true;
        stoppable[1] = true;
    }
    recentReceivedTime[part] = time;
    return stopped;
}

// "1-0:18,03"
// "2-0:22,04"
// "s"
// "w"
void SerialControl::HandleShortCommands()
{
    if (currentLine[0] == 's')
    {
        return pCallbackStart();
    }
    else if (currentLine[0] == 'w')
    {
        return pCallbackWait();
    }
    else if (currentLine[0] == '1' || currentLine[0] == '2')
    {
        lastDeciTime = 0;
        lastDeciTime += (currentLine[2] - 48) * 6000;
        lastDeciTime += (currentLine[4] - 48) * 1000;
        lastDeciTime += (currentLine[5] - 48) * 100;
        lastDeciTime += (currentLine[7] - 48) * 10;
        lastDeciTime += (currentLine[8] - 48);
        return pCallbackShow(lastDeciTime, currentLine[0] - 49);
    }
}

// "#0:08,000|0:09,343*\r\n"
void SerialControl::HandleTeamComputerLong(uint8_t start)
{
    lastDeciTime = 0;
    lastDeciTime += (currentLine[start + 1] - 48) * 6000;
    lastDeciTime += (currentLine[start + 3] - 48) * 1000;
    lastDeciTime += (currentLine[start + 4] - 48) * 100;
    lastDeciTime += (currentLine[start + 6] - 48) * 10;
    lastDeciTime += (currentLine[start + 7] - 48);
    pCallbackShow(lastDeciTime, 0);

    lastDeciTime = 0;
    lastDeciTime += (currentLine[start + 10] - 48) * 6000;
    lastDeciTime += (currentLine[start + 12] - 48) * 1000;
    lastDeciTime += (currentLine[start + 13] - 48) * 100;
    lastDeciTime += (currentLine[start + 15] - 48) * 10;
    lastDeciTime += (currentLine[start + 16] - 48);
    pCallbackShow(lastDeciTime, 1);
}