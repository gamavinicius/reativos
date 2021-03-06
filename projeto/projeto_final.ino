#include <SoftwareSerial.h>
#include <Stepper.h>

#define BTH_TX 13
#define BTH_RX 12
#define PIN1_STEPPER 8
#define PIN2_STEPPER 9
#define PIN3_STEPPER 10
#define PIN4_STEPPER 11
#define STEP 1
#define TURNSTEP 90
#define RPM 30

Stepper stepper(TURNSTEP, PIN1_STEPPER, PIN2_STEPPER, PIN3_STEPPER, PIN4_STEPPER); 

SoftwareSerial bth_serial(BTH_RX, BTH_TX);
String bth_message = "";
int rotate = 0;

void setup() {
  Serial.begin(9600);
  bth_serial.begin(9600);
  stepper.setSpeed(RPM);
}

void loop() {
  if(bth_serial.available()) {
    while (bth_serial.available())
      bth_message += (char)bth_serial.read();

    switch (bth_message[0]) {
      case '1':
        rotate++;
        break;
      case '2':
        rotate--;
        break;
      case '3':
      default:
        rotate = 0;
        break;
    }
    Serial.print(rotate);
    Serial.print("Bluetooth Message: ");
    Serial.println(bth_message);
    bth_message = "";
  }

  int signal = 1;
  if (rotate < 0)
    signal = -1;
  stepper.setSpeed(signal * rotate * RPM);
  if (rotate != 0 )
    stepper.step(signal * STEP); //blocking fuction
}