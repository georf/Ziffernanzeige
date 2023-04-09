#include <main.h>

Modus modus = BOOTING;
Modus lines[2] = {BOOTING, BOOTING};
Display display;
StopWatch stopWatch;
Button btn = Button(pinBtn, &btnCallback, &btnCallbackLong);
SerialControl serialControl;
WifiControl wifiControl;

void setup()
{
    EEPROM.begin(512);
    display.init();
    pinMode(D1, INPUT);
    pinMode(pinWifi, INPUT);
    pinMode(SR_OE, OUTPUT);
    serialControl.Startup(&stopWatchStart, &waiting, &showingCallback);

    analogWrite(SR_OE, EEPROM.read(0));

    wifiControl.Startup(&display, &serialControl, digitalRead(pinWifi));
}

void btnCallback()
{
    if (modus == WAITING)
        stopWatchStart();
    else if (modus == RUNNING)
        stopWatchStop();
    else if (modus == SHOWING)
        waiting();
}

void btnCallbackLong()
{
    uint8_t brightness = EEPROM.read(0);
    char output[5];
    brightness += 2;

    stopWatch.centiSecondsToChars(brightness, output);
    output[0] = ' ';
    output[1] = ' ';
    modus = SHOWING;
    display.setOutput(0, "88888", 0b1111);
    display.setOutput(1, output, 0);
    analogWrite(SR_OE, brightness);
    EEPROM.write(0, brightness);
    EEPROM.commit();
}

void loop()
{
    btn.read();
    handleDisplay();
    serialControl.handle();
    if (wifiControl.handleOrBooting())
        modus = BOOTING;
}

void handleDisplay()
{
    char output[5];
    byte points = 0b1111;
    switch (modus)
    {
    case RUNNING:
        if (!stopWatch.hasUpdate())
            return;

        for (uint8_t i = 0; i < 2; i++)
        {
            if (lines[i] == RUNNING)
            {
                stopWatch.output(output);
                output[4] = ' ';
                points = StopWatch::cleanLeadingZeros(output);
                display.setOutput(i, output, points);
                return;
            }
        }
        display.green(false);
        modus = SHOWING;
        break;

    case WAITING:
        waitingShow();
        break;

    case BOOTING:
        if (wifiControl.bootingFinish())
            waiting();
        break;

    case SHOWING:
        break;
    }
}

void showingCallback(uint32_t lastDeciTime, uint8_t lastLine)
{
    char output[5];
    StopWatch::centiSecondsToChars(lastDeciTime, output);
    byte points = StopWatch::cleanLeadingZeros(output);
    display.setOutput(lastLine, output, points);
    lines[lastLine] = SHOWING;
}

void stopWatchStart()
{
    stopWatch.start();
    modus = RUNNING;
    lines[0] = RUNNING;
    lines[1] = RUNNING;
    display.setOutput(0, empty, 0);
    display.setOutput(1, empty, 0);
    display.green(true);
}

void stopWatchStop()
{
    char output[5];
    byte points;
    for (uint8_t i = 0; i < 2; i++)
    {
        if (lines[i] == RUNNING)
        {
            stopWatch.output(output);
            points = StopWatch::cleanLeadingZeros(output);
            display.setOutput(i, output, points);
            lines[i] = SHOWING;
            display.red(true);
            return;
        }
    }
}

void waiting()
{
    modus = WAITING;
    lines[0] = WAITING;
    lines[1] = WAITING;
    waitingShow();
    display.green(false);
    display.red(false);
}

void waitingShow()
{
    if (lines[0] != WAITING && lines[1] != WAITING)
    {
        modus = RUNNING;
        return;
    }

    char spinner[2][9][5] = {
        {
            {' ', ' ', ' ', ' ', '}'},
            {' ', ' ', ' ', ' ', ']'},
            {' ', ' ', ' ', ' ', '~'},
            {' ', ' ', ' ', '~', ' '},
            {' ', ' ', '~', ' ', ' '},
            {' ', '~', ' ', ' ', ' '},
            {'~', ' ', ' ', ' ', ' '},
            {'[', ' ', ' ', ' ', ' '},
            {'{', ' ', ' ', ' ', ' '},
        },
        {
            {'[', ' ', ' ', ' ', ' '},
            {'{', ' ', ' ', ' ', ' '},
            {'_', ' ', ' ', ' ', ' '},
            {' ', '_', ' ', ' ', ' '},
            {' ', ' ', '_', ' ', ' '},
            {' ', ' ', ' ', '_', ' '},
            {' ', ' ', ' ', ' ', '_'},
            {' ', ' ', ' ', ' ', '}'},
            {' ', ' ', ' ', ' ', ']'},
        },
    };
    unsigned long current = millis();
    uint8_t output_line = current / 1000 % 2;

    display.setOutput((output_line + 1) % 2, "     ", 0);
    display.setOutput(output_line, spinner[output_line][current % 1000 / 112], 0);
}