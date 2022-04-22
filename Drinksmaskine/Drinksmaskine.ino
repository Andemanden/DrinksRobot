/* Drinksmaskine
   Robotteknologi*/
#include <Wire.h> //Inkludere LCD bibliotek
#include "rgb_lcd.h" //Inkludere LCD bibliotek
#include <AccelStepper.h> // Inkludere AccelStepper biblioteket
#define dirPin1 11 //Retnings pin
#define stepPin1 12 //Stepper pin
#define motorInterfaceType 1
const int selPin=16, scrPin=17, hallPin=9; //Pins
short int selOp=LOW, selKnap=LOW, op=0, hallState=0, fillAnim=3, fillAnimTom=3,
glasPos=0, stepsTotal=12000, retning=LOW, stepsFlyt=0, lok=10;  //Variable
String opskriftNavn[7]={"Gin & Tonic","Gin & Lemon","Lille Lyseroed",
"Filur","Screwdriver","Screwdriver Gin","Vodka Lemon"};
// Laver en ny AccelStepper klasse med biblioteket
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
rgb_lcd lcd;
byte drinkGlasTom[] = { //LCD pixel data til animation
  B10001,
  B10001,
  B10001,
  B10001,
  B01110,
  B00100,
  B00100,
  B11111
};

byte bogstav[] = { //Bogstavet æ
  B00000,
  B01010,
  B10101,
  B00101,
  B01111,
  B10100,
  B01111,
  B00000
};
void anim(){ /*---Animation---*/
   fillAnim=0;
   fillAnimTom=0;
   lcd.clear(); 
   for (fillAnim=3; fillAnim>-1;fillAnim-=1){
    drinkGlasTom[fillAnim]=B11111;
    lcd.createChar(0, drinkGlasTom);
    lcd.home();
    lcd.write(byte(0));
    delay(700);
    lcd.clear(); 
   }
   for (fillAnimTom=3; fillAnimTom>-1;fillAnimTom-=1){
    drinkGlasTom[fillAnimTom]=B10001;
   }
   lcd.createChar(0, drinkGlasTom);
   lcd.home();
   lcd.clear();
};

