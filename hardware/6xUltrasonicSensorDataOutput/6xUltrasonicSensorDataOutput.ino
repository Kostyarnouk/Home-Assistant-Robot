// Define pins for each HC-SR04 ultrasonic sensor using specified variable names
// Back Right Sensor:
const int trigBR = 2;
const int echoBR = 3;
//Back Left Sensor:
const int trigBL = 4;
const int echoBL = 5;
// Front Left Sensor:
const int trigFL = 6;
const int echoFL = 7;
// Front Right Sensor:
const int trigFR = 8;
const int echoFR = 9;
//Side Right Sensor:
const int trigSR = 10;  
const int echoSR = 11;
// Side Left Sensor:
const int trigSL = 12;  
const int echoSL = 13;

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
  // Initialize sensor pins as output and input
  pinMode(trigBR, OUTPUT);
  pinMode(echoBR, INPUT);
  pinMode(trigBL, OUTPUT);
  pinMode(echoBL, INPUT);
  pinMode(trigFL, OUTPUT);
  pinMode(echoFL, INPUT);
  pinMode(trigFR, OUTPUT);
  pinMode(echoFR, INPUT);
  pinMode(trigSR, OUTPUT);  
  pinMode(echoSR, INPUT);
  pinMode(trigSL, OUTPUT);  
  pinMode(echoSL, INPUT);

  Serial.begin(9600);  // Start serial communication for Serial Plotter
}

void loop() {
  // Measure distances from each sensor
  long distanceBR = measureDistance(trigBR, echoBR);
  long distanceBL = measureDistance(trigBL, echoBL);
  long distanceFL = measureDistance(trigFL, echoFL);
  long distanceFR = measureDistance(trigFR, echoFR);
  long distanceSR = measureDistance(trigSR, echoSR); 
  long distanceSL = measureDistance(trigSL, echoSL);  
  // Send data to Serial Monitor in a clean format
  Serial.print(distanceFL); Serial.print(",");
  Serial.print(distanceFR); Serial.print(",");
  Serial.print(distanceSR); Serial.print(",");
  Serial.print(distanceBR); Serial.print(",");
  Serial.print(distanceBL); Serial.print(",");
  Serial.println(distanceSL);

  // Print distances in a format compatible with the Serial Plotter
  // Each sensor has a separate line in the plot to simulate individual bar graphs
  //Serial.print("Back_Right:"); Serial.print(distanceBR); Serial.print(" ");
  //Serial.print("Back_Left:"); Serial.print(distanceBL); Serial.print(" ");
  //Serial.print("Front_Left:"); Serial.print(distanceFL); Serial.print(" ");
  //Serial.print("Front_Right:"); Serial.print(distanceFR); Serial.print(" ");
  //Serial.print("Side_Right:"); Serial.print(distanceSR); Serial.print(" ");
  //Serial.print("Side_Left:"); Serial.print(distanceSL); Serial.println();

  //delay(200);  // Small delay for readability in Serial Plotter
}
