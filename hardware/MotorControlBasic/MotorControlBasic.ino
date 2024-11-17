// Sensor Monitoring Pins
const int sensorWire1 = A0; // Connect one sensor wire to A0
const int sensorWire2 = A1; // Connect the second sensor wire to A1
const float referenceVoltage = 5.0; // Arduino's reference voltage (5V for most boards)

// Define motor pins for L298N Motor Driver #1
const int LF_IN1 = 13;  // Front Left Motor IN1
const int LF_IN2 = 7;   // Front Left Motor IN2
const int LF_ENA = 10;  // Enable pin for Left Front Motor (PWM)

// Define motor pins for L298N Motor Driver #2
const int RF_IN1 = 8;   // Front Right Motor IN1
const int RF_IN2 = 9;   // Front Right Motor IN2
const int RF_ENA = 11;  // Enable pin for Right Front Motor (PWM)

// Define motor pins for L298N Motor Driver #3
const int LR_IN3 = 4;   // Back Left Motor IN3
const int LR_IN4 = 12;  // Back Left Motor IN4
const int LR_ENA = 5;   // Enable pin for Left Rear Motor (PWM)

// Define motor pins for L298N Motor Driver #4
const int RR_IN3 = 2;   // Back Right Motor IN3
const int RR_IN4 = 3;   // Back Right Motor IN4
const int RR_ENA = 6;   // Enable pin for Right Rear Motor (PWM)

void setup() {
   // Initialize Serial Monitor for sensor monitoring
  Serial.begin(9600);

  // Configure sensor pins as inputs
  pinMode(sensorWire1, INPUT);
  pinMode(sensorWire2, INPUT);

  // Set motor control pins as output
  pinMode(LF_IN1, OUTPUT);
  pinMode(LF_IN2, OUTPUT);
  pinMode(LF_ENA, OUTPUT);

  pinMode(RF_IN1, OUTPUT);
  pinMode(RF_IN2, OUTPUT);
  pinMode(RF_ENA, OUTPUT);

  pinMode(LR_IN3, OUTPUT);
  pinMode(LR_IN4, OUTPUT);
  pinMode(LR_ENA, OUTPUT);

  pinMode(RR_IN3, OUTPUT);
  pinMode(RR_IN4, OUTPUT);
  pinMode(RR_ENA, OUTPUT);
}

// Function to stop all motors
void stopMotors() {
  analogWrite(LF_ENA, 0);
  analogWrite(RF_ENA, 0);
  analogWrite(LR_ENA, 0);
  analogWrite(RR_ENA, 0);

  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN1, LOW);
  digitalWrite(RF_IN2, LOW);
  digitalWrite(LR_IN3, LOW);
  digitalWrite(LR_IN4, LOW);
  digitalWrite(RR_IN3, LOW);
  digitalWrite(RR_IN4, LOW);
}

// Move forward
void moveForward(int speed) {
  analogWrite(LF_ENA, speed);
  digitalWrite(LF_IN1, HIGH);
  digitalWrite(LF_IN2, LOW);

  analogWrite(RF_ENA, speed);
  digitalWrite(RF_IN1, HIGH);
  digitalWrite(RF_IN2, LOW);

  analogWrite(LR_ENA, speed);
  digitalWrite(LR_IN3, HIGH);
  digitalWrite(LR_IN4, LOW);

  analogWrite(RR_ENA, speed);
  digitalWrite(RR_IN3, HIGH);
  digitalWrite(RR_IN4, LOW);
}

// Move backward
void moveBackward(int speed) {
  analogWrite(LF_ENA, speed);
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, HIGH);

  analogWrite(RF_ENA, speed);
  digitalWrite(RF_IN1, LOW);
  digitalWrite(RF_IN2, HIGH);

  analogWrite(LR_ENA, speed);
  digitalWrite(LR_IN3, LOW);
  digitalWrite(LR_IN4, HIGH);

  analogWrite(RR_ENA, speed);
  digitalWrite(RR_IN3, LOW);
  digitalWrite(RR_IN4, HIGH);
}

// Strafe left
void strafeLeft(int speed) {
  analogWrite(LF_ENA, speed);
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, HIGH);

  analogWrite(RF_ENA, speed);
  digitalWrite(RF_IN1, HIGH);
  digitalWrite(RF_IN2, LOW);

  analogWrite(LR_ENA, speed);
  digitalWrite(LR_IN3, HIGH);
  digitalWrite(LR_IN4, LOW);

  analogWrite(RR_ENA, speed);
  digitalWrite(RR_IN3, LOW);
  digitalWrite(RR_IN4, HIGH);
}

// Strafe right
void strafeRight(int speed) {
  analogWrite(LF_ENA, speed);
  digitalWrite(LF_IN1, HIGH);
  digitalWrite(LF_IN2, LOW);

  analogWrite(RF_ENA, speed);
  digitalWrite(RF_IN1, LOW);
  digitalWrite(RF_IN2, HIGH);

  analogWrite(LR_ENA, speed);
  digitalWrite(LR_IN3, LOW);
  digitalWrite(LR_IN4, HIGH);

  analogWrite(RR_ENA, speed);
  digitalWrite(RR_IN3, HIGH);
  digitalWrite(RR_IN4, LOW);
}

// Function for monitoring sensors
void monitorBatterySensors() {
  // Read analog values from sensors
  int rawValue1 = analogRead(sensorWire1);
  int rawValue2 = analogRead(sensorWire2);

  // Convert raw values to voltage
  float voltage1 = (rawValue1 / 1023.0) * referenceVoltage;
  float voltage2 = (rawValue2 / 1023.0) * referenceVoltage;

  // Print sensor values to the Serial Monitor
  Serial.print("Sensor Wire 1 Voltage: ");
  Serial.print(voltage1);
  Serial.println(" V");

  Serial.print("Sensor Wire 2 Voltage: ");
  Serial.print(voltage2);
  Serial.println(" V");

  Serial.println("-----------------------------");
}

void loop() {
  monitorBatterySensors();
  moveForward(255);  // Move forward at full speed
  delay(1000);       // Move for 1 second
  stopMotors();
  delay(5000);        // Pause for 0.5 seconds

  moveBackward(255); // Move backward at full speed
  delay(1000);       // Move for 1 second
  stopMotors();
  delay(5000);        // Pause for 0.5 seconds

  strafeLeft(255);   // Strafe left at full speed
  delay(1000);       // Move for 1 second
  stopMotors();
  delay(5000);        // Pause for 0.5 seconds

  strafeRight(255);  // Strafe right at full speed
  delay(1000);       // Move for 1 second
  stopMotors();
  delay(5000);        // Pause for 0.5 seconds
  
}
