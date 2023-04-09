#include "Arduino.h"

class StopWatch
{
public:
    void start();
    boolean hasUpdate();
    void output(char *output);
    static void centiSecondsToChars(uint32_t centiSeconds, char *output);
    static byte cleanLeadingZeros(char *output);

private:
    uint16_t time;
    unsigned long lastCentiSecond;
};