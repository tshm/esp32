#include "Arduino.h"

/** 内蔵LEDの操作 */
class LED
{
public:
  static void setup()
  {
    pinMode(LED_BUILTIN, OUTPUT);
  }

  static void turnOff()
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  static void turnOn()
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
};
