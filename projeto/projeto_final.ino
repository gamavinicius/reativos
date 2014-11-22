#include <SoftwareSerial.h>

#define BTH_TX 13
#define BTH_RX 12

SoftwareSerial bth_serial(BTH_RX, BTH_TX);
String bth_message = "";

void setup() {
  Serial.begin(9600);
  bth_serial.begin(9600);
}

void loop() {
  delay(50);
  if(bth_serial.available()) { 
    while(bth_serial.available())
      bth_message += (char)bth_serial.read();
    Serial.print("Bluetooth Message: ");
    Serial.println(bth_message);
    bth_message = "";
  }
}
