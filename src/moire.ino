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
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1); //Connect a stepper motor with 200 steps per revolution (1.8 degree) to motor port #1 (M1 and M2)

/* Variables */
int sliderPin = A2;
int sliderValue = 0;
int sliderCurrentValue = 0;
int sliderPreviousValue = 0;
int sliderDifference = 0;
int incomingByte = ' ';
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
int pinLedSpeedOne = 5;
int pinLedSpeedTwo = 6;
int pinLedSpeedThree = 7;
int pinLedSpeedFour = 8;
int pinLedSlider = 9;

/* Buttons */
int pinButtonSpeedOne = 10;
int pinButtonSpeedTwo = 11;
int pinButtonSpeedThree = 12;
int pinButtonSpeedFour = 13;

int currentValueSpeedOne = 0;
int currentValueSpeedTwo = 0;
int currentValueSpeedThree = 0;
int currentValueSpeedFour = 0;

int previousValueSpeedOne = LOW;
int previousValueSpeedTwo = LOW;
int previousValueSpeedThree = LOW;
int previousValueSpeedFour = LOW;

int stateValueOne = LOW;
int stateValueTwo = LOW;
int stateValueThree = LOW;
int stateValueFour = LOW;

void setup() {
  Serial.begin(9600);

  /* Start the motor */
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(5);  // 10 rpm

  /* LED Setup */
  pinMode(pinLedSpeedOne, OUTPUT);
  pinMode(pinLedSpeedTwo, OUTPUT);
  pinMode(pinLedSpeedThree, OUTPUT);
  pinMode(pinLedSpeedFour, OUTPUT);
  pinMode(pinLedSlider,OUTPUT);

  /* Button Setuo */
  pinMode(pinButtonSpeedOne,INPUT);
  pinMode(pinButtonSpeedTwo,INPUT);
  pinMode(pinButtonSpeedThree,INPUT);
  pinMode(pinButtonSpeedFour,INPUT);

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

      // Set to LOW all Auto Speed LED
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = LOW;
      delay(50);
    }

    activateSlider = 1;
    slider(activateSlider);
    wasTheSliderOn = 1;
    globalSliderPreviousValue = globalSliderCurrentValue;
    delay(10);
  }
  /* ----- Check if the Slider is moved -----*/


  /* ----- Read Serial Data -----*/
  if (Serial.available() > 0){  // see if there's incoming serial data
    incomingByte = Serial.read(); // read it
  }
  /* ----- Read Serial Data -----*/

  /* ----- Read the Buttons -----*/
  currentValueSpeedOne = digitalRead(pinButtonSpeedOne);
  currentValueSpeedTwo = digitalRead(pinButtonSpeedTwo);
  currentValueSpeedThree = digitalRead(pinButtonSpeedThree);
  currentValueSpeedFour = digitalRead(pinButtonSpeedFour);

  // Check the first button
  if(currentValueSpeedOne == HIGH && previousValueSpeedOne == LOW){
    if(stateValueOne == HIGH){
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = LOW;
    }
    else{
      if(wasTheSliderOn == 1){
        activateSlider = 0;
        slider(activateSlider);
        digitalWrite(pinLedSlider, LOW);
      }
      stateValueOne = HIGH;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = LOW;
      delay(30);
    }
    delay(50);
  }

  // Check the second button
  if(currentValueSpeedTwo == HIGH && previousValueSpeedTwo == LOW){
    if(stateValueTwo == HIGH){
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = LOW;
    }
    else{
      if(wasTheSliderOn == 1){
        activateSlider = 0;
        slider(activateSlider);
        digitalWrite(pinLedSlider, LOW);
      }
      stateValueOne = LOW;
      stateValueTwo = HIGH;
      stateValueThree = LOW;
      stateValueFour = LOW;
      delay(30);
    }
    delay(50);
  }

  // Check the third button
  if(currentValueSpeedThree == HIGH && previousValueSpeedThree == LOW){
    if(stateValueThree == HIGH){
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = LOW;
    }
    else{
      if(wasTheSliderOn == 1){
        activateSlider = 0;
        slider(activateSlider);
        digitalWrite(pinLedSlider, LOW);
      }
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = HIGH;
      stateValueFour = LOW;
      delay(30);
    }
    delay(50);
  }

  // Check the fourth button
  if(currentValueSpeedFour == HIGH && previousValueSpeedFour == LOW){
    if(stateValueFour == HIGH){
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = LOW;
    }
    else{
      if(wasTheSliderOn == 1){
        activateSlider = 0;
        slider(activateSlider);
        digitalWrite(pinLedSlider, LOW);
      }
      stateValueOne = LOW;
      stateValueTwo = LOW;
      stateValueThree = LOW;
      stateValueFour = HIGH;
      delay(30);
    }
    delay(50);
  }

  /* ----- Read the Buttons -----*/

  /* ----- Activate the Slider -----*/
  if (incomingByte == 'H') {
    if(wasTheAutoSpeedOn == 1){
      activateAutoSpeed = 0;
      autoSpeed(activateAutoSpeed);
    }

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
    delay(5);

    activateAutoSpeed = 1;
    myMotor->setSpeed(5);
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 1 -----*/

  /* ----- Autospeed with 2 -----*/
  if (incomingByte == 'S' || stateValueTwo == HIGH){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }
    delay(5);

    activateAutoSpeed = 1;
    myMotor->setSpeed(8);
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 2 -----*/

  /* ----- Autospeed with 3 -----*/
  if (incomingByte == 'D' || stateValueThree == HIGH){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }
    delay(5);

    activateAutoSpeed = 1;
    myMotor->setSpeed(10);
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  /* ----- Autospeed with 3 -----*/

  /* ----- Autospeed with 4 -----*/
  if (incomingByte == 'F' || stateValueFour == HIGH){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
      digitalWrite(pinLedSlider, LOW);
    }
    delay(5);

    activateAutoSpeed = 1;
    myMotor->setSpeed(15);
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

    activateAutoSpeed = 0;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 0;
    stateValueOne = LOW;
    stateValueTwo = LOW;
    stateValueThree = LOW;
    stateValueFour = LOW;
    incomingByte=' ';
    delay(50);
  }
  /* ----- Deactivate the autospeed -----*/

  Serial.print("ONE: ");
  Serial.print(stateValueOne);
  Serial.print("//");
  Serial.print("TWO: ");
  Serial.print(stateValueTwo);
  Serial.print("//");
  Serial.print("THREE: ");
  Serial.print(stateValueThree);
  Serial.print("//");
  Serial.print("FOUR: ");
  Serial.println(stateValueFour);

  // Write the LED state
  digitalWrite(pinLedSpeedOne, stateValueOne);
  digitalWrite(pinLedSpeedTwo, stateValueTwo);
  digitalWrite(pinLedSpeedThree, stateValueThree);
  digitalWrite(pinLedSpeedFour, stateValueFour);

  // Change the previous Button state
  previousValueSpeedOne = currentValueSpeedOne;
  previousValueSpeedTwo = currentValueSpeedTwo;
  previousValueSpeedThree = currentValueSpeedThree;
  previousValueSpeedFour = currentValueSpeedFour;

  delay(1);
}

/* ====== Auto-Speed ====== */
void autoSpeed(int iAmInCharge){
  if(iAmInCharge == 0){
    wasTheAutoSpeedOn = 0;
  }
  else if (iAmInCharge >= 1){
    autoSpeedCurrentValue = iAmInCharge*40;
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
  sliderCurrentValue = map(sliderValue,0,1023,500,-500);

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
