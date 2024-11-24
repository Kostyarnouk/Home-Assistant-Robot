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

void stopMotors(int time) {
  for (int i = 0; i < 4; i++)
    controlMotor(i, 0, 0);
  delay(time);
}

void moveMotors(int speed, int dir1, int dir2, int dir3, int dir4, int timeLength) {
  controlMotor(0, speed, dir1);       // Back Right
  controlMotor(1, speed, dir2);       // Back Left
  controlMotor(2, speed + 5, dir3);   // Front Right (adjusted for resistance)
  controlMotor(3, speed, dir4);       // Front Left
  delay(timeLength); //smooth adjustment bewteen actions
}
//FL FR BL BR
void moveForward(int speed,int time)   { moveMotors(speed, -1, -1, -1, -1, time); }
void moveBackward(int speed,int time)  { moveMotors(speed, 1, 1, 1, 1, time); }
void moveLeft(int speed,int time)      { moveMotors(speed, -1, 1, 1, -1, time); }
void moveRight(int speed,int time)     { moveMotors(speed, 1, -1, -1, 1, time); }
void move180(int speed,int time)       { moveMotors(speed, 1, 1, -1, -1, time); }

// Navigation Logic
//Distances[FL,FR,SL,SR,BL,BR]
void navigate() {
  static int stuckCounter = 0; // Counter to track if the robot is stuck
  if (distances[0] < 40 || distances[1] < 40) {  // Front sensors (FL, FR)
    Serial.println("Obstacle ahead! Moving backward.");
    moveBackward(100,100);

    // Turn left or right to avoid obstacle
    if (distances[2] > distances[3]) { // More space on the left
      Serial.println("Turning left to avoid obstacle.");
      moveLeft(200,100);
    } else { // More space on the right
      Serial.println("Turning right to avoid obstacle.");
      moveRight(200,100);
    }
    stuckCounter++; // Increment stuck counter
  } 
  else if (distances[2] < 20) {                // Side Left (SL)
    Serial.println("Obstacle on left! Turning right.");
    moveRight(100,100);
    stuckCounter = 0; // Reset stuck counter
  } 
  else if (distances[3] < 20) {                // Side Right (SR)
    Serial.println("Obstacle on right! Turning left.");
    moveLeft(100,100);
    stuckCounter = 0; 
  } 
  else if (distances[4] < 40 || distances[5] < 40) {  // Back sensors (BL, BR)
    Serial.println("Obstacle behind! Moving forward.");
    //stopMotors(100);
    moveForward(100,100);
    //stopMotors(300);
    stuckCounter = 0; 
  } 
  else {
    Serial.println("Path is clear. Moving forward.");
    moveForward(100,100);
    stuckCounter = 0; 
  }
  // If the robot is stuck (repeated obstacle detection), make a full turn
  if (stuckCounter >= 3) {
    Serial.println("Robot appears to be stuck. Performing a full turn.");
    move180(240,200); // Perform a 180-degree turn 
    stopMotors(500);
    stuckCounter = 0; // Reset stuck counter after turning
  }
}
void navigateTest() {
  static int stuckCounter = 0;        // Counter to track stuck state

  // Read sensor data
  int frontLeft = distances[0];
  int frontRight = distances[1];
  int sideLeft = distances[2];
  int sideRight = distances[3];
  int backLeft = distances[4];
  int backRight = distances[5];

  // Define thresholds for obstacles
  bool frontObstacle = (frontLeft < 30 || frontRight < 30);
  bool backObstacle = (backLeft < 30 || backRight < 30);
  bool leftObstacle = (sideLeft < 30);
  bool rightObstacle = (sideRight < 30);

  // Navigation logic
  if (frontObstacle) {
    Serial.println("Obstacle ahead! Moving backward.");
    moveBackward(100, 200); // Move backward briefly
    stuckCounter++;
  } 
  else if (backObstacle) {
    Serial.println("Obstacle behind! Moving forward.");
    moveForward(100, 200); // Move forward briefly
    stuckCounter++;
  } 
  else if (leftObstacle && sideRight > sideLeft) {
    Serial.println("Obstacle on left! Turning right.");
    moveRight(100, 300); // Turn right
    stuckCounter = 0;
  } 
  else if (rightObstacle && sideLeft > sideRight) {
    Serial.println("Obstacle on right! Turning left.");
    moveLeft(100, 300); // Turn left
    stuckCounter = 0;
  } 
  else if (!frontObstacle && !backObstacle) {
    Serial.println("Path is clear. Moving forward.");
    moveForward(100, 300); // Keep moving forward
    stuckCounter = 0;      // Reset stuck counter
  }

  // Handle stuck state (repeated obstacle detection)
  if (stuckCounter >= 3) {  // If stuck for 3 cycles
    Serial.println("Robot appears to be stuck. Performing a 180-degree turn.");
    move180(200,800);      // Perform a 180-degree turn for 900ms
    stopMotors(1000);      
    stuckCounter = 0;       // Reset stuck counter after turning
  }
}



void loop() {
 
  if (dataReceived) {
    dataReceived = false; // Reset flag
    // Debug
    // Serial.print("Distances: ");
    // for (int i = 0; i < 6; i++) {
    //   Serial.print(distances[i]);
    //   if (i < 5) Serial.print(", ");
    //   if (i == 5) Serial.print(" ");
    // }
    // Serial.println();
    //navigate();  // Call navigation logic
    navigateTest();
  } else {
    // If no data, indicate waiting
    Serial.println("Waiting for data...");

  }
}
