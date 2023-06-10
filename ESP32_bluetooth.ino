#include <BluetoothSerial.h>
#include <SPI.h>
#include <Wire.h>

#define ECG_INPUT     36
#define ECG_LO_MIN    40
#define ECG_LO_PLUS   41
#define BT_TX         7
#define BT_RX         6

BluetoothSerial SerialBT;

void setup() {
  pinMode(ECG_INPUT, INPUT);
  SerialBT.begin("ESP32test");
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {

  int ecgRead = analogRead(ECG_INPUT);

  Serial.println(ecgRead);
  SerialBT.println(ecgRead);

  delay(1);
}
