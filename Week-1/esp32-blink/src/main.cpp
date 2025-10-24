#include <Arduino.h>

#define Led_BuiltIn 2

const bool Active_Low = false;

void setup()
{
    pinMode(Led_BuiltIn,OUTPUT);
    digitalWrite(Led_BuiltIn,Active_Low?HIGH:LOW);   

}

void loop()
{
  static bool on = false;
  on = !on;
  digitalWrite(Led_BuiltIn, (on ^ Active_Low) ? HIGH : LOW);
  delay(500); 
  
}
