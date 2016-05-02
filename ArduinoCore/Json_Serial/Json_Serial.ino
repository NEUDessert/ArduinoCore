/*
  Sample sketch communicating over Serial using JSON

  This sketch communicates over serial link with a computer, sending
  JSON-encoded state of its analog pints every second and accepting
  per-line JSON messages with desired values of PWM pins on input.

  Circuit:
    (Optional) Analog sensors attached to analog pin.
    (Optional) LEDs attached to PWM pins 9 and 8.

  created 1 November 2012
  by Petr Baudis

  https://github.com/interactive-matter/ajson
  This code is in the public domain.
*/

#include <aJSON.h>
#include "jsonn.h"

unsigned long last_print = 0;
aJsonStream serial_stream(&Serial);

void setup()
{
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
}

void loop()
{
  if (millis() - last_print > 1000) {
    /* One second elapsed, send message. */
    aJsonObject *msg = createMessage();
    Serial.println(aJson.print(msg));
    aJson.deleteItem(msg);
    last_print = millis();
  }

  if (serial_stream.available()) {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }

  if (serial_stream.available()) {
    /* Something real on input, let's take a look. */
    aJsonObject *msg = aJson.parse(&serial_stream);
    processMessage(msg);
    aJson.deleteItem(msg);
  }
}
