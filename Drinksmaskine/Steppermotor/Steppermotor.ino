/* Example sketch to control a stepper motor with TB6600 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <AccelStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 11
#define stepPin 12
#define motorInterfaceType 1


// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(9000);
  stepper.setAcceleration(9000);
}

void loop() {
  //digitalWrite(2, HIGH);
  //delay(1000);
  //digitalWrite(2, LOW);
  // Set the target position:
  stepper.moveTo(12000);
  
    //stepper.moveTo(50);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  stepper.moveTo(6000);
  stepper.runToPosition();
  stepper.moveTo(1000);
  stepper.runToPosition();
  stepper.moveTo(12000);
  // Move back to zero:
  stepper.runToPosition();
  stepper.moveTo(5000);
  stepper.runToPosition();
  stepper.moveTo(7000);
  stepper.runToPosition();
  stepper.moveTo(0);
  

  
}
