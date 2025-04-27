#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
      int incomingByte = Serial.read();
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
  }

  if (mySerial.available() > 0) {
      int receivedByte = mySerial.read();
      Serial.print("Received from SoftwareSerial: ");
      Serial.println(receivedByte, DEC);
  }

  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 1000) {
      lastSendTime = millis();
      mySerial.println("Hello, world");
  }
}