void pump(int vol, int pumpe){ /*---Ingrediens pumpning---*/
  digitalWrite(pumpe+2,HIGH);
  delay(vol*600); //Volumen af ingrediensen
  digitalWrite(pumpe+2,LOW);
  
  Serial.print("Pumpe: ");
  Serial.print(pumpe+2);
  Serial.print("Vol: ");
  Serial.print(vol); 
};
void ryst(int gange, int styrke){
  lcd.clear();
  lcd.print("     Ryster");
   for (int ryst=1; ryst<gange; ryst++){ //Stepper motor loop
    if ((ryst % 2) == 0) {
      digitalWrite(dirPin1, HIGH); //Vælger retningen med uret
    } else {
      digitalWrite(dirPin1, LOW); //Vælger retningen mod uret
    }
    for (int ryst2=1; ryst2<styrke; ryst2++){
      digitalWrite(stepPin1,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(500);
    }
    delay(100);
  } 
}

void kal(){ /*---Kalibrering---*/
  digitalWrite(dirPin1, HIGH); //Vælger retningen
  for (int ing=0; ing<13000; ing++){ //Stepper motor loop
    digitalWrite(stepPin1,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin1,LOW);
    delayMicroseconds(500);
    hallState=digitalRead(hallPin);
   
    if (hallState == 1){
      glasPos=0;
      hallState=0;
      break; 
    }
  }
  for (int ing=0; ing<5600; ing++){ //Stepper motor loop
    digitalWrite(stepPin1,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin1,LOW);
    delayMicroseconds(500);
  }
  glasPos=5600;
};
class Opskrift{ /*---Opskrift klassen---*/
    private: 
      int pumpelok[7]={7510,6110,4510,2950,1300,10,10610}; //Microstep lokalitet
      //Vodka, Gin, Sprite, Lemon Soda, Tonic, Rød Soda, Appelsinjuice
      //Opskrifter: Gin & Tonic, Gin & Lemon ,Lille Lyserød ,Filur ,Screwdriver
      //, Screwdriver & Gin, Vodka & Lemon
      int opskriftIng[7][7]={{0,2,0,0,13,0,0},{0,2,0,13,0,0,0},{0,2,7,0,0,6,0},
        {2,0,0,0,0,5,8},{5,0,0,0,0,0,10},{0,5,0,0,0,0,10},{2,0,0,13,0,0,0}};
    public:
    void lav(int op){ //Lav drinken
      anim(); //Animere drink glas
      for (int ing=0; ing<7; ing++){ 
        if (opskriftIng[op][ing]>0){
            Serial.print("Pumpelok: ");
            Serial.print(pumpelok[ing]);
            //stepper.moveTo(pumpelok[ing]);
            //stepper.runToPosition();

            steptil(pumpelok[ing],500);
            
            delay(2000);
            lcd.clear();
            lcd.print("Laver din drink");
            //kal(pumpelok[ing]); //Kalibrer mens steppermoteren kører
            pump(opskriftIng[op][ing],ing); //Pumper ingrediens
            lcd.clear(); 
            
            delay(5000);
        }
      }
      lcd.clear(); 
      lcd.print("    Rystet?");
      lcd.setCursor(0,1);
      lcd.print("Nej           Ja");
      for (int vent=0; vent<200000; vent++){ 
        selKnap=analogRead(selPin); //Vælge Knap pin
        selOp=analogRead(scrPin); //Scroller Knap pin
        if (selKnap == 1023){
          ryst(60,30);
          break;
        } else if (selOp==1023){
          break;
        }  
        delay(20);
      }
      lcd.clear();
      kal();
      
      Serial.print("FÆRDIG");
      Serial.print(glasPos);
      lcd.clear(); 
      lcd.print("Drink Er F");
      lcd.write(byte(1)); //Skriver æ
      lcd.print("rdig");
      delay(2000);
     }
};

void steptil(int pos, int delayet){/*---Stepper bevægelse---*/
  stepsFlyt=abs(pos-glasPos); //Finder step antallet
  if (pos-glasPos<0){
    stepsFlyt=((12000-glasPos)+pos);
  } 
  /*if (pos-glasPos>0){//Finder retningen
     if(((12000-pos)+glasPos)>(pos-glasPos)){
      retning=HIGH;
      stepsFlyt=(pos-glasPos);
     } else {
      retning=LOW;
      stepsFlyt=((12000+pos)+glasPos);
     }
    //retning=LOW;//Mod uret
  } else if (pos-glasPos<0){
     if (((12000-glasPos)+pos)>(glasPos-pos)){
      retning=HIGH;
      stepsFlyt=(glasPos-pos);
     } else {
      retning=LOW;
      stepsFlyt=((12000-glasPos)+pos);
     }
    //retning=HIGH; //Med uret
  }*/
  retning=HIGH; //Med uret
  //Serial.print(retning);
  digitalWrite(dirPin1, retning); //Vælger retningen
  for (int steps=0; steps<stepsFlyt; steps++){ //Stepper motor loop
    digitalWrite(stepPin1,HIGH);
    delayMicroseconds(delayet);
    digitalWrite(stepPin1,LOW);
    delayMicroseconds(delayet);
    //Serial.print(glasPos);
    if (retning==HIGH){ //Opdatere position
      glasPos+=1; //Forlæns
    } else {
    glasPos-=1; //Baglæns
    }
    if (hallState == 1){
      glasPos=0; 
      hallState=0;
    }
  }
  //Serial.print(stepsFlyt);
  //Serial.print(glasPos);
  /*if (retning==HIGH){ //Opdatere position
    glasPos+=stepsFlyt; //Forlæns
    Serial.print("HIGH");
  } else if(retning==LOW){
    glasPos-=stepsFlyt; //Baglæns
  }*/
  //Serial.print(glasPos);
  if (glasPos>stepsTotal){ //Omdanner omgang til position
    glasPos-=stepsTotal;
  } else if (glasPos<0){
    glasPos=stepsTotal-abs(glasPos);
  }
  /*if (glasPos<0){
    glasPos=stepsTotal-abs(glasPos);
  }*/
  stepsFlyt=0; //Til at der kan ses om moteren kører
};

void setup(){ /*---Køres en gang---*/
  Serial.begin(9600); //Opsætter bitrate aftalen
  pinMode(scrPin, INPUT);
  pinMode(selPin, INPUT);
  
  pinMode(hallPin, INPUT);
  for (int p=2; p<9; p++){
    pinMode(p, OUTPUT); //Pumpernes pins
  }
  //Angiver den maksimale hastighed og acceleration:
  stepper.setMaxSpeed(10000); //Kom ikke over 1500
  stepper.setAcceleration(1400); //Kom ikke over 600
  //stepper.moveTo(0);
  //stepper.runToPosition();
  //kal(0);
  //LCD start besked
  lcd.begin(16, 2);
  lcd.createChar(1, bogstav);
  lcd.createChar(0, drinkGlasTom);
  lcd.home(); 
  lcd.print(" Drinksmaskine");
  delay(500);
  lcd.setCursor(0,1);
  lcd.print(" Robotteknologi");
  delay(500);
  lcd.clear();
  //stepper.move(200000);
  //stepper.runToPosition();
  //kali(current);
 
  /*if (hallState == HIGH){
    stepper.stop();
  }*/
  lcd.print("  Kalibrering");
  lcd.setCursor(0,1);
  lcd.print("     Igang");
  //stepper.setMaxSpeed(10000);
  //stepper.setAcceleration(10000);
  /*for (int steps=0; steps<20000; steps++){
    stepper.move(60);
    stepper.runToPosition();
    kal(steps);
  }*/
  //-(129*stepsFlyt/stepsTotal)
  kal();
  glasPos=5600;
  Serial.print(glasPos);
  steptil(5600,500);
  lcd.clear();
  lcd.print("F");
  lcd.write(byte(1)); //Skriver æ
  lcd.print("rdig");
  delay(2000);
}

void loop(){ /*---Løkken---*/
  selKnap=analogRead(selPin); //Vælge Knap pin
  selOp=analogRead(scrPin); //Scroller Knap pin
  //Menuen på skærmen
  lcd.clear(); 
  lcd.print("V");
  lcd.write(byte(1));
  lcd.print("lg Drink: ");
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.print(">");
  lcd.print(opskriftNavn[op]);
  //Vælge knappen
  if (selKnap == 1023){
    Serial.print("SelKnap");
    lcd.clear();
    lcd.print("Laver Drink ");
    lcd.setCursor(0,1);
    lcd.print(opskriftNavn[op]);
    Opskrift Opskriften;
    Opskriften.lav(op);
  }
  //Scroller knappen
  if (selOp==1023){ //
    Serial.print("SelOp");
    if (op>5){op=0;} else {
    op++;}
  }

  delay(200); //Nødvendig til display
}
