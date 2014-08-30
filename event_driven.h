#ifndef EVENT_DRIVEN_H
#define EVENT_DRIVEN_H

#define BUTTON_TOTAL 4
#define TIMER_TOTAL 4

class EventDriven
{
  public:
    EventDriven();
    ~EventDriven();
    void ButtonListenCb(void(*bt_cb)(int, int));
    bool ButtonListen (int pin);
    void TimerSetCb(void(*timer_cb)(int));
    bool TimerSet(int id, unsigned long ms, bool reset);

    void Setup(void(*init_cb)());
    void Loop();

  private:
    int m_pin[BUTTON_TOTAL];
    int m_state[BUTTON_TOTAL];
    int m_pin_count;
    void (*m_bt_cb) (int, int);

    unsigned long m_old_time[TIMER_TOTAL];
    unsigned long m_new_time[TIMER_TOTAL];
    unsigned long m_timer[TIMER_TOTAL];
    int m_timer_count;
    void (*m_timer_cb) (int);
};

#endif