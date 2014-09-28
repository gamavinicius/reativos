/* This is the transmissor module
 * Must be installed in the exit of parking.
 *
 * Can control more than just one exit.
 */

#include <VirtualWire.h>
#include <NewPing.h>

#define TX_RF_PIN 7
#define TRIGGER_SONAR_PIN 9
#define ECHO_SONAR_PIN 8

#define SONAR_NUM 1
#define MAX_DISTANCE 200
#define PING_INTERVAL 33 // 29ms is the min.

#define CAR_DISTANCE 50

unsigned long ping_timer[SONAR_NUM];
unsigned int sensor[SONAR_NUM];
bool already_send[SONAR_NUM];
uint8_t current_sensor = 0;

NewPing sonar[SONAR_NUM] = {
  NewPing(TRIGGER_SONAR_PIN, ECHO_SONAR_PIN, MAX_DISTANCE)
};

void EchoCheck() {
  if (sonar[current_sensor].check_timer()) {
    sensor[current_sensor] = sonar[current_sensor].ping_result / US_ROUNDTRIP_CM;
  }
}

void CycleComplete() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (sensor[i] < CAR_DISTANCE) {
      if (!already_send[i]) {
        char message[] = "Out";
        send(message);
        already_send[i] = true;
      }
    }
    else {
      already_send[i] = false;
    }
  }
}

void send (const char *message)
{
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();
}

void setup ()
{
  //Serial
  Serial.begin(9600);

  //Sonar
  ping_timer[0] = millis() + 75;
  for (uint8_t i = 1; i < SONAR_NUM; i++) {
    ping_timer[i] = ping_timer[i - 1] + PING_INTERVAL;
    already_send[i] = false;
  }

  //TX
  vw_set_tx_pin(TX_RF_PIN);
  vw_setup(2000);
}

void loop ()
{
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (millis() >= ping_timer[i]) {
      ping_timer[i] += PING_INTERVAL * SONAR_NUM;
      if (i == 0 && current_sensor == SONAR_NUM - 1)
        CycleComplete();
      sonar[current_sensor].timer_stop();
      current_sensor = i;
      sonar[current_sensor].ping_timer(EchoCheck); 
    }
  }

  delay(10);
}