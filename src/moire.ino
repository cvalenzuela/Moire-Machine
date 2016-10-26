/*
==============================
Moire Machine: An interactive Visualization
by Cristóbal Valenzuela & Roxanne Kim
Physical Computing Midterm
October 2016
==============================
*/

/* Libraries */
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

/* Objects */
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2); //Connect a stepper motor with 200 steps per revolution (1.8 degree) to motor port #2 (M3 and M4)

/* Variables */
int sliderPin = A2;
int sliderValue = 0;
int sliderCurrentValue = 0;
int sliderPreviousValue = 0;
int sliderDifference = 0;
int incomingByte = 0;
int activateSlider = 9;
int activateAutoSpeed = 9;
int autoSpeedCurrentValue = 0;
int wasTheSliderOn = 0;
int wasTheAutoSpeedOn = 0;

int globalSliderValue = 0;
int globalSliderCurrentValue = 0;
int globalSliderPreviousValue = 0;
int globalSliderDifference = 0;

/* LEDS */
int pinLedSpeedOne = 10;
int pinLedSpeedTwo = 9;
int pinLedSpeedThree = 8;
int pinLedSpeedFour = 7;
int pinLedSlider = 6;

/* Buttons */
int pinButtonSpeedOne = 3;
int pinButtonSpeedTwo = 2;
int pinButtonSpeedThree = 1;
int pinButtonSpeedFour = 0;

int currentValueSpeedOne = 0;
int buttonValueSpeedTwo = 0;
int buttonValueSpeedThree = 0;
int buttonValueSpeedFour = 0;

int previousValueSpeedOne = 0;

int stateValueOne;

int speedOneisInCharge = 9;

void setup() {
  Serial.begin(9600);

  /* Start the motor */
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(200);  // 10 rpm

  /* LED Setup */
  pinMode(pinLedSpeedOne, OUTPUT);
  pinMode(pinLedSpeedTwo, OUTPUT);
  pinMode(pinLedSpeedThree, OUTPUT);
  pinMode(pinLedSpeedFour, OUTPUT);
  pinMode(pinLedSlider,OUTPUT);

  /* Button Setuo */
  pinMode(pinButtonSpeedOne,INPUT);
  // more buttons here
}

void loop() {


  /* ----- Read the Slider Value -----*/
  globalSliderValue = analogRead(sliderPin);
  globalSliderCurrentValue = map(globalSliderValue,0,1023,-500,500);
  globalSliderDifference = globalSliderCurrentValue - globalSliderPreviousValue;
  /* ----- Read the Slider Value -----*/

  /* ----- Check if the Slider is moved -----*/
  if(abs(globalSliderDifference) > 2){
    if(wasTheAutoSpeedOn == 1){
      activateAutoSpeed = 0;
      autoSpeed(activateAutoSpeed);
      incomingByte=' ';
    }

    stateValueOne = LOW;

    activateSlider = 1;
    slider(activateSlider);
    wasTheSliderOn = 1;
    globalSliderPreviousValue = globalSliderCurrentValue;
  }
  /* ----- Check if the Slider is moved -----*/


  /* ----- Read Serial Data -----*/
  if (Serial.available() > 0){  // see if there's incoming serial data
    incomingByte = Serial.read(); // read it
  }
  /* ----- Read Serial Data -----*/

  /* ----- Read the Buttons -----*/
  currentValueSpeedOne = digitalRead(pinButtonSpeedOne);

  if(currentValueSpeedOne == HIGH && previousValueSpeedOne == LOW){
    if(stateValueOne == HIGH){
      stateValueOne = LOW;

    }
    else{
      if(wasTheSliderOn == 1){
        activateSlider = 0;
        slider(activateSlider);
        digitalWrite(pinLedSlider, LOW);
      }
      stateValueOne = HIGH;

    }
    delay(50);
  }

  digitalWrite(pinLedSpeedOne, stateValueOne);
  previousValueSpeedOne = currentValueSpeedOne;
  /* ----- Read the Buttons -----*/

  /* ----- Activate the Slider -----*/
  if (incomingByte == 'H') {
    if(wasTheAutoSpeedOn == 1){
      activateAutoSpeed = 0;
      autoSpeed(activateAutoSpeed);
    }
    digitalWrite(pinLedSpeedOne, LOW);
    digitalWrite(pinLedSpeedTwo, LOW);
    digitalWrite(pinLedSpeedThree, LOW);
    digitalWrite(pinLedSpeedFour, LOW);

    activateSlider = 1;
    slider(activateSlider);
    wasTheSliderOn = 1;
  }
  /* ----- Activate the Slider -----*/

  /* -----  Deactivate the Slider -----*/
  if (incomingByte == 'L'){
    activateSlider = 0;
    slider(activateSlider);
    incomingByte=' ';
  }
  /* -----  Deactivate the Slider -----*/

  /* ----- Autospeed with 1 -----*/
  if (incomingByte == 'A' || stateValueOne == HIGH ){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }

    digitalWrite(pinLedSpeedOne, HIGH);
    digitalWrite(pinLedSpeedTwo, LOW);
    digitalWrite(pinLedSpeedThree, LOW);
    digitalWrite(pinLedSpeedFour, LOW);

    activateAutoSpeed = 1;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 1 -----*/

  /* ----- Autospeed with 2 -----*/
  if (incomingByte == 'S'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }

    digitalWrite(pinLedSpeedOne, LOW);
    digitalWrite(pinLedSpeedTwo, HIGH);
    digitalWrite(pinLedSpeedThree, LOW);
    digitalWrite(pinLedSpeedFour, LOW);

    activateAutoSpeed = 2;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 2 -----*/

  /* ----- Autospeed with 3 -----*/
  if (incomingByte == 'D'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }

    digitalWrite(pinLedSpeedOne, LOW);
    digitalWrite(pinLedSpeedTwo, LOW);
    digitalWrite(pinLedSpeedThree, HIGH);
    digitalWrite(pinLedSpeedFour, LOW);

    activateAutoSpeed = 3;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 3 -----*/

  /* ----- Autospeed with 4 -----*/
  if (incomingByte == 'F'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }

    digitalWrite(pinLedSpeedOne, LOW);
    digitalWrite(pinLedSpeedTwo, LOW);
    digitalWrite(pinLedSpeedThree, LOW);
    digitalWrite(pinLedSpeedFour, HIGH);

    activateAutoSpeed = 4;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 4 -----*/

  /* ----- Deactivate the autospeed -----*/
  if (incomingByte == 'B'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    digitalWrite(pinLedSpeedOne, LOW);
    digitalWrite(pinLedSpeedTwo, LOW);
    digitalWrite(pinLedSpeedThree, LOW);
    digitalWrite(pinLedSpeedFour, LOW);
    activateAutoSpeed = 0;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 0;
    stateValueOne = LOW;
    incomingByte=' ';
  }
  /* ----- Deactivate the autospeed -----*/

  delay(10);
}

