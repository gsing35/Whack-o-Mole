#include <Arduino.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>


Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscs[12] = { // Array of mozzi oscilators to play every note at same time
  {SIN2048_DATA}, {SIN2048_DATA}, {SIN2048_DATA},
  {SIN2048_DATA}, {SIN2048_DATA}, {SIN2048_DATA},
  {SIN2048_DATA}, {SIN2048_DATA}, {SIN2048_DATA},
  {SIN2048_DATA}, {SIN2048_DATA}, {SIN2048_DATA} };


const int pins[] = {22, 23, 26, 27, 30, 31, 34, 35, 38, 39, 42, 43}; // A–G#
const int octave_pin = 2;
const int buzzer_pin = 11;


unsigned long last_octave_time = 0;
int base_freqs[] = {
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
bool key_pressed[12] = {false};
int octave = 1;


void setup() {
  pinMode(octave_pin, INPUT_PULLUP);
  for (int i = 0; i < 12; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(115200);
  startMozzi();
}


void updateControl() {
    // Run every 250ms
    if (digitalRead(octave_pin) == HIGH && (millis() -last_octave_time) > 250) {
    octave = (octave == 1) ? 2 : 1;
    Serial.print("Switched to octave ");
    Serial.println(octave);
    last_octave_time = millis();
  }


  // Check if key pressed and stored which key
  for (int i = 0; i < 12; i++) {
    if (digitalRead(pins[i]) == HIGH) {  // key pressed
      oscs[i].setFreq(base_freqs[i] * octave);
      key_pressed[i] = true;
    } else {
      key_pressed[i] = false;
    }
  }
}


// Mix th audio
int updateAudio() {
  int mix_audio = 0;


  for (int i = 0; i < 12; i++) {
    if (key_pressed[i]) {
      mix_audio += oscs[i].next();
    }
  }


  return mix_audio >> 3;  // Right bit shift to avoid distored sounds
}


void loop() {
  audioHook();
}





