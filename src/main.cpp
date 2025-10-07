#include <Arduino.h>

const int touchPin = 2;   // Pin connected to the sound button
const int touchPinOctave = 3;   // Pin connected to the button for octave
const int buzzerPin = 11;  // Pin connected to the buzzer
int octive = 1;    // Current octave (1-3)
bool isOctiveChanged = false; // Flag to track octave button state

void setup() {
  pinMode(touchPinOctave, INPUT);  // Octave button as input 
  pinMode(touchPin, INPUT);    // Button as input
  pinMode(buzzerPin, OUTPUT);  // Buzzer as output
  Serial.begin(9600);
}

void loop() {
  // If statement to change octave
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

  // Read if sound button has been pressed
  int touchState = digitalRead(touchPin);  

  // If the button is pressed, sound the buzzer 
  if (touchState == HIGH) {  
    Serial.println("Touched!");
    if(octive == 1){
      tone(buzzerPin, 100);   
    } else if(octive == 2){
      tone(buzzerPin, 1000);    // Different sounds to play based on octave
    } else if(octive == 3){
      tone(buzzerPin, 10000); 
    }
    delay(200);              // Delay to avoid multiple triggers too quickly
    noTone(buzzerPin);       
  } else {
    noTone(buzzerPin);        // Ensure the buzzer is off
  }

  delay(50);  
}
