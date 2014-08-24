#ifndef EVENT_DRIVEN_H
#define EVENT_DRIVEN_H

#define TIMER_TOTAL 2

class EventDriven
{
  public:
    void ButtonListenCb(void(*bt_cb)(int, int));
    void ButtonListen (int pin);
    void TimerSetCb(void(*timer_cb)(int));
    void TimerSet(int id, unsigned long ms);

    void Setup(void(*init_cb)());
    void Loop();

  private:
    int m_pin;
    int m_bt;
    void (*m_bt_cb) (int, int);

    unsigned long m_old_time[TIMER_TOTAL];
    unsigned long m_new_time[TIMER_TOTAL];
    unsigned long m_timer[TIMER_TOTAL];
    void (*m_timer_cb) (int);
};

#endif