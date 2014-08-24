#include "event_driven.h"

#include <Arduino.h>

EventDriven::EventDriven()
{
  m_pin_total = 1;
  m_pin_count = 0;
  m_pin = (int *) malloc(m_pin_total * sizeof(int));
  m_state = (int *) malloc(m_pin_total * sizeof(int));

  m_timer_total = 1;
  m_old_time = (unsigned long *) malloc(m_timer_total * sizeof(unsigned long));
  m_new_time = (unsigned long *) malloc(m_timer_total * sizeof(unsigned long));
  m_timer = (unsigned long *) malloc(m_timer_total * sizeof(unsigned long));
}

EventDriven::~EventDriven()
{
  free(m_pin);
  free(m_state);
  free(m_old_time);
  free(m_new_time);
  free(m_timer);
}

void EventDriven::ButtonListenCb(void(*bt_cb)(int, int))
{
  m_bt_cb = bt_cb;
}

void EventDriven::ButtonListen(int pin)
{
  m_pin_count++;
  if (m_pin_count >= m_pin_total) {
    m_pin_total *= 2;
    m_pin = (int *) realloc(m_pin, m_pin_total * sizeof(int));
    m_state = (int *) realloc(m_state, m_pin_total * sizeof(int));
  }
  int pos = m_pin_count - 1;

  m_pin[pos] = pin;
  pinMode(m_pin[pos], INPUT);

  m_state[pos] = digitalRead(m_pin[pos]);
}

void EventDriven::TimerSetCb(void(*timer_cb)(int))
{
  m_timer_cb = timer_cb;
}

void EventDriven::TimerSet(int id, unsigned long ms, bool reset)
{
  if (id >= m_timer_total) {
    m_timer_total *= 2;
    m_old_time = (unsigned long *) realloc(m_old_time, m_timer_total * sizeof(unsigned long));
    m_new_time = (unsigned long *) realloc(m_new_time, m_timer_total * sizeof(unsigned long));
    m_timer = (unsigned long *) realloc(m_timer, m_timer_total * sizeof(unsigned long));
    for (int i=(m_timer_total/2); i<m_timer_total; i++) {
      m_old_time[i] = millis();
      m_new_time[i] = millis();
      m_timer[i] = 0;
    }
  }

  if (reset) {
    m_old_time[id] = millis();
    m_new_time[id] = millis();
  }
  m_timer[id] = ms;
}

void EventDriven::Setup(void(*init_cb)())
{
  for (int i=0; i<m_timer_total; i++) {
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

  for (int i=0; i<m_timer_total; i++) {
    m_new_time[i] = millis();
    if (m_timer[i] != 0 && m_new_time[i] - m_old_time[i] >= m_timer[i]) {
      m_old_time[i] = m_new_time[i];
      m_timer_cb(i);
    }
  }
}