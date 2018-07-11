#include <Arduino.h>

const int ledPin = 13;

void setup();
void loop();

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}

int main(int argc, char const *argv[]) {
  setup();
  while(true) {
    loop();
  }
  return 0;
}
