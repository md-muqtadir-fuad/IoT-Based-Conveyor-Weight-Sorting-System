#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 0; // ESP-01S GPIO0
const int LOADCELL_SCK_PIN = 2;  // ESP-01S GPIO2

HX711 scale;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing HX711...");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Set the scale factor (calibration value)
  // This value needs to be determined through calibration
  // Example: If 1kg reads 20000 counts, then scale_factor = 20000 / 1000 (grams) = 20
  scale.set_scale(-22.00); // Adjust this value based on your calibration
  scale.tare(); // Reset the scale to zero

  Serial.println("Scale initialized. Place known weight for calibration.");
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();
    float grams = scale.get_units(10); // Get average of 10 readings in grams
    float kilograms = grams / 1000.0;

    Serial.print("Raw reading: ");
    Serial.print(reading);
    Serial.print("\tWeight: ");
    Serial.print(kilograms, 2); // Print with 2 decimal places
    Serial.println(" kg");
  } else {
    Serial.println("HX711 not found.");
  }

  delay(100);
}