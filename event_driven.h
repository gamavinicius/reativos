#ifndef EVENT_DRIVEN_H
#define EVENT_DRIVEN_H

#define TIMER_TOTAL 2

class EventDriven
{
  public:
    EventDriven();
    ~EventDriven();
    void ButtonListenCb(void(*bt_cb)(int, int));
    void ButtonListen (int pin);
    void TimerSetCb(void(*timer_cb)(int));
    void TimerSet(int id, unsigned long ms, bool reset);

    void Setup(void(*init_cb)());
    void Loop();

  private:
    int *m_pin;
    int *m_state;
    int m_pin_total;
    int m_pin_count;
    void (*m_bt_cb) (int, int);

    unsigned long *m_old_time;
    unsigned long *m_new_time;
    unsigned long *m_timer;
    int m_timer_total;
    int m_timer_count;
    void (*m_timer_cb) (int);
};

#endif