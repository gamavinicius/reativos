#include "event_driven.h"

#include <Arduino.h>

EventDriven::EventDriven()
  : m_pin_count(0)
{
}

EventDriven::~EventDriven()
{
}

void EventDriven::ButtonListenCb(void(*bt_cb)(int, int))
{
  m_bt_cb = bt_cb;
}

bool EventDriven::ButtonListen(int pin)
{
  m_pin_count++;
  if (m_pin_count >= BUTTON_TOTAL)
    return false;

  int pos = m_pin_count - 1;

  m_pin[pos] = pin;
  pinMode(m_pin[pos], INPUT);

  m_state[pos] = digitalRead(m_pin[pos]);
  return true;
}

void EventDriven::TimerSetCb(void(*timer_cb)(int))
{
  m_timer_cb = timer_cb;
}

bool EventDriven::TimerSet(int id, unsigned long ms, bool reset)
{
  if (id >= TIMER_TOTAL)
    return false;

  if (reset) {
    m_old_time[id] = millis();
    m_new_time[id] = millis();
  }

  m_timer[id] = ms;
  return true;
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
  for (int i=0; i<m_pin_count; i++) {
    int state = digitalRead(m_pin[i]);
    if (m_state[i] != state) {
      m_state[i] = state;
      m_bt_cb(m_pin[i], m_state[i]);
    }
  }

  for (int i=0; i<TIMER_TOTAL; i++) {
    m_new_time[i] = millis();
    if (m_timer[i] != 0 && m_new_time[i] - m_old_time[i] >= m_timer[i]) {
      m_old_time[i] = m_new_time[i];
      m_timer_cb(i);
    }
  }
}