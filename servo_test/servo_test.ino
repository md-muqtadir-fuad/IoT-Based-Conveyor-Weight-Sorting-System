#include <Servo.h>

#define SERVO_PIN 2  // GPIO2 on ESP-01S

Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
}

void loop() {
  // Sweep from 0 to 180 and back
  for (int pos = 0; pos <= 180; pos += 1) {
    myServo.write(pos);
    delay(10);
  }
  for (int pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);
    delay(10);
  }
}
