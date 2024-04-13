
const int selPin=16, scrPin=17;
int selOp=LOW, selKnap=LOW;

void pump(int pumpe, int vol){
  Serial.print("Pumper: ");
  Serial.print(pumpe);
  digitalWrite(pumpe,HIGH);
  delay(600*vol); 
  digitalWrite(pumpe,LOW);
  delay(1000);
}

void setup() {
  Serial.begin(9600); //Opsætter bitrate aftalen
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(selKnap, INPUT);
  pinMode(selOp, INPUT);
  Serial.print("Starter test: ");
}

void loop() {
  selKnap=analogRead(selPin);
  selOp=analogRead(scrPin);
  //pump(2);
  //pump(3);
  if (selOp==1023){ 
    pump(4,1);
  }
  
  if (selKnap == 1023){
   pump(5,1); 
  }
  //pump(6);
  //pump(7);
  //pump(8);
}
