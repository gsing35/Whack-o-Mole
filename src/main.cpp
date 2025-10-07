#include <Arduino.h>

const int touchPin = 2;   // Pin connected to the touch sensor
const int touchPinOctave = 3;   // Pin connected to the touch sensor for octave
const int buzzerPin = 11;  // Pin connected to the buzzer
int octive = 1;
bool isOctiveChanged = false;

void setup() {
  pinMode(touchPinOctave, INPUT);       
  pinMode(touchPin, INPUT);    // Sensor as input
  pinMode(buzzerPin, OUTPUT);  // Buzzer as output
  Serial.begin(9600);
}

void loop() {
 if (digitalRead(touchPinOctave) == HIGH && octive < 3 && !isOctiveChanged) {
    octive += 1;
    delay(200);
    isOctiveChanged = true;
  } else if (digitalRead(touchPinOctave) == LOW) {
    isOctiveChanged = false;
  }
  else if(digitalRead(touchPinOctave) == HIGH && octive >= 3 && !isOctiveChanged){ 
    octive = 1;
    delay(200);
    isOctiveChanged = true;
  }

  int touchState = digitalRead(touchPin);  

  if (touchState == HIGH) {  
    Serial.println("Touched!");
    if(octive == 1){
      tone(buzzerPin, 100);   
    } else if(octive == 2){
      tone(buzzerPin, 1000);  
    } else if(octive == 3){
      tone(buzzerPin, 10000); 
    }
    delay(200);              
    noTone(buzzerPin);       
  } else {
    noTone(buzzerPin);       
  }

  delay(50);  
}
