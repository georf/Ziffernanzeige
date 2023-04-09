#include <Button.h>

Button::Button(int PIN, void (*pCallbackFunction)(), void (*pCallbackLongPressed)())
{
    pin = PIN;
    pCallback = pCallbackFunction;
    pCallbackLong = pCallbackLongPressed;
    pinMode(pin, INPUT_PULLUP);
    state = digitalRead(pin);
    pressed = state == LOW;
    lastValue = state;
    lastDebounceTime = 0;
}

void Button::read()
{
    int value = digitalRead(pin);
    if (value != lastValue)
    {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > 10)
    {
        if (value != state)
        {
            state = value;
            pressed = state == LOW;
            if (pressed)
            {
                nextLongPressedTime = millis() + 1000;
                pCallback();
            }
        }
    }
    lastValue = value;
    if (pressed && nextLongPressedTime < millis())
    {
        nextLongPressedTime = millis() + 80;
        pCallbackLong();
    }
}