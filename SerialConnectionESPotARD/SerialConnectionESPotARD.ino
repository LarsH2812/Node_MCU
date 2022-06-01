byte practicumByte;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial1.begin(115200);
  a
}

void loop() {
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    byte inByte = Serial1.read();
    practicumByte = inByte;
    Serial.println(inByte, BIN);
  }

  if (Serial.available() > 0) {
    byte sending = Serial.readStringUntil('\n').toInt();
    Serial1.write(sending);
    Serial.println(sending, BIN);
  }
}
