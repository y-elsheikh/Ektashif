const int IN3 = 12;
const int IN4 = 13;
const int ENA = 11;

// Motor Right
const int IN1 = 8;
const int IN2 = 9;
const int ENB = 10;

// Infrared Sensors
const int infraSensorL = 5;
const int infraSensorR = 6;

// Ultrasonic Sensor
const int triggerIN = 2;
const int echoOUT = A5;

// LED
const int leftLEDpair = 3;
const int rightLEDpair = 4;

// Microphone
const int microphone = 7;

// Detection distance
float detectionDistance = 12.5;


// System toggle
bool systemActive = false;
bool lastMicState = LOW;
bool startBlink = false;


void setup() {

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);


  // IR sensors
  pinMode(infraSensorL, INPUT);
  pinMode(infraSensorR, INPUT);


  // Ultrasonic
  pinMode(triggerIN, OUTPUT);
  pinMode(echoOUT, INPUT);


  // LEDs
  pinMode(leftLEDpair, OUTPUT);
  pinMode(rightLEDpair, OUTPUT);


  // Microphone
  pinMode(microphone, INPUT);


  Serial.begin(9600);
}


// ---------------- MOTOR FUNCTIONS ----------------

void moveForward() {

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}


void turnLeft() {

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 150);
}


void turnRight() {

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  analogWrite(ENA, 150);
  analogWrite(ENB, 0);
}


void stopMotors() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


// ---------------- ULTRASONIC ----------------

float getDistance() {

  digitalWrite(triggerIN, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(triggerIN, LOW);


  long time = pulseIn(echoOUT, HIGH, 30000);


  if (time == 0) {
    return 999;
  }


  float distance = time * 0.0343 / 2.0;

  return distance;
}


// ---------------- LINE FOLLOWING ----------------

void lineFollow() {

  int leftSensor = digitalRead(infraSensorL);
  int rightSensor = digitalRead(infraSensorR);


  // White + White
  if (leftSensor == HIGH && rightSensor == HIGH) {
    moveForward();
  }


  // Left sensor on black
  else if (leftSensor == LOW && rightSensor == HIGH) {
    turnLeft();
  }


  // Right sensor on black
  else if (leftSensor == HIGH && rightSensor == LOW) {
    turnRight();
  }


  // Both on black
  else if (leftSensor == LOW && rightSensor == LOW) {
    moveForward();
  }

}


// ---------------- LED START FLASH ----------------

void startLEDblink() {

  digitalWrite(leftLEDpair, HIGH);
  digitalWrite(rightLEDpair, HIGH);

  delay(300);

  digitalWrite(leftLEDpair, LOW);
  digitalWrite(rightLEDpair, LOW);
}


// ---------------- MICROPHONE ----------------

void checkSnap() {

  bool micState = digitalRead(microphone);


  // Detect snap rising edge
  if (micState == HIGH && lastMicState == LOW) {


    systemActive = !systemActive;


    if (systemActive) {

      startBlink = true;

    }

    else {

      stopMotors();

    }


    delay(200);

  }


  lastMicState = micState;

}


// ---------------- MAIN LOOP ----------------

void loop() {

  checkSnap();


  if (systemActive) {


    if (startBlink) {

      startLEDblink();
      startBlink = false;

    }


    float distance = getDistance();


    if (distance <= detectionDistance) {

      stopMotors();

    }

    else {

      lineFollow();

    }


  }


  else {

    stopMotors();

    digitalWrite(leftLEDpair, LOW);
    digitalWrite(rightLEDpair, LOW);

  }

}