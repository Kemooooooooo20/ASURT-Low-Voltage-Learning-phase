// Pins for potentiometer and UART communication
const int potPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read potentiometer value
  int potValue = analogRead(potPin);
  int mappedValue = map(potValue, 0, 1023, 0, 180); // Map potentiometer value to servo angle range (0-180)
//>>2,/4
  // Send potentiometer value via UART to slave
  Serial.write(mappedValue);

  delay(100);
}