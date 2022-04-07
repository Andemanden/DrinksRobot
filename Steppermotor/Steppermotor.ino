/* Example sketch to control a stepper motor with TB6600 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <AccelStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define pumpePin1 4

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
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
  delay(1000);

  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();

  delay(1000);
}
