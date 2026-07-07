#include "NewPing.h"
const int IRSensorYellow = 3;
const int IRSensorRed = 4;
const int ultrasoundTrigger = 2;
const int ultrasoundEchoOut = 5;
const char temperatureDetector[] "A0";
const char motionDetector[] "A1";
const char analog_In_Mic[] = "A2";
const char digital_In_Mic[] = "A3";
int maxDistance = 400;
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IRSensorYellow, INPUT);
  pinMode(IRSensorRed, INPUT);
  pinMode(ultrasoundTrigger, OUTPUT);
  pinMode(ultrasoundEchoOut, INPUT);
  pinMode(temperatureDetector, INPUT);
  pinMode(motionDetector, INPUT);
  pinMode(analog_In_Mic, INPUT);
  pinMode(digital_In_Mic, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("******************** Select Your Sensor: ********************");
  Serial.println("Sensors: Yellow Light IR = 1, Red Light Ultrasound = 2, Temperature = 3, Motion = 4, Sound = 5");
  char sensorChoice = Serial.read();
  switch(sensorChoice) {
    case 1:
      // IR
    case 2:
      // Ultrasound
        NewPing sonar(ultrasoundTrigger, ultrasoundEchoOut, maxDistance);
        Serial.print(sonar.ping_cm())
        delay(3000);
    case 3:
      // Temperature
    case 4:
      // Motion
    case 5:
      // Sound
  }

  int temperature = 
}
