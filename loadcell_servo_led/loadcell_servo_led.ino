#include "HX711.h"    // HX711 library for load cell amplifier
#include <Servo.h>    // Servo library for controlling servo motors

// --- Pin Definitions ---
const int LOADCELL_DOUT_PIN = 0;   // HX711 DT to ESP-01S GPIO0
const int LOADCELL_SCK_PIN  = 3;   // HX711 SCK to ESP-01S GPIO3 (RX)
const int SERVO_PIN         = 2;   // SG90 signal to ESP-01S GPIO2

// --- Weight Thresholds ---
const float UPPER_THRESHOLD_KG = 0.2;
const float LOWER_THRESHOLD_KG = 0.1;

// --- Servo Angles ---
const int NORMAL_SERVO_ANGLE = 0;
const int ALERT_SERVO_ANGLE  = 60;

// --- Calibration ---
const float CALIBRATION_FACTOR = -22.00;

HX711 scale;
Servo myServo;

// Blink the onboard LED for the specified duration (in milliseconds)
void blinkOnboardLED(int duration_ms) {
  unsigned long start = millis();
  while (millis() - start < duration_ms) {
    digitalWrite(LED_BUILTIN, LOW);   // ON (ESP-01S LED is active LOW)
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);  // OFF
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("--- ESP-01S Load Cell & Servo Control ---");
  Serial.println("Initializing HX711 and Servo...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // OFF initially

  blinkOnboardLED(5000);  // Blink onboard LED for 5 seconds on power up

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare();

  myServo.attach(SERVO_PIN);
  myServo.write(NORMAL_SERVO_ANGLE);

  Serial.println("System initialized.");
  Serial.print("Load cell DOUT: GPIO"); Serial.println(LOADCELL_DOUT_PIN);
  Serial.print("Load cell SCK: GPIO"); Serial.println(LOADCELL_SCK_PIN);
  Serial.print("Servo signal: GPIO"); Serial.println(SERVO_PIN);
  Serial.print("Calibration factor: "); Serial.println(CALIBRATION_FACTOR, 2);
  Serial.println("Place known weight for calibration if readings are off.");
}

void loop() {
  if (scale.is_ready()) {
    float currentWeightKg = scale.get_units(10);
    if (currentWeightKg > -0.005 && currentWeightKg < 0.005) {
      currentWeightKg = 0.0;
    }
    Serial.print("Weight: ");
    Serial.print(currentWeightKg, 3);
    Serial.println(" kg");

    if (currentWeightKg > UPPER_THRESHOLD_KG || currentWeightKg < LOWER_THRESHOLD_KG) {
      myServo.write(ALERT_SERVO_ANGLE);
      Serial.print("Servo State: ALERT (");
      Serial.print(ALERT_SERVO_ANGLE);
      Serial.println(" degrees)");
    } else {
      myServo.write(NORMAL_SERVO_ANGLE);
      Serial.print("Servo State: NORMAL (");
      Serial.print(NORMAL_SERVO_ANGLE);
      Serial.println(" degrees)");
    }
  } else {
    Serial.println("HX711 not found or not ready.");
  }
  delay(100);
}
