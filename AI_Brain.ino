// Arduino pins for sensors and motors
const int leftUltrasonicPin = 7;    // digital pin for left ultrasonic sensor.  Digital because analog signal is processed on other ardunio
const int frontUltrasonicPin = 6;   // digital pin for front ultrasonic sensor  Digital because analog signal is processed on other ardunio
const int rightUltrasonicPin = 5;   // digital pin for right ultrasonic sensor  Digital because analog signal is processed on other ardunio


const int leftLineSensorPin = A3;    // Analog pin for left line sensor
const int middleLineSensorPin = A4;  // Analog pin for middle line sensor
const int rightLineSensorPin = A5;   // Analog pin for right line sensor

const int leftMotorPin1 = 8;         // Digital pins for left motor control
const int leftMotorPin2 = 9;
const int leftMotorPWM = 10;

const int rightMotorPin1 = 13;       // Digital pin for right motor control
const int rightMotorPin2 = 12;
const int rightMotorPWM = 11;

// Define distance thresholds
const float leftdist = 5;  //change these to be accurate
const float rightdist = 5;
const float frontdist = 5;

// Function prototypes
void followLinetoT();
void runtoline();
void startingZoneBehavior();
void approachContactZone();

// Function to follow the tape line to T
void followLinetoT() {
  while (true) {
    float leftLineSensorData = analogRead(leftLineSensorPin);
    float middleLineSensorData = analogRead(middleLineSensorPin);
    float rightLineSensorData = analogRead(rightLineSensorPin);
    // Implement line-following logic based on line sensor data
    if (leftLineSensorData == 1 && middleLineSensorData == 1 && rightLineSensorData == 1) {
      Serial.println("I found the T!");
    } else if (middleLineSensorData == 1) {
        // The middle sensor detects the tape, continue forward
        digitalWrite(leftMotorPin1, 1);
        digitalWrite(leftMotorPin2, 0);
        digitalWrite(rightMotorPin1, 1);
        digitalWrite(rightMotorPin2, 0);
    } else if (rightLineSensorData == 1) {
        digitalWrite(leftMotorPin1, 1);  // Activate left motor
        digitalWrite(leftMotorPin2, 0); 
        delay(200);  // Rotate for 200 milliseconds
        digitalWrite(leftMotorPin1, 0);   // Deactivate left motor
    } else if (leftLineSensorData == 1) {
        digitalWrite(rightMotorPin1, 1);  // Activate right motor
        digitalWrite(rightMotorPin2, 0);
        delay(200);  // Rotate for 200 milliseconds
        digitalWrite(rightMotorPin1, 0);   // Deactivate right motor
    }
  }
}

void runtoline() {
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  delay(150); //Make this non-blocking
  digitalWrite(leftMotorPin1, HIGH);
  delay (1000); //make non-blocking
}

void approachContactZone() {

}

// Function to implement behavior in the starting zone
void startingZoneBehavior() {
    // Initialize variables to track distances from walls
    while (true) {
        // Read ultrasonic sensor data
        int leftDistance = analogRead(leftUltrasonicPin);
        int frontDistance = analogRead(frontUltrasonicPin);
        int rightDistance = analogRead(rightUltrasonicPin);

        // Check if desired distances from each wall are reached
        if (leftDistance >= leftdist && frontDistance >= frontdist && rightDistance >= rightdist) {
            // Rotate the robot counterclockwise by activating only the right motor for 200 milliseconds
            digitalWrite(rightMotorPin1, HIGH);  // Activate right motor
            delay(200);  // Nake this non-blocking
            digitalWrite(rightMotorPin1, LOW);   // Deactivate right motor
            // return to collecting distance data
            break;
        }
        
        // Robot is now facing the relatively correct direction
        // Can begin moving forward and line following
        Serial.println("I think I'm lined up");
    }
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Implement behavior in the starting zone
    startingZoneBehavior();
    runtoline();
    followLinetoT();
    approachContactZone();

}
