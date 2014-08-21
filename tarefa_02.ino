/**
  * Piscar o LED a cada 1 segundo
  * Botão 1: Acelerar o piscapisca cada pressionamento
  * Botão 2: Desacelerar a cada pressionamento
  * Botão 1+2 (em menos de 500ms): Parar
*/

#define LED_PIN 13
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

typedef enum {
  State0,
  State1,
  InState1,
  State2,
  InState2,
  State3,
} State;

State state_machine;
unsigned long old_time;
unsigned long new_time;
unsigned long wait_time;
unsigned long debounce_delay = 50; 
int led_state;

void WaitingInputs(int button1, int button2)
{
  if (button1 == HIGH && button2 == HIGH) {
    state_machine = State3;
  }
  else if (button1 == HIGH) {
    state_machine = State1;
  }
  else if (button2 == HIGH) {
    state_machine = State2;
  }

  if (new_time >= old_time + wait_time) {
    old_time = new_time;
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
  }
}

int IsDebounce()
{
  return (new_time - debounce_delay <= old_time);
}

void VerifyState3(int button1, int button2)
{
  if (IsDebounce()) {
    return;
  }

  if (button1 == HIGH && button2 == HIGH) {
    while(1);
  }
  else {
    state_machine = State0;
  }
}

void VerifyState1(int button1, int button2)
{
  if (IsDebounce()) {
    return;
  }

  if (button1 == HIGH && button2 == LOW && state_machine != InState1) {
    state_machine = InState1;
    wait_time -= 100;
  }
  else if (button1 == LOW && button2 == LOW) {
    state_machine = State0;
  }
  else if (button1 == LOW && button2 == HIGH) {
    state_machine = State2;
  }
  else  if (button1 == HIGH && button2 == HIGH && new_time - old_time <= 500) {
    state_machine = State3;
  }
}

void VerifyState2(int button1, int button2)
{
  if (IsDebounce()) {
    return;
  }

  if (button2 == HIGH && button1 == LOW && state_machine != InState2) {
    state_machine = InState2;
    wait_time += 100;
  }
  else if (button2 == LOW && button1 == LOW) {
    state_machine = State0;
  }
  else if (button2 == LOW && button1 == HIGH) {
    state_machine = State1;
  }
  else if (button2 == HIGH && button1 == HIGH && new_time - old_time <= 500) {
    state_machine = State3;
  }
}

void setup()
{
  state_machine = State0;
  old_time = millis();
  wait_time = 1000;
  led_state = HIGH;

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
}

void loop()
{
  new_time = millis();
  int button1 = digitalRead(BUTTON1_PIN);
  int button2 = digitalRead(BUTTON2_PIN);

  switch(state_machine) {
    case State0:
      WaitingInputs(button1, button2);
    break;

    case State1:
    case InState1:
      VerifyState1(button1, button2);
    break;

    case State2:
    case InState2:
      VerifyState2(button1, button2);
    break;

    case State3:
      VerifyState3(button1, button2);
    break;

    default:
      state_machine = State0;
    break;
  }
}