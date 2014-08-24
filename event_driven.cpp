#include "event_driven.h"

#include <Arduino.h>

void EventDriven::ButtonListenCb(void(*bt_cb)(int, int))
{
  m_bt_cb = bt_cb;
}

void EventDriven::ButtonListen(int pin)
{
  m_pin = pin;
  pinMode(m_pin, INPUT);

  m_bt = digitalRead(m_pin);
}

void EventDriven::TimerSetCb(void(*timer_cb)(int))
{
  m_timer_cb = timer_cb;
}

void EventDriven::TimerSet(int id, unsigned long ms)
{
  m_timer[id] = ms;
}

void EventDriven::Setup(void(*init_cb)())
{
  for (int i=0; i<TIMER_TOTAL; i++) {
    m_old_time[i] = millis();
    m_new_time[i] = millis();
    m_timer[i] = 0;
  }

  init_cb();
}

void EventDriven::Loop()
{
  int bt = digitalRead(m_pin);
  if (m_bt != bt) {
    m_bt = bt;
    m_bt_cb(m_pin, m_bt);
  }

  for (int i=0; i<TIMER_TOTAL; i++) {
    m_new_time[i] = millis();
    if (m_timer[i] != 0 && m_new_time[i] - m_old_time[i] >= m_timer[i]) {
      m_old_time[i] = m_new_time[i];
      m_timer_cb(i);
    }
  }
}