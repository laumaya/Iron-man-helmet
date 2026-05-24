#include <Arduino.h>
#include <Servo.h>
#include <AceButton.h>
using namespace ace_button;

Servo myservoL;
Servo myservoR;

const int LEFT_SERVO_PIN= 5;
const int RIGHT_SERVO_PIN= 6;

const int B_OPEN_CLOSE_BUTTON = 2;
const int B_ON_OFF_LED_BUTTON= 3;
const int LED_PIN = 9;

const int LED_ON = 1;
const int LED_OFF = 0;

const int openLeftAngle= 45; // 15
const int openRightAngle= 180 - openLeftAngle;
const int closeAngle= 130; // 150

const unsigned long courseTime= 450;

AceButton buttonOpenClose(B_OPEN_CLOSE_BUTTON);
AceButton buttonOnOffLed(B_ON_OFF_LED_BUTTON);

bool closed= true;
bool lightOn= true;

void buttonHandleEvent(AceButton*, uint8_t, uint8_t);
void detachServo();
void attachServo();

void setup() {
  delay(200);
 
  pinMode(B_OPEN_CLOSE_BUTTON, INPUT_PULLUP);
  pinMode(B_ON_OFF_LED_BUTTON, INPUT_PULLUP);               
  pinMode(LED_PIN, OUTPUT);   
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(buttonHandleEvent);
  
  bool on= true;
  const int count= 19;
  for (int i= 0; i < count; ++i)
  {      
      if (on) digitalWrite(LED_PIN, LED_ON);
      else digitalWrite(LED_PIN, LED_OFF);
      on= !on;
      delay (((count - i) * 2));
  }
  
  delay(200);

}

void loop() {
  const bool previousClosed= closed;
  const bool previousLightOn= lightOn;
  buttonOpenClose.check();
  buttonOnOffLed.check();
  if (closed != previousClosed)
  {
    if (!closed)
    {
      attachServo();
      myservoL.write(openLeftAngle);  
      myservoR.write(openRightAngle);
      digitalWrite(LED_PIN, LED_OFF);
      lightOn= false;
      delay(courseTime);
    }
    else
    {
      attachServo();
      myservoL.write(openLeftAngle + closeAngle);  
      myservoR.write(openRightAngle - closeAngle);
      delay(courseTime);
      digitalWrite(LED_PIN, LED_ON);
      lightOn= true;
    }
    detachServo();
  }
  else if (lightOn != previousLightOn)
  {
    if (lightOn)
    {
        digitalWrite(LED_PIN, LED_ON);
    }
    else
    {
        digitalWrite(LED_PIN, LED_OFF);
    }
  }
}

void buttonHandleEvent(AceButton* button , uint8_t eventType, uint8_t /* buttonState */) {
  switch (eventType){
    case AceButton::kEventPressed :
      if (button->getPin() == B_OPEN_CLOSE_BUTTON)
      {
          closed= !closed;
      }
      else if (closed)
      {
          lightOn= !lightOn;
      }   
      break;
    default :
      break;
  }
}

void detachServo()
{
  myservoL.detach();
  myservoR.detach();
}

void attachServo()
{
  myservoL.attach(LEFT_SERVO_PIN);
  myservoR.attach(RIGHT_SERVO_PIN);
}
