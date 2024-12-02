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

void moveMotorsCalibrated(int speedOthers, int speedMotor2, int dir1, int dir2, int dir3, int dir4, int timeLength) {
  controlMotor(0, speedOthers, dir1);    // Back Right
  controlMotor(1, speedOthers, dir2);    // Back Left
  controlMotor(2, speedMotor2, dir3);    // Front Right (Motor 2 with specific speed)
  controlMotor(3, speedOthers, dir4);    // Front Left
  if (timeLength > 0) {                  // Only trigger delay if timeLength is greater than 0
    delay(timeLength);                   // Smooth adjustment between actions
  }
}

//FL FR BL BR
void moveForward(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2, -1, -1, -1, -1, time); // All wheels move backward
}

void moveBackward(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2, 1, 1, 1, 1, time); // All wheels move forward
}

void moveLeft(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2 + 48, 1, -1, -1, 1, time); // Leftward movement
}

void moveRight(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2 + 48, -1, 1, 1, -1, time); // Rightward movement
}

void moveDiagonalForwardLeft(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2 + 50, 0, -1, -1, 0, time); // Front Left diagonal
}

void moveDiagonalForwardRight(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2 + 130, -1, 0, 0, -1, time); // Front Right diagonal
}

void moveDiagonalBackwardLeft(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2 + 50, 0, 1, 1, 0, time); // Back Left diagonal
}

void moveDiagonalBackwardRight(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers, speedMotor2 + 50, 1, 0, 0, 1, time); // Back Right diagonal
}

void rotateClockwise(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers + 50, speedMotor2 + 60, 1, -1, 1, -1, time); // Wheels rotate in opposite directions
}

void rotateCounterClockwise(int speedOthers, int speedMotor2, int time) {
  moveMotorsCalibrated(speedOthers + 50, speedMotor2 + 60, -1, 1, -1, 1, time); // Wheels rotate in opposite directions
}

void rotate180() {
  moveMotorsCalibrated(175, 185, -1, 1, -1, 1, 600); // Wheels rotate in opposite directions
}

void rotate360() {
  moveMotorsCalibrated(175, 185, -1, 1, -1, 1, 1200); // Wheels rotate in opposite directions
}

//Distances[FL,FR,SL,SR,BL,BR]
// Navigation Logic for Testing
void MovementTest() {
  int testSpeed = 125;  // Set a fixed speed for testing
  int testTime = 1000;  // Set a fixed time for each movement

  Serial.println("Testing movements...");

  // Test Forward Movement
  Serial.println("Testing: Move Forward");
  moveForward(testSpeed, testSpeed, testTime);
  stopMotors(500); // Pause after movement

  // Test Backward Movement
  Serial.println("Testing: Move Backward");
  moveBackward(testSpeed, testSpeed, testTime);
  stopMotors(500);

  // Test Left Movement
  Serial.println("Testing: Move Left");
  moveLeft(testSpeed, testSpeed, testTime);
  stopMotors(500);

  // Test Right Movement
  Serial.println("Testing: Move Right");
  moveRight(testSpeed, testSpeed, testTime);
  stopMotors(500);

  // Test Diagonal Forward Left
  Serial.println("Testing: Diagonal Forward Left");
  moveDiagonalForwardLeft(testSpeed, testSpeed, testTime);
  stopMotors(500);

  // Test Diagonal Forward Right
  Serial.println("Testing: Diagonal Forward Right");
  moveDiagonalForwardRight(testSpeed, testSpeed, testTime);
  stopMotors(500);

  // Test Diagonal Backward Left
  Serial.println("Testing: Diagonal Backward Left");
  moveDiagonalBackwardLeft(testSpeed, testSpeed, testTime);
  stopMotors(500);

  // Test Diagonal Backward Right
  Serial.println("Testing: Diagonal Backward Right");
  moveDiagonalBackwardRight(testSpeed, testSpeed, testTime);
  stopMotors(500);

  //Test Rotate Clockwise
  Serial.println("Testing: Rotate Clockwise");
  rotate180();
  stopMotors(1000);

  // Test Rotate Counterclockwise
  Serial.println("Testing: Rotate Counterclockwise");
  rotate360();
  stopMotors(1000);

  Serial.println("Movement tests complete.");
}
void navigateTest() {
  int speed = 100;           // Set speed for movement
  int timePerMove = 0;     // Pause time 

  // Read sensor data
  int frontLeft = distances[0];
  int frontRight = distances[1];
  int sideLeft = distances[2];
  int sideRight = distances[3];
  int backLeft = distances[4];
  int backRight = distances[5];

  // Define thresholds for obstacles
  bool frontObstacle = (frontLeft < 35 || frontRight < 35); // Threshold for front sensors
  bool leftClear = (sideLeft > 30);  // Check if left side is clear
  bool rightClear = (sideRight > 30); // Check if right side is clear
  bool backObstacle = (backLeft < 35 || backRight < 35); // Threshold for back sensors

  // Navigation logic
  if (frontObstacle) {
    Serial.println("Obstacle ahead!");

    // Try to move right if clear
    if (rightClear) {
      Serial.println("Right is clear. Moving right.");
      moveRight(speed, speed, timePerMove); // Move right briefly
    }
    // If right is blocked, try left
    else if (leftClear) {
      Serial.println("Right blocked. Left is clear. Moving left.");
      moveLeft(speed, speed, timePerMove); // Move left briefly
    }
    // If front and both sides are blocked, perform a 180-degree turn
    else {
      Serial.println("Front and sides blocked! Performing 180-degree turn.");
      rotate180();
      stopMotors(500); // Pause briefly to calculate next move
    }
  } 
  // If no obstacle ahead, move forward
  else if (!frontObstacle) {
    Serial.println("Path is clear. Moving forward.");
    moveForward(speed, speed, 200); // Smooth forward movement
  }

  // If the back sensors detect an obstacle, stop moving backward
  if (backObstacle) {
    Serial.println("Obstacle detected behind! Avoiding backward movement.");
    stopMotors(500);
  }
}




