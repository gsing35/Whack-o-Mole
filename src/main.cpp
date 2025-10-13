#include <Arduino.h>

const int touchPin = 2;   // Pin connected to the sound button
const int touchPinOctaveOne = 3;   // Pin connected to the octave button
const int touchPinOctaveTwo = 4;   // Pin connected to the octave button
const int touchPinOctaveThree = 5;   // Pin connected to the octave button
const int buzzerPin = 11;  // Pin connected to the buzzer
int octive = 1;  // Current octave (1, 2, or 3)
bool isOctiveChanged = false; // Flag to track octave button state

void setup() {
  pinMode(touchPinOctaveOne, INPUT);     // Octave button as input
  pinMode(touchPinOctaveTwo, INPUT);
  pinMode(touchPinOctaveThree, INPUT);
  pinMode(touchPin, INPUT);    // Sensor as input
  pinMode(buzzerPin, OUTPUT);  // Buzzer as output
  Serial.begin(115200);
}

void loop() {
  //If statement to change octave
 if (digitalRead(touchPinOctaveThree) == HIGH ) {
    if(!isOctiveChanged && octive < 3){ 
      octive++;
      isOctiveChanged = true;
      delay(200);
    }
    else {
      octive = 1; // Reset to first octave if it exceeds 3
      isOctiveChanged = true;
      delay(200);
    }
    isOctiveChanged = false;
    delay(200);
  } 
  /*else if (digitalRead(touchPinOctaveTwo) == LOW) {
    octive = 2;
    delay(200);
  }
  else if(digitalRead(touchPinOctaveThree) == LOW){ 
    octive = 3;
    delay(200);
  }
    */

  // Read if sound button has been pressed
  int touchState = digitalRead(touchPin);  

  // If the button is pressed, sound the buzzer 
  if (touchState == LOW) {  
    Serial.println("Touched!");
    if(octive == 1){
      tone(buzzerPin, 500);   
    } else if(octive == 2){
      tone(buzzerPin, 1000);  // Different sounds to play based on octave
    } else if(octive == 3){
      tone(buzzerPin, 5000); 
    }
    delay(200);              // Delay to avoid multiple triggers too quickly
    noTone(buzzerPin);       
  } else {
    noTone(buzzerPin);        // Ensure the buzzer is off
  }

  delay(50);  
}
