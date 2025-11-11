#include <Arduino.h>

const int pins[] = {22, 23, 26, 27, 30, 31, 34, 35, 38, 39, 42, 43}; // A–G#
const int touchPinOctave = 2;
const int buzzerPin = 11;
bool lastOctButton = false;
int baseFreqs[] = {
  440, // A
  466, // Ab
  494, // B
  523, // Bb
  554, // C
  587, // D
  622, // Dd
  659, // E
  698, // F
  740, // Fb
  784, // G
  831  // Gb
};
bool keyPressed[13] = {false};
int octave = 1;

void setup() {
  pinMode(touchPinOctave, INPUT_PULLUP);
  for (int i = 0; i < 12; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // Press = LOW
  if (digitalRead(touchPinOctave) == LOW && !lastOctButton) {
    octave = (octave == 1) ? 2 : 1;
    Serial.print("Switched to octave ");
    Serial.println(octave);
    lastOctButton = true;
    delay(200); // delay to prevent multi input from single touch
  } else if (digitalRead(touchPinOctave) == HIGH) {
    lastOctButton = false;
  }


  
  // Check all keys for press (LOW)
  for (int i = 0; i < 12; i++) {
    if (digitalRead(pins[i]) == LOW) {
      keyPressed[i] = true;
      int freq = baseFreqs[i] * octave;
      tone(buzzerPin, freq);
      Serial.print("Playing freq: ");
      Serial.println(freq);
      delay(200);
      noTone(buzzerPin);
    } else {
      keyPressed[i] = false;
    }
  }
    


  delay(50);
}

