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

void setup() {
  Serial.begin(9600);
  /* Start the motor */
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(200);  // 10 rpm
}

void loop() {


  // // Read the Slider Value
  globalSliderValue = analogRead(sliderPin);
  globalSliderCurrentValue = map(globalSliderValue,0,1023,-500,500);
  globalSliderDifference = globalSliderCurrentValue - globalSliderPreviousValue;
  //Serial.println(globalSliderDifference);

  if(abs(globalSliderDifference) > 2){
    if(wasTheAutoSpeedOn == 1){
      activateAutoSpeed = 0;
      autoSpeed(activateAutoSpeed);
      incomingByte=' ';
    }
    activateSlider = 1;
    slider(activateSlider);
    wasTheSliderOn = 1;
    globalSliderPreviousValue = globalSliderCurrentValue;
  }



  if (Serial.available() > 0){  // see if there's incoming serial data
    incomingByte = Serial.read(); // read it
  }

  // Activate the Slider
  if (incomingByte == 'H') {
    if(wasTheAutoSpeedOn == 1){
      activateAutoSpeed = 0;
      autoSpeed(activateAutoSpeed);
    }
    activateSlider = 1;
    slider(activateSlider);
    wasTheSliderOn = 1;
  }
  // Deactivate the Slider
  if (incomingByte == 'L'){
    activateSlider = 0;
    slider(activateSlider);
    incomingByte=' ';
  }

  // Activate the autospeed with 1
  if (incomingByte == 'A'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 1;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }

  // Activate the autospeed with 2
  if (incomingByte == 'S'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 2;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }

  // Activate the autospeed with 3
  if (incomingByte == 'D'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 3;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  // Activate the autospeed with 4
  if (incomingByte == 'F'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 4;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 1;
  }
  // Deactivate the autospeed
  if (incomingByte == 'B'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 0;
    autoSpeed(activateAutoSpeed);
    wasTheAutoSpeedOn = 0;
    incomingByte=' ';
  }

}

/* ----- Auto-Speed ----- */
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

/* ----- Slider ----- */
void slider(int iAmInCharge) {

  // Read the Slider Value
  sliderValue = analogRead(sliderPin);
  sliderCurrentValue = map(sliderValue,0,1023,-500,500);

  // If Slider is ON
  if(iAmInCharge == 1){

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
