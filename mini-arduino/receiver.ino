/* This is the receiver module
 * Must be installed in the parking entrance.
 *
 * Can control more than just one entrance.
 */

#include <VirtualWire.h>
#include <NewPing.h>

#define RX_RF_PIN 2
#define TRIGGER_SONAR_PIN 9
#define ECHO_SONAR_PIN 8

#define SONAR_NUM 1
#define MAX_DISTANCE 200
#define PING_INTERVAL 33

#define CAR_DISTANCE 25
#define PARKING_LIMIT 5

unsigned long ping_timer[SONAR_NUM];
unsigned int sensor[SONAR_NUM];
bool already_checked[SONAR_NUM];
uint8_t current_sensor = 0;

unsigned int car_total = 0;

NewPing sonar[SONAR_NUM] = {
  NewPing(TRIGGER_SONAR_PIN, ECHO_SONAR_PIN, MAX_DISTANCE)
};

void echoCheck() {
  if (sonar[current_sensor].check_timer()) {
    sensor[current_sensor] = sonar[current_sensor].ping_result / US_ROUNDTRIP_CM;
  }
}

void oneSensorCycle() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    //Serial.println(sensor[i]);
    if (sensor[i] < CAR_DISTANCE && sensor[i] != 0) {
      if (car_total == PARKING_LIMIT) {
        Serial.println("Proibido de entrar");
      }
      else if (!already_checked[i]) {
        Serial.println("Permitido entrar");
        car_total = car_total + 1;
        already_checked[i] = true;
        Serial.println("Carro entrou");
      }
    }
    else {
      already_checked[i] = false;
    }
  }
}

void setup ()
{
  //Serial
  Serial.begin(9600);

  //Sonar
  ping_timer[0] = millis() + 75;
  for (uint8_t i = 1; i < SONAR_NUM; i++) {
    ping_timer[i] = ping_timer[i - 1] + PING_INTERVAL;
    already_checked[i] = false;
  }

  //RX
  vw_set_rx_pin(RX_RF_PIN);
  vw_setup(2000);
  vw_rx_start();
}

void loop ()
{
  //Sonar
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (millis() >= ping_timer[i]) {
      ping_timer[i] += PING_INTERVAL * SONAR_NUM;
      if (i == 0 && current_sensor == SONAR_NUM - 1)
        oneSensorCycle();
      sonar[current_sensor].timer_stop();
      current_sensor = i;
      sonar[current_sensor].ping_timer(echoCheck);
    }
  }

  //RX
  uint8_t message[VW_MAX_MESSAGE_LEN];
  uint8_t msgLength = VW_MAX_MESSAGE_LEN;

  if (vw_wait_rx_max(200)) { //Blocking until 200ms
    if (vw_get_message(message, &msgLength)) // Non-blocking
    {
      if (msgLength == 3 &&
            message[0] == 'O' &&
            message[1] == 'u' &&
            message[2] == 't') { //Not good ...
        if (car_total > 0 ) {
          car_total = car_total - 1;
          Serial.println("Carro saiu");
        }
        else {
          Serial.println("Algum problema ocorreu");
        }
      }
    }
  }
}