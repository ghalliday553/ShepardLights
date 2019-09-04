#include "arduinoFFT.h"
#include <string.h>
#include <FastLED.h>
 
// Must be a power of 2
#define SAMPLES 64

#define SAMPLING_FREQUENCY 2560  

// Number of LEDS in light strip
#define NUM_LEDS 150

// Serial pin controlling WS2812B lights
#define DATA_PIN 53

arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;

// Arrays holding post-FFT bins
double vReal[SAMPLES] = { 0 };
double vImag[SAMPLES] = { 0 };

double *vRealPointer = vReal;
double *vImagPointer = vImag;

// Initialize other global variables
int lightState = 0;
String songState = "";
boolean active = false;
int sineFlag = 0;

CRGB leds[NUM_LEDS];

void setup() {
  // Using Serial2 so that debugging through USB does not write to nodeMCU
  Serial2.begin(115200);

  // Set light data pin to output
  pinMode(53, OUTPUT);

  // Initialize WS2812B Strip
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // Calculate sampling period
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}

void loop() {
  // Revert to max brightness
  FastLED.setBrightness(255);
  
  // Incoming data from nodeMCU regarding song selection
  if (Serial2.available() > 0 ) {
    
    String prevSongState = songState;
    songState = Serial2.readString();
    
    // Turn lights off if song selection has changed
    if (strcmp(prevSongState.c_str(), songState.c_str())) {
      off();
      lightState = 0;
    }
  }
  
  // Check for song selection
  // Display lights until new data is available in serial buffer from nodeMCU
  if (!strcmp(songState.c_str(), "off")) {
    off();
  } else if (!strcmp(songState.c_str(), "intro")) {
    CRGB lights [] = {CRGB::Indigo, CRGB::LightYellow};
    while(Serial2.available() < 1) {
      strobe(lights);
    }
  } else if (!strcmp(songState.c_str(), "flowers")) {
    CRGB lights [] = {CRGB::SaddleBrown, CRGB::HotPink};
    while(Serial2.available() < 1) {
      strobe(lights);
    }
  } else if (!strcmp(songState.c_str(), "reapers")) {
    CRGB lights [] = {CRGB::MediumPurple, CRGB::AntiqueWhite, CRGB:: DarkRed, CRGB::Gold};
    while(Serial2.available() < 1) {
      snake(lights);
    }
  } else if (!strcmp(songState.c_str(), "pick")) {
    int lights[] = {150, 40};
    while(Serial2.available() < 1) {
      sinePop(lights);
    }
  } else if (!strcmp(songState.c_str(), "passion")) {
    CRGB lights [] = {CRGB::IndianRed, CRGB::Navy};
    while(Serial2.available() < 1) {
      strobe(lights);
    }
  } else if (!strcmp(songState.c_str(), "lake")) {
    int lights [] = {80, 20};
    while(Serial2.available() < 1) {
      sine(lights);
    }
  } else if (!strcmp(songState.c_str(), "clouds")) {
    int lights[] = {200, 60};
    while(Serial2.available() < 1) {
      sinePop(lights);
    }
  } else if (!strcmp(songState.c_str(), "uncanadian")) {
    CRGB lights [] = {CRGB::PaleGreen, CRGB::OrangeRed, CRGB::NavajoWhite, CRGB::Tan};
    while(Serial2.available() < 1) {
      snake(lights);
    }
  } else if (!strcmp(songState.c_str(), "wildside")) {
    CRGB lights [] = {CRGB::Beige, CRGB::Honeydew};
    while(Serial2.available() < 1) {
      strobe(lights);
    }
  }
}

// Update frequency bins
void fft() {

  // Take sample ever sample period until SAMPLES samples have been taken
  for(int i=0; i<SAMPLES; i++) {
        microseconds = micros();    //Overflows after around 70 minutes
     
        *(vRealPointer + i) = analogRead(0);
        *(vImagPointer + i) = 0;
     
        while((micros() - microseconds) < sampling_period_us);
    }

  // Compute FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
}

// Determine if selected frequencies have met threshold
boolean determineActive() {
  boolean active = false;

  // bins contain frequencies in 40 Hz increments
  // Checks for frequencies between 80 and 160 Hz
  for (int i = 2; i<5; i++) {

    // Threshold of 1800 selected from trial and error
    // Reasonably isolates kick drum frequencies
    if (*(vRealPointer + i) >1800) {
      active = true;
    }
  }

  return active;
}

// No lights
void off() {
  fft();
  if (determineActive()) {
    lightState++;
  }
  for (int i=0; i<150; i++) {
      leds[i] = CRGB::Black;
    }
  FastLED.show();
}

// Changes between two solid colours
void strobe(CRGB lights []) {
  fft();
  if (determineActive()) {
    if (lightState == 0) {
      for (int i=0; i<150; i++) {
        leds[i] = lights[0];
      }
      FastLED.show();
      lightState = 1;
    } else {
      for (int i=0; i<150; i++) {
        leds[i] = lights[1];
      }
      FastLED.show();
      lightState = 0;
    }
  }
}

// Changes individual sections of the light strip intiially
// Strobes two solid colours afterwards
void snake(CRGB lights[]) {
  fft();
  if (determineActive()) {
    if (lightState == 0) {
      for (int i=0; i<25; i++) {
        leds[i] = lights[0];
      }
      FastLED.show();
      lightState = 1;
      
    } else if (lightState == 1) {
      for (int i=25; i<50; i++) {
        leds[i] = lights[1];
      }
      FastLED.show();
      lightState = 2;
      
    } else if (lightState == 2) {
      for (int i=50; i<75; i++) {
        leds[i] = lights[0];
      }
      FastLED.show();
      lightState = 3;
      
    } else if (lightState == 3) {
      for (int i=75; i<100; i++) {
        leds[i] = lights[1];
      }
      FastLED.show();
      lightState = 4;
  
    } else if (lightState == 4) {
      for (int i=100; i<125; i++) {
        leds[i] = lights[0];
      }
      FastLED.show();
      lightState = 5;
  
    } else if (lightState == 5) {
      for (int i=125; i<150; i++) {
        leds[i] = lights[1];
      }
      FastLED.show();
      lightState = 6;
  
    } else if (lightState == 6) {
      for (int i=0; i<150; i++) {
        leds[i] = lights[2];
      }
      FastLED.show();
      lightState = 7;
  
    } else if (lightState == 7) {
      for (int i=0; i<150; i++) {
        leds[i] = lights[3];
      }
      FastLED.show();
      lightState = 0;
    }
  }
}

// Creates a sine wave of colours
// Brightness is momentarily increased after kick-drum detection
void sinePop(int lights[]) {
  fft();
  
  if (determineActive() || sineFlag > 0) {
    FastLED.setBrightness(255);
    if (determineActive()) {
      sineFlag = 3;
    }
  } else {
    FastLED.setBrightness(80);
  }
  
  for (int i=0; i<150; i++) {
    leds[i].setRGB( lights[0], lights[1], quadwave8(lightState));
  }
  
  lightState = lightState+2;
  if (lightState == 258) {
    lightState = 0;
  }

  if (sineFlag>0) {
    sineFlag --;
  }

  FastLED.show();
}

// Creates static sine wave of colours
void sine(int lights[]) {
  for (int i=0; i<150; i++) {
    leds[i].setRGB( lights[0], lights[1], quadwave8(lightState));
  }
  
  lightState = lightState+2;
  if (lightState == 258) {
    lightState = 0;
  }

  FastLED.show();
}
