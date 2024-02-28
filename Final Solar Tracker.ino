// Motor one
int enA = 5;
int in1 = 9;
int in2 = 8;

// Motor two
int enB = 10;
int in3 = 7;
int in4 = 6;

// Motor three
int enC = 3;
int in5 = 11;
int in6 = 12;

const int pushButton = 13;
const int pushButton1 = 2;
const int pushButton2 = 4;
const int pushButton3 = A4;

// LDR pins
int ldrPin1 = A0; // Connect LDR1 to analog pin A0
int ldrPin2 = A1; // Connect LDR2 to analog pin A1
int ldrPin3 = A2; // Connect LDR3 to analog pin A2
int ldrPin4 = A3; // Connect LDR4 to analog pin A3

// Light threshold
int lightThreshold = 100;

volatile bool manualControl = false;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(pushButton3, INPUT_PULLUP);

  // Attach interrupts for manual control
  attachInterrupt(digitalPinToInterrupt(pushButton), manualControlHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(pushButton1), manualControlHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(pushButton2), manualControlHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(pushButton3), manualControlHandler, FALLING);
}

void loop() {
  if (!manualControl) {
    // Automatic operation based on LDR readings
    automaticOperation();
  } else {
    // Manual operation
    manualOperation();
  }
}

void automaticOperation() {
  // Read analog values from LDRs
  int ldrValue1 = analogRead(ldrPin1);
  int ldrValue2 = analogRead(ldrPin2);
  int ldrValue3 = analogRead(ldrPin3);
  int ldrValue4 = analogRead(ldrPin4);

  // Check if light is detected by LDRs
  bool lightDetected = (ldrValue1 > lightThreshold) && (ldrValue2 > lightThreshold) &&
                       (ldrValue3 > lightThreshold) && (ldrValue4 > lightThreshold);

  if (lightDetected) {
    // Check the difference between the light levels of LDRs
    int difference = ldrValue1 - ldrValue2;
    int different = ldrValue3 - ldrValue4;

    // Determine motor direction based on the difference in light levels
    if (difference > lightThreshold) {
      // LDR1 has higher light level, rotate motors A and B clockwise
      rotateClockwise();
    } else if (difference < -lightThreshold) {
      // LDR2 has higher light level, rotate motors A and B counterclockwise
      rotateCounterclockwise();
    } else {
      // Stop motors A and B
      stopMotorsAB();
    }

    if (different > lightThreshold) {
      // LDR3 has higher light level, rotate motor C forward
      rotateForward();
    } else if (different < -lightThreshold) {
      // LDR4 has higher light level, rotate motor C backward
      rotateBackward();
    } else {
      // Stop motor C
      stopMotorC();
    }
  } else {
    // No light detected, stop all motors
    stopAllMotors();
  }

  delay(100); // Adjust delay as needed to prevent rapid changes
}

void manualOperation() {
  if (digitalRead(pushButton) == LOW) { // Check if button is pressed (LOW because of INPUT_PULLUP)
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 140); // Assuming PWM control for motor speed
    analogWrite(enB, 100);
  } else if (digitalRead(pushButton1) == LOW) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 140); // Assuming PWM control for motor speed
    analogWrite(enB, 100);
  } else if (digitalRead(pushButton2) == LOW) {
    digitalWrite(in5, LOW);
    digitalWrite(in6, HIGH);
    analogWrite(enC, 100); // Assuming PWM control for motor speed
  } else if (digitalRead(pushButton3) == LOW) {
    digitalWrite(in6, LOW);
    digitalWrite(in5, HIGH);
    analogWrite(enC, 100);
  } else {
    stopAllMotors(); // Stop all motors if no buttons are pressed
  }
}

void rotateClockwise() {
  // Motor A rotates clockwise
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 80);

  // Motor B rotates clockwise
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 60);
}

void rotateForward() {
  // Motor C rotates forward
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, 60);
}

void rotateCounterclockwise() {
  // Motor A rotates counterclockwise
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 80);

  // Motor B rotates counterclockwise
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 60);
}

void rotateBackward() {
  // Motor C rotates backward
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, 60);
}

void stopMotorsAB() {
  // Stop motors A and B
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void stopMotorC() {
  // Stop motor C
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
}

void stopAllMotors() {
  // Stop all motors
  stopMotorsAB();
  stopMotorC();
}

void manualControlHandler() {
  // Toggle manual control mode when any pushbutton is pressed
  manualControl = !manualControl;
}
