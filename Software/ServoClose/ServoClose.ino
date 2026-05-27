#include <Arduino.h>
#include <Servo.h>

Servo myservoL;
Servo myservoR;

// Pin setup
const int LEFT_SERVO_PIN= 5;
const int RIGHT_SERVO_PIN= 6;

// Course setup
const int openLeftAngle= 20; 
const int openRightAngle= 180 - openLeftAngle;
const int closeAngle= 105;

void setup() {
  myservoL.attach(LEFT_SERVO_PIN);
  myservoR.attach(RIGHT_SERVO_PIN);
 }

void loop() {
      myservoL.write(openLeftAngle + closeAngle);  
      myservoR.write(openRightAngle - closeAngle);
}
