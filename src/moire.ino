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

void setup() {
  Serial.begin(9600);
  /* Start the motor */
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(200);  // 10 rpm
}

void loop() {

  if (Serial.available() > 0){  // see if there's incoming serial data
    incomingByte = Serial.read(); // read it
  }
  // Activate the Slider
  if (incomingByte == 'H') {
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

  }
  // Activate the autospeed with 2
  if (incomingByte == 'S'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 2;
    autoSpeed(activateAutoSpeed);
  }
  // Activate the autospeed with 3
  if (incomingByte == 'D'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 3;
    autoSpeed(activateAutoSpeed);
  }
  // Activate the autospeed with 4
  if (incomingByte == 'F'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 4;
    autoSpeed(activateAutoSpeed);
  }
  // Deactivate the autospeed
  if (incomingByte == 'B'){
    if(wasTheSliderOn == 1){
      activateSlider = 0;
      slider(activateSlider);
    }
    activateAutoSpeed = 0;
    autoSpeed(activateAutoSpeed);
    incomingByte=' ';
  }


  // Reset everything to 0 to avoid stepper going further the limits

  // Check who is going to controll the stepper. Default is Manual.
    // if the Slider is Moved
      // Turn ON the Slider, turn OFF the buttons and OFF the Distance
    // if any button is pressed
      // Turn ON the Buttons, turn OFF the Slider and OFF the Distance
    // if Distance Sensor is ON
      // Turn ON the Distance Sensor, turn OFF the Slider and OFF the Buttons

  // if Slider is ON and everything else is OFF, then Slider
    // Read slider position
      // Move stepper to match slider

  // if one of the Auto-Speed button is ON and everything else if OFF, then Auto-Speed
    // check which Auto-Speed
      // Move to the selected Auto-Speed

  // if Distance Sensor is ON and everything else if OFF, then Distance Sensor
    // Check Distance Sensor Value
      // Move stepper to match Sensor Value`


  /* Read the Slider Value */


}

// Element function accepts one boolean argument
  // if TRUE
    // Read and move stepper
  // if FALSE
    // Reset the Stepper to origin
    // do nothing

/* ----- Auto-Speed ----- */
void autoSpeed(int iAmInCharge){


  if(iAmInCharge == 0){
    // autoSpeedCurrentValue = 100;
    // myMotor->step(autoSpeedCurrentValue, BACKWARD, SINGLE); // Return to start
    // iAmInCharge = 9;
  }
  else if (iAmInCharge > 0 && iAmInCharge < 8){
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
