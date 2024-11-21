#include <Wire.h>

// Define trig and echo pins for ultrasonic sensors
const int trigPins[] = {6, 8, 10, 12, 4, 2};  // FL, FR, SL, SR, BL, BR
const int echoPins[] = {7, 9, 11, 13, 5, 3};   // FL, FR, SL, SR, BL, BR

// Array to store sensor distances
long distances[6];

// Function to measure distance from a single sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // Convert time to distance in cm
}

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  Wire.begin();  // Initialize I2C as Master
  Serial.begin(9600); // Debugging
}

void loop() {
  // Measure distances from each sensor
  for (int i = 0; i < 6; i++) {
    distances[i] = measureDistance(trigPins[i], echoPins[i]);
  }

  // Send distances to Motor Arduino (Slave)
  Wire.beginTransmission(8); // Address of the Motor Arduino
  for (int i = 0; i < 6; i++) {
    Wire.write((distances[i] >> 8) & 0xFF); // Send high byte
    Wire.write(distances[i] & 0xFF);        // Send low byte
  }
  byte result = Wire.endTransmission(); // End I2C communication

  // Debug: Print transmission status
  // if (result == 0) {
  //   Serial.println("Data sent successfully.");
  // } else {
  //   Serial.print("Error sending data: ");
  //   Serial.println(result);
  // }

  //Debug: Print distances
  // Serial.print("Distances: ");
  // for (int i = 0; i < 6; i++) {
  //   Serial.print(distances[i]);
  //   if (i < 5) Serial.print(", ");
  // }
  // Serial.println();

  delay(100);  // Small delay
}