/* ====== Auto-Speed ====== */
void autoSpeed(int iAmInCharge){
  if(iAmInCharge == 0){
    wasTheAutoSpeedOn = 0;
  }
  else if (iAmInCharge >= 1){
    autoSpeedCurrentValue = iAmInCharge*100;
    myMotor->step(autoSpeedCurrentValue, FORWARD, SINGLE);
    myMotor->step(autoSpeedCurrentValue, BACKWARD, SINGLE);
    myMotor->step(autoSpeedCurrentValue, BACKWARD, SINGLE);
    myMotor->step(autoSpeedCurrentValue, FORWARD, SINGLE);
  }
}
/* ====== Auto-Speed ====== */


/* ====== Slider ====== */
void slider(int iAmInCharge) {

  // Read the Slider Value
  sliderValue = analogRead(sliderPin);
  sliderCurrentValue = map(sliderValue,0,1023,-500,500);

  // If Slider is ON
  if(iAmInCharge == 1){
    //Turn ON the LED
    digitalWrite(pinLedSlider, HIGH);

    // Compare current and previous states
    sliderDifference = sliderCurrentValue - sliderPreviousValue;

    // Move the Slider depending on the position
    if(sliderDifference != 0 && iAmInCharge == 1){
       if(sliderDifference >= 1){
          myMotor->step(abs(sliderDifference), FORWARD, SINGLE);
        }
        else if(sliderDifference < 0){
          myMotor->step(abs(sliderDifference), BACKWARD, SINGLE);
        }
        sliderPreviousValue = sliderCurrentValue;
    }

  }

  // If Slider is OFF
  else if(iAmInCharge == 0){
    //Turn OFF the LED
    digitalWrite(pinLedSlider, LOW);

    sliderDifference = -sliderCurrentValue;

    // Reset the Stepper Position
    if(sliderDifference != 0){ // Just run this one once, so check the previous value and then change it
      if (sliderDifference >= 1){
        myMotor->step(abs(sliderDifference), FORWARD, SINGLE);
      }
      else if (sliderDifference < 0){
        myMotor->step(abs(sliderDifference), BACKWARD, SINGLE);
      }
      sliderValue = 0;
      sliderCurrentValue = 0;
      sliderPreviousValue = 0;
      sliderDifference = 0;
      wasTheSliderOn = 0;
    }
  }
}
/* ====== Slider ====== */
