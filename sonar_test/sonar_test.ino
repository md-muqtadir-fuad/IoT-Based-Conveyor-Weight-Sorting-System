#define TRIG_PIN 0  // ESP-01S GPIO0 (Pin 3)
#define ECHO_PIN 2  // ESP-01S GPIO2 (Pin 4)

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration, distance;

  // Clear the trigPin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Set the trigPin HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echoPin
  duration = pulseIn(ECHO_PIN, HIGH, 25000); // Timeout after 25ms
  distance = (duration / 2) / 29.1; // Calculate distance in cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
