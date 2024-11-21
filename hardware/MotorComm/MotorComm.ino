#include <Wire.h>

// Motor Pin Definitions
const int motorIN1[] = {13, 8, 2, 4};  // IN1 pins for motors
const int motorIN2[] = {7, 9, 3, 12};  // IN2 pins for motors
const int motorENA[] = {10, 11, 6, 5}; // ENA (PWM) pins for motors

// Array to store received distances
long distances[6];
bool dataReceived = false;  // Flag to indicate new data received

void setup() {
  // Initialize I2C communication
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register receive event
  Serial.begin(9600); // Debugging

  // Initialize motor pins
  for (int i = 0; i < 4; i++) {
    pinMode(motorIN1[i], OUTPUT);
    pinMode(motorIN2[i], OUTPUT);
    pinMode(motorENA[i], OUTPUT);
  }

  Serial.println("Motor Arduino ready. Waiting for data...");
}

// Function to receive data from Sensor Arduino
void receiveEvent(int bytes) {
  int index = 0;

  // Debugging: Print the number of bytes received
  // Serial.print("Bytes received: ");
  // Serial.println(bytes);

  // Read and parse distances
  while (Wire.available() >= 2 && index < 6) { // Ensure there are enough bytes for a complete value
    int highByte = Wire.read(); // High byte
    int lowByte = Wire.read();  // Low byte
    distances[index] = (highByte << 8) | lowByte; // Combine bytes
    index++;
  }

  // Debug: Print the parsed distances
  // Serial.print("Parsed Distances: ");
  // for (int i = 0; i < 6; i++) {
  //   Serial.print(distances[i]);
  //   if (i < 5) Serial.print(", ");
  // }
  // Serial.println();

  // Set flag to indicate new data is available
  dataReceived = true;
}

// Motor Control Functions
void controlMotor(int motorIndex, int speed, int dir) {
  digitalWrite(motorIN1[motorIndex], dir == 1);       // IN1
  digitalWrite(motorIN2[motorIndex], dir == -1);      // IN2
  analogWrite(motorENA[motorIndex], speed);           // Set speed
}

void stopMotors() {
  for (int i = 0; i < 4; i++) 
    controlMotor(i, 0, 0);
}

void moveMotors(int speed, int dir1, int dir2, int dir3, int dir4) {
  controlMotor(0, speed, dir1);       // Back Right
  controlMotor(1, speed, dir2);       // Back Left
  controlMotor(2, speed + 5, dir3);   // Front Right (adjusted for resistance)
  controlMotor(3, speed, dir4);       // Front Left
}

void moveForward(int speed)   { moveMotors(speed, 1, 1, 1, 1); }
void moveBackward(int speed)  { moveMotors(speed, -1, -1, -1, -1); }
void moveLeft(int speed)      { moveMotors(speed, -1, 1, 1, -1); }
void moveRight(int speed)     { moveMotors(speed, 1, -1, -1, 1); }

// Navigation Logic
void navigate() {
  static int stuckCounter = 0; // Counter to track if the robot is stuck
  
  if (distances[0] < 30 || distances[1] < 30) {  // Front sensors (FL, FR)
    Serial.println("Obstacle ahead! Moving backward.");
    moveBackward(100);
    delay(500);
    stopMotors();
    delay(500);

    // Turn left or right to avoid obstacle
    if (distances[2] > distances[3]) { // More space on the left
      Serial.println("Turning left to avoid obstacle.");
      moveLeft(100);
      delay(500);
    } else { // More space on the right
      Serial.println("Turning right to avoid obstacle.");
      moveRight(100);
      delay(500);
    }
    stopMotors();
    delay(500);

    stuckCounter++; // Increment stuck counter
  } 
  else if (distances[2] < 30) {                // Side Left (SL)
    Serial.println("Obstacle on left! Turning right.");
    moveRight(100);
    delay(500);
    stopMotors();
    delay(500);
    stuckCounter = 0; // Reset stuck counter
  } 
  else if (distances[3] < 30) {                // Side Right (SR)
    Serial.println("Obstacle on right! Turning left.");
    moveLeft(100);
    delay(500);
    stopMotors();
    delay(500);
    stuckCounter = 0; // Reset stuck counter
  } 
  else if (distances[4] < 30 || distances[5] < 30) {  // Back sensors (BL, BR)
    Serial.println("Obstacle behind! Moving forward.");
    moveForward(100);
    delay(500);
    stopMotors();
    delay(500);
    stuckCounter = 0; // Reset stuck counter
  } 
  else {
    Serial.println("Path is clear. Moving forward.");
    moveForward(100);
    stuckCounter = 0; // Reset stuck counter
  }

  // If the robot is stuck (repeated obstacle detection), make a full turn
  if (stuckCounter >= 3) {
    Serial.println("Robot appears to be stuck. Performing a full turn.");
    moveRight(100); // Perform a 360-degree turn or partial turn
    delay(1000);    // Adjust delay for desired turn angle
    stopMotors();
    delay(500);
    stuckCounter = 0; // Reset stuck counter after turning
  }
}

void loop() {
  // Check if new data has been received
  if (dataReceived) {
    dataReceived = false; // Reset flag
    navigate();  // Call navigation logic
  } else {
    // If no data, indicate waiting
    Serial.println("Waiting for data...");
    delay(100);
  }
}
