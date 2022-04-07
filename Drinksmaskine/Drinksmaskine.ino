/* Drinksmaskine
   Robotteknologi*/
#include <Wire.h>
#include "rgb_lcd.h"
#include <AccelStepper.h> // Include the AccelStepper library:
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin1 2
#define stepPin1 3
#define motorInterfaceType 1
#define pumpePin1 2

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, 3, 2);
rgb_lcd lcd;

void setup(){
  Serial.begin(9600); //Opsætter bitrate aftalen
  pinMode(14, OUTPUT); 
  pinMode(15, INPUT);
   // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  //LCD start besked
  lcd.begin(16, 2);
  lcd.print("Drinksmaskine");
  delay(500);
  lcd.clear();
  lcd.print(" Robotteknologi");
  delay(500);
  
}

void loop(){ 
  //stepper.moveTo(12000);
  //stepper.moveTo(50);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  digitalWrite(pumpePdin1, HIGH);
  delay(1000);
  digitalWrite(pumpePin1, LOW);
  delay(1000);
  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();
  lcd.clear(); 
  lcd.print("> ");
  lcd.print("Martini"); //Viser elasticitetsmodul
  lcd.setCursor(0,1);
  lcd.print("Gin and Tonic");
  delay(400);
}
