#include "HX711.h" // Include the HX711 library for load cell amplifier
#include <Servo.h>   // Include the Servo library for controlling servo motors

// --- HX711 Load Cell Amplifier Pin Definitions ---
// Connect HX711 DT (Data) pin to ESP-01S GPIO0
const int LOADCELL_DOUT_PIN = 0; 
// Connect HX711 SCK (Clock) pin to ESP-01S GPIO3 (RX pin)
// Be aware that using GPIO3 (RX) might interfere with serial communication if you're
// actively sending data *to* the ESP-01S from your computer while the program runs.
// For debugging output *from* the ESP-01S, it's generally fine.
const int LOADCELL_SCK_PIN = 3;  

// --- SG90 Servo Motor Pin Definition ---
// Connect SG90 Signal (Orange/Yellow) wire to ESP-01S GPIO2
// This is a good choice for servo control as it's a stable GPIO.
const int SERVO_PIN = 2;         

// --- Weight Thresholds for Servo Control ---
// If weight is above this, or below LOWER_THRESHOLD_KG, servo goes to ALERT_SERVO_ANGLE
const float UPPER_THRESHOLD_KG = 0.2; 
// If weight is below this, or above UPPER_THRESHOLD_KG, servo goes to ALERT_SERVO_ANGLE
const float LOWER_THRESHOLD_KG = 0.1; 

// --- Servo Angle Definitions ---
// Servo angle when weight is within the normal range (0.1kg to 0.2kg)
const int NORMAL_SERVO_ANGLE = 0;   
// Servo angle when weight is outside the normal range (<0.1kg or >0.2kg)
const int ALERT_SERVO_ANGLE = 60;   

// --- Load Cell Calibration Factor ---
// IMPORTANT: This value needs to be determined through proper calibration of your
// specific load cell and HX711 module.
// A negative value might be needed depending on how your load cell is wired.
// Example: If 1kg placed on the scale results in a raw reading change of -22000,
// then the scale factor would be -22000 / 1000g = -22.00.
const float CALIBRATION_FACTOR = -22.00; 

// Create instances of the HX711 and Servo objects
HX711 scale;
Servo myServo; 

void setup() {
  // Initialize serial communication for debugging and monitoring
  Serial.begin(115200); 
  Serial.println("--- ESP-01S Load Cell & Servo Control ---");
  Serial.println("Initializing HX711 and Servo...");

  // Initialize the HX711 module
  // Pass the data (DOUT) and clock (SCK) pin numbers
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Set the calibration factor for the load cell
  // This converts raw readings into meaningful units (e.g., grams or kilograms)
  scale.set_scale(CALIBRATION_FACTOR); 
  
  // Tare the scale: sets the current reading as the zero point
  // Ensure nothing is on the load cell when the ESP-01S powers up for accurate taring.
  scale.tare();            

  // Attach the servo object to the specified servo pin
  myServo.attach(SERVO_PIN); 
  // Set the initial servo position to the normal angle
  myServo.write(NORMAL_SERVO_ANGLE); 

  Serial.println("System initialized.");
  Serial.print("Load cell DOUT: GPIO"); Serial.println(LOADCELL_DOUT_PIN);
  Serial.print("Load cell SCK: GPIO"); Serial.println(LOADCELL_SCK_PIN);
  Serial.print("Servo signal: GPIO"); Serial.println(SERVO_PIN);
  Serial.print("Calibration factor: "); Serial.println(CALIBRATION_FACTOR, 2);
  Serial.println("Place known weight for calibration if readings are off.");
}

void loop() {
  // Check if the HX711 is ready to provide a new reading
  if (scale.is_ready()) {
    // Get the current weight reading from the load cell
    // get_units(10) takes an average of 10 readings for stability
    // The result is in kilograms based on the CALIBRATION_FACTOR
    float currentWeightKg = scale.get_units(10); 

    // Optional: Add a small dead zone around zero to prevent fluctuating readings
    // from showing small negative or positive values when nothing is on the scale.
    if (currentWeightKg > -0.005 && currentWeightKg < 0.005) { 
      currentWeightKg = 0.0; // Treat very small readings as zero
    }

    // Print the current weight to the Serial Monitor for debugging
    Serial.print("Weight: ");
    Serial.print(currentWeightKg, 3); // Print with 3 decimal places
    Serial.println(" kg");

    // --- Control Logic for Servo based on Weight ---
    // Check if the weight is outside the desired range (less than 0.1kg OR greater than 0.2kg)
    if (currentWeightKg > UPPER_THRESHOLD_KG || currentWeightKg < LOWER_THRESHOLD_KG) {
      // If outside range, move servo to the alert angle
      myServo.write(ALERT_SERVO_ANGLE);
      Serial.print("Servo State: ALERT ("); Serial.print(ALERT_SERVO_ANGLE); Serial.println(" degrees)");
    } else {
      // If within the normal range, move servo to the normal angle
      myServo.write(NORMAL_SERVO_ANGLE);
      Serial.print("Servo State: NORMAL ("); Serial.print(NORMAL_SERVO_ANGLE); Serial.println(" degrees)");
    }

  } else {
    // If HX711 is not found or not ready, print an error message
    Serial.println("HX711 not found or not ready.");
  }

  // Small delay to prevent constantly reading and writing, which can consume power
  // and make serial output unreadable. Adjust as needed.
  delay(100); 
}
