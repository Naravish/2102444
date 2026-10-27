#include <AccelStepper.h>
#include <Bounce2.h>

#define MOTOR_INTERFACE_TYPE 4
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;
const int PIN_IN3 = 4;
const int PIN_IN4 = 5;

AccelStepper stepper(MOTOR_INTERFACE_TYPE, PIN_IN1, PIN_IN3, PIN_IN2, PIN_IN4);

const int PIN_BTN_1 = 8;
const int PIN_BTN_2 = 9;
const int PIN_BTN_3 = 10;
const int PIN_BTN_4 = 11;

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce();

unsigned long btn1PressTime = 0;
bool btn1LongPressHandled = false;

unsigned long btn4PressTime = 0;
bool btn4LongPressHandled = false;

const unsigned long LONG_PRESS_TIME = 800; 

const int STEPS_PER_90_DEG = 512; 
const float CONT_SPEED = 500.0;   

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(PIN_BTN_1, INPUT_PULLUP);
  pinMode(PIN_BTN_2, INPUT_PULLUP);
  pinMode(PIN_BTN_3, INPUT_PULLUP);
  pinMode(PIN_BTN_4, INPUT_PULLUP);

  debouncer1.attach(PIN_BTN_1);
  debouncer1.interval(25);
  debouncer2.attach(PIN_BTN_2);
  debouncer2.interval(25);
  debouncer3.attach(PIN_BTN_3);
  debouncer3.interval(25);
  debouncer4.attach(PIN_BTN_4);
  debouncer4.interval(25);

  stepper.setMaxSpeed(500.0);
  stepper.setAcceleration(500.0);
}

void loop() {
  
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();

  //B2
  if (debouncer2.read() == LOW) {
    stepper.setSpeed(-CONT_SPEED);
    stepper.runSpeed();
    
    stepper.setCurrentPosition(stepper.currentPosition());
    return; 
  }

  //B3
  if (debouncer3.read() == LOW) {
    stepper.setSpeed(CONT_SPEED);
    stepper.runSpeed();
    
    stepper.setCurrentPosition(stepper.currentPosition());
    return; 
  }

  if (debouncer1.fell()) {
    btn1PressTime = millis();
    btn1LongPressHandled = false;
  }

  //B1 Long Press
  if (debouncer1.read() == LOW) {
    if (!btn1LongPressHandled && (millis() - btn1PressTime >= LONG_PRESS_TIME)) {
      stepper.setCurrentPosition(0);
      Serial.println("Button 1 Long Press: Home position set to 0.");
      btn1LongPressHandled = true;

      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
      }
    }
  }

  //B1 Short Press
  if (debouncer1.rose()) {
    if (!btn1LongPressHandled && (millis() - btn1PressTime < LONG_PRESS_TIME)) {
      stepper.move(-STEPS_PER_90_DEG);
      Serial.println("Button 1 Short Press: Moving +90 degrees.");
    }
  }

  if (debouncer4.fell()) {
    btn4PressTime = millis();
    btn4LongPressHandled = false;
  }

  //B4 Long Press
  if (debouncer4.read() == LOW) {
    if (!btn4LongPressHandled && (millis() - btn4PressTime >= LONG_PRESS_TIME)) {
      stepper.moveTo(0);
      Serial.println("Button 4 Long Press: Returning to home (0).");
      btn4LongPressHandled = true;
    }
  }

  //B4 Short Press
  if (debouncer4.rose()) {
    if (!btn4LongPressHandled && (millis() - btn4PressTime < LONG_PRESS_TIME)) {
      stepper.move(STEPS_PER_90_DEG);
      Serial.println("Button 4 Short Press: Moving -90 degrees.");
    }
  }

  stepper.run();
}