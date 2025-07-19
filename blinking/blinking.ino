void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Hello, ESP-01S!");
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}