//Distances[FL,FR,SL,SR,BL,BR]
void navigate() {
  static int stuckCounter = 0;        // Counter to track stuck state

  // Read sensor data
  int frontLeft = distances[0];
  int frontRight = distances[1];
  int sideLeft = distances[2];
  int sideRight = distances[3];
  int backLeft = distances[4];
  int backRight = distances[5];

  // Define thresholds for obstacles
  bool frontObstacle = (frontLeft < 40 || frontRight < 40);
  bool backObstacle = (backLeft < 30 || backRight < 30);
  bool leftObstacle = (sideLeft < 30);
  bool rightObstacle = (sideRight < 30);

  // Navigation logic
  if (frontObstacle) {
    Serial.println("Obstacle ahead! Moving backward.");

    if (leftObstacle && sideRight > sideLeft && sideRight >20) {
    Serial.println("Obstacle on left! Turning right.");
    moveRight(125, 125, 100); // Turn right

    } 
    else if (rightObstacle && sideLeft > sideRight && sideLeft >20) {
      Serial.println("Obstacle on right! Turning left.");
      moveLeft(125, 125, 100); // Turn left

    }
  moveBackward(125, 125, 100); // Move backward briefly 
  } 
  // else if (leftObstacle && sideRight > sideLeft && sideRight >20) {
  //   Serial.println("Obstacle on left! Turning right.");
  //   moveRight(125, 125, 100); // Turn right
  //   stuckCounter = 0;
  // } 
  // else if (rightObstacle && sideLeft > sideRight && sideLeft >20) {
  //   Serial.println("Obstacle on right! Turning left.");
  //   moveLeft(125, 125, 100); // Turn left
  //   stuckCounter = 0;
  // } 
  else if (!frontObstacle && !backObstacle) {
    Serial.println("Path is clear. Moving forward.");
    moveForward(125, 125, 100); // Keep moving forward
    stuckCounter = 0;      // Reset stuck counter
  }
    else if (backObstacle) {
    Serial.println("Obstacle behind! Moving forward.");
    moveForward(125, 125, 100); // Move forward briefly
    stuckCounter++;
  } 

  // Handle stuck state (repeated obstacle detection)
  if (frontObstacle && leftObstacle && rightObstacle) {  // If stuck for 3 cycles
    Serial.println("Robot appears to be stuck. Performing a 180-degree turn.");
    rotate180();      // Perform a 180-degree turn for 900ms
    stopMotors(1000);      
    stuckCounter = 0;       // Reset stuck counter after turning
  }
  delay(100);
}



void loop() {
  //delay(600);
  if (dataReceived) {
    dataReceived = false; // Reset flag
    // moveLeft(150,400);
    // delay(1000);
    // stopMotors(500);

    // Debug
    Serial.print("Distances: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(distances[i]);
      if (i < 5) Serial.print(", ");
      if (i == 5) Serial.print(" ");
    }
    Serial.println();
    //navigate();  
    MovementTest();
    //navigateTest();
  } else {
    // If no data, indicate waiting
    Serial.println("Waiting for data...");

  }
}
