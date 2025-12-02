#include <Arduino.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#define CONTROL_RATE 64
#define NUM_VOICES 8   

const int pins[] = {22, 23, 26, 27, 30, 31, 34, 35, 38, 39, 42, 43};
const int touchPinOctave = 2;

bool lastOctButton = false;
int octave = 1;
bool keyPressed[12] = {false};

int baseFreqs[] = {
  440, // A
  466, // Bb
  494, // B
  523, // C
  554, // Dd
  587, // D
  622, // Eb
  659, // E
  698, // F
  740, // Gb
  784, // G
  831  // Ab
};

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> voices[NUM_VOICES] = {
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA)
};

float voiceFreq[NUM_VOICES] = {0};

void noteOn(float freq) {
  for (int i = 0; i < NUM_VOICES; i++) {
    if (voiceFreq[i] == 0) {
      voiceFreq[i] = freq;
      voices[i].setFreq(freq);
      return;
    }
  }
}

void noteOff(float freq) {
  for (int i = 0; i < NUM_VOICES; i++) {
    if (voiceFreq[i] == freq) {
      voiceFreq[i] = 0;
      voices[i].setFreq(0);
      return;
    }
  }
}

void updateControl() {

}

AudioOutput_t updateAudio() {
  int mixed = 0;

  for (int i = 0; i < NUM_VOICES; i++) {
    if (voiceFreq[i] > 0) {
      mixed += voices[i].next();
    }
  }

  mixed /= 1; 

  return MonoOutput::from8Bit(mixed);
}

void setup() {
  pinMode(touchPinOctave, INPUT_PULLUP);
  for (int i = 0; i < 12; i++) pinMode(pins[i], INPUT_PULLUP);

  Serial.begin(115200);

  startMozzi(CONTROL_RATE);
}

void loop() {
  if (digitalRead(touchPinOctave) == LOW && !lastOctButton) {
    octave = (octave == 1) ? 2 : 1;
    Serial.print("Switched to octave ");
    Serial.println(octave);
    lastOctButton = true;
  } else if (digitalRead(touchPinOctave) == HIGH) {
    lastOctButton = false;
  }

  for (int i = 0; i < 12; i++) {
    int freq = baseFreqs[i] * octave;

    if (digitalRead(pins[i]) == LOW) {
      if (!keyPressed[i]) {
        keyPressed[i] = true;
        noteOn(freq);
        Serial.print("Note ON: ");
        Serial.println(freq);
      }
    } else {
      if (keyPressed[i]) {
        keyPressed[i] = false;
        noteOff(freq);
        Serial.print("Note OFF: ");
        Serial.println(freq);
      }
    }
  }

  audioHook();
}
