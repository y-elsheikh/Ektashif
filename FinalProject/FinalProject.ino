const int IN3 = 12;
const int IN4 = 13;
const int ENA = 10;

// Motor Right
const int IN1 = 8;
const int IN2 = 9;
const int ENB = 11;

// Infrared Sensors
const int infraSensorL = 5;
const int infraSensorR = 7;

// Ultrasonic Sensor
const int triggerIN = 2;
const int echoOUT = A1;

// LED
const int leftLEDpair = 3;
const int rightLEDpair = 4;

// Microphone
const int microphone = 6;


// Detection distance
float detectionDistance = 12.5;

// System toggle
bool systemActive = false;
bool lastMicState = LOW;
bool startBlink = false;

// LED timing
unsigned long previousLEDMillis = 0;
bool policeLEDState = false;

const int LEDinterval = 300;



void setup() {

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);


  pinMode(infraSensorL, INPUT);
  pinMode(infraSensorR, INPUT);


  pinMode(triggerIN, OUTPUT);
  pinMode(echoOUT, INPUT);


  pinMode(leftLEDpair, OUTPUT);
  pinMode(rightLEDpair, OUTPUT);


  pinMode(microphone, INPUT);


  Serial.begin(9600);

}



// ---------------- MOTOR FUNCTIONS ----------------

void moveForward() {

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(ENA, 85);
  analogWrite(ENB, 85);

}



void curveLeft() {

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Slow left wheel, speed up right wheel
  analogWrite(ENA, 45);
  analogWrite(ENB, 185);

  digitalWrite(leftLEDpair, HIGH);
  digitalWrite(rightLEDpair, LOW);

}



void curveRight() {

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Speed up left wheel, slow right wheel
  analogWrite(ENA, 185);
  analogWrite(ENB, 45);

  digitalWrite(leftLEDpair, LOW);
  digitalWrite(rightLEDpair, HIGH);

}



void adjustLeft() {

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(ENA, 45);
  analogWrite(ENB, 185);

  digitalWrite(leftLEDpair, HIGH);
  digitalWrite(rightLEDpair, LOW);

}



void adjustRight() {

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(ENA, 185);
  analogWrite(ENB, 45);

  digitalWrite(leftLEDpair, LOW);
  digitalWrite(rightLEDpair, HIGH);

}



void stopMotors() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(leftLEDpair, LOW);
  digitalWrite(rightLEDpair, LOW);

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



  return time * 0.0343 / 2.0;

}



// ---------------- LED POLICE FLASH ----------------


void policeLights() {


  if (millis() - previousLEDMillis >= LEDinterval) {


    previousLEDMillis = millis();


    policeLEDState = !policeLEDState;



    if (policeLEDState) {

      digitalWrite(leftLEDpair, HIGH);
      digitalWrite(rightLEDpair, LOW);

    }

    else {

      digitalWrite(leftLEDpair, LOW);
      digitalWrite(rightLEDpair, HIGH);

    }

  }

}



// ---------------- LINE FOLLOWING ----------------


void lineFollow() {

  int leftSensor = digitalRead(infraSensorL);
  int rightSensor = digitalRead(infraSensorR);


  // Both sensors on white -> go straight
  if (leftSensor == LOW && rightSensor == LOW) {

    moveForward();

  }


  // Left sensor sees black -> correct left
  else if (leftSensor == HIGH && rightSensor == LOW) {

    adjustLeft();

  }


  // Right sensor sees black -> correct right
  else if (leftSensor == LOW && rightSensor == HIGH) {

    adjustRight();

  }


  // Both sensors see black -> slow down and continue
  else if (leftSensor == HIGH && rightSensor == HIGH) {

    moveForward();

  }

}


// ---------------- MICROPHONE ----------------


void checkSnap() {

  bool micState = digitalRead(microphone);


  // Detect clap rising edge
  if (micState == HIGH && lastMicState == LOW) {

    systemActive = !systemActive;


    if (systemActive) {

      startBlink = true;

    }

    else {

      stopMotors();

    }


    delay(50);

  }


  lastMicState = micState;

}



// ---------------- LOOP ----------------


void loop() {


  checkSnap();



  if (systemActive) {



    float distance = getDistance();



    if (distance <= detectionDistance) {


      stopMotors();


      policeLights();


    }


    else {


      lineFollow();


    }



  }


  else {


    stopMotors();


  }


}