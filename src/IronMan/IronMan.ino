#include <Arduino.h>
#include <Servo.h>
#include <AceButton.h>
using namespace ace_button;

Servo myservoL;
Servo myservoR;

// Pin setup
const int LEFT_SERVO_PIN= 5;
const int RIGHT_SERVO_PIN= 6;

const int B_OPEN_CLOSE_BUTTON = 2;
const int B_ON_OFF_LED_BUTTON= 3;
const int LED_PIN = 9;

const int LED_ON = 1;
const int LED_OFF = 0;

// Course setup
const int openLeftAngle= 45; // Angle could be tuned to open wider
const int openRightAngle= 180 - openLeftAngle;
const int closeAngle= 130; // Angle could be tuned to rich perfect close position

// After 450 ms detach servo
const unsigned long courseTime= 450;

AceButton buttonOpenClose(B_OPEN_CLOSE_BUTTON);
AceButton buttonOnOffLed(B_ON_OFF_LED_BUTTON);

// Initial state, helmet is closed and LED is ON
bool closed= true;
bool lightOn= true;

// Update state on button event
void buttonHandleEvent(AceButton*, uint8_t, uint8_t);

// Detach left and right servo
void detachServo();

// Attach left and right servo
void attachServo();

void setup() {
  delay(200);
 
  // Pin mode setup
  pinMode(B_OPEN_CLOSE_BUTTON, INPUT_PULLUP);
  pinMode(B_ON_OFF_LED_BUTTON, INPUT_PULLUP);               
  pinMode(LED_PIN, OUTPUT);

  // AceButton setup
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(buttonHandleEvent);
  
  bool on= true;

  // Add cool LED effect on power up
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
  // Save state to detect change
  const bool previousClosed= closed;
  const bool previousLightOn= lightOn;

  // Check if button are pressed
  buttonOpenClose.check();
  buttonOnOffLed.check();

  if (closed != previousClosed)
  { // Open or close helmet
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
  { // Tur LED ON or OFF
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
