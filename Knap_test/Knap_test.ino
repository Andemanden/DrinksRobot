const int pin=17;
int selKnap=HIGH;
void setup() {
   Serial.begin(9600); //Opsætter bitrate aftalen
   pinMode(pin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  selKnap=analogRead(pin); //Vælge Knap pin
  Serial.print(selKnap);
  if (selKnap == LOW){
    Serial.print("TRYK!");
  }
  delay(500);
}
