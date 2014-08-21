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
  Idle,
  Button1Pressed,
  InButton1Pressed,
  Button2Pressed,
  InButton2Pressed,
  AllButtonsPressed,
} State;

State state_machine;
unsigned long old_time;
unsigned long new_time;
unsigned long wait_time;
int led_state;

const unsigned long debounce_delay = 50;
const unsigned long change_time = 100;
const unsigned long stop_time = 500;

void IdleFunc(int button1, int button2)
{
  if (button1 == HIGH && button2 == HIGH) {
    state_machine = AllButtonsPressed;
  }
  else if (button1 == HIGH) {
    state_machine = Button1Pressed;
  }
  else if (button2 == HIGH) {
    state_machine = Button2Pressed;
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

void VerifyAllButtonsPressed(int button1, int button2)
{
  if (IsDebounce()) {
    return;
  }

  if (button1 == HIGH && button2 == HIGH) {
    while(1);
  }
  else {
    state_machine = Idle;
  }
}

void VerifyButton1Pressed(int button1, int button2)
{
  if (IsDebounce()) {
    return;
  }

  if (button1 == HIGH && button2 == LOW && state_machine != InButton1Pressed) {
    state_machine = InButton1Pressed;
    wait_time -= change_time;
  }
  else if (button1 == LOW && button2 == LOW) {
    state_machine = Idle;
  }
  else if (button1 == LOW && button2 == HIGH) {
    state_machine = Button2Pressed;
  }
  else  if (button1 == HIGH && button2 == HIGH && new_time - old_time <= stop_time) {
    state_machine = AllButtonsPressed;
  }
}

void VerifyButton2Pressed(int button1, int button2)
{
  if (IsDebounce()) {
    return;
  }

  if (button2 == HIGH && button1 == LOW && state_machine != InButton2Pressed) {
    state_machine = InButton2Pressed;
    wait_time += change_time;
  }
  else if (button2 == LOW && button1 == LOW) {
    state_machine = Idle;
  }
  else if (button2 == LOW && button1 == HIGH) {
    state_machine = Button1Pressed;
  }
  else if (button2 == HIGH && button1 == HIGH && new_time - old_time <= stop_time) {
    state_machine = AllButtonsPressed;
  }
}

void setup()
{
  state_machine = Idle;
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
    case Idle:
      IdleFunc(button1, button2);
    break;

    case Button1Pressed:
    case InButton1Pressed:
      VerifyButton1Pressed(button1, button2);
    break;

    case Button2Pressed:
    case InButton2Pressed:
      VerifyButton2Pressed(button1, button2);
    break;

    case AllButtonsPressed:
      VerifyAllButtonsPressed(button1, button2);
    break;

    default:
      state_machine = Idle;
    break;
  }
}