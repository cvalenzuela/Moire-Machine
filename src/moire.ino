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
boolean activateSlider = false;


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
  if (incomingByte == 'H') {
    activateSlider = true;
    slider(activateSlider);
  }
  if (incomingByte == 'L'){
    activateSlider = false;
    slider(activateSlider);
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





/* ----- Slider ----- */
void slider(boolean iAmInCharge) {

  // Read the Slider Value
  sliderValue = analogRead(sliderPin);
  sliderCurrentValue = map(sliderValue,0,1023,-500,500);

  // If Slider is ON
  if(iAmInCharge){

    // Compare current and previous states
    sliderDifference = sliderCurrentValue - sliderPreviousValue;

    // Move the Slider depending on the position
    if(sliderDifference != 0 && iAmInCharge){
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
  if(!iAmInCharge){

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
    }

  }

}
