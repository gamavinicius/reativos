#include "event_driven.h"

#define LED_PIN 13
#define BUT_PIN 2

EventDriven m_eventdrive;

void button_changed (int pin, int v)
{
  if (pin == BUT_PIN)
    digitalWrite(LED_PIN, v);
}

void init_client ()
{
  pinMode(LED_PIN, OUTPUT);

  m_eventdrive.ButtonListenCb(&button_changed);
  m_eventdrive.ButtonListen(BUT_PIN);
}

void setup ()
{
  m_eventdrive.Setup(&init_client);
}
void loop ()
{
  m_eventdrive.Loop();
}