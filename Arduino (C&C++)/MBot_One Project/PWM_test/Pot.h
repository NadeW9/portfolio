const int potPin = 15;
const int numReadings = 10;
int potValue = 0;
int lastPotValue;
int readIndex = 0;
int total = 0;
int average = 0;

void getPotValue() {
  lastPotValue = average;
  total = total - potValue;
  potValue = analogRead(potPin);
  total = total + potValue;
  readIndex = readIndex + 1;
  
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  
  average = total / numReadings;
  
  if(abs(lastPotValue - average) >= 10){
    Serial.print("POT:   ");
    Serial.println(average);
  }
  
  delay(1);
}
