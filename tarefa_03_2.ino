#include "event_driven.h"

#define LED_PIN 13
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

int button1;
int button2;
int led_state;

EventDriven eventdrive;

unsigned long wait_time;
const unsigned long debounce_delay = 50;
const unsigned long change_time = 100;

void timer_expired (int id)
{
  switch(id) {
    case 0: //Piscar LED
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
    break;

    case 1: //Debounce
      eventdrive.TimerSet(1, 0, true);
      VerifyButtons();
      eventdrive.TimerSet(0, wait_time, false);
    break;

    default:
    break;
  }
}

void button_changed (int pin, int v)
{
  switch(pin) {
    case BUTTON1_PIN:
        button1 = v;
        eventdrive.TimerSet(1, debounce_delay, true);
    break;

    case BUTTON2_PIN:
        button2 = v;
        eventdrive.TimerSet(1, debounce_delay, true);
    break;

    default:
    break;
  }
}

void init_client()
{
  wait_time = 1000;
  led_state = HIGH;

  pinMode(LED_PIN, OUTPUT);

  eventdrive.ButtonListenCb(&button_changed);
  eventdrive.ButtonListen(BUTTON1_PIN);
  eventdrive.ButtonListen(BUTTON2_PIN);

  eventdrive.TimerSetCb(&timer_expired);
  eventdrive.TimerSet(0, wait_time, false);
}

void VerifyAllButtonsPressed()
{
  if (button1 == HIGH && button2 == HIGH) {
    while(1);
  }
}

void VerifyButton1Pressed()
{
  if (button1 == HIGH && button2 == LOW) {
    wait_time -= change_time;
  }
}

void VerifyButton2Pressed()
{
  if (button2 == HIGH && button1 == LOW) {
    wait_time += change_time;
  }
}

void VerifyButtons()
{
  VerifyAllButtonsPressed();
  VerifyButton1Pressed();
  VerifyButton2Pressed();
}

void setup()
{
  eventdrive.Setup(&init_client);
}

void loop()
{
  eventdrive.Loop();
}