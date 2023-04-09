#include <StopWatch.h>

void StopWatch::start()
{
  time = 0;
  lastCentiSecond = millis();
}

boolean StopWatch::hasUpdate()
{
  unsigned long newCentiSecond = millis();
  if (newCentiSecond >= lastCentiSecond + 10)
  {
    lastCentiSecond = newCentiSecond;
    time++;
    return true;
  };
  return false;
}

void StopWatch::output(char *output)
{
  centiSecondsToChars(time, output);
}

void StopWatch::centiSecondsToChars(uint32_t centiSeconds, char *output)
{
  output[0] = 48 + (centiSeconds / 6000);
  output[1] = 48 + ((centiSeconds / 100 % 60) / 10);
  output[2] = 48 + (centiSeconds / 100 % 10);
  output[3] = 48 + (centiSeconds / 10 % 10);
  output[4] = 48 + (centiSeconds % 10);
}

byte StopWatch::cleanLeadingZeros(char *output)
{
  byte points = 0b1111;
  if (output[0] == '0')
  {
    output[0] = ' ';
    points = 0b0011;
    if (output[1] == '0')
    {
      output[1] = ' ';
      if (output[2] == '0')
      {
        output[2] = ' ';
        points = 0;
      }
    }
  }
  return points;
}