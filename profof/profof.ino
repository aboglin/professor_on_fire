/*
 * Goertzel algo: Jacob Rosenthal: https://github.com/jacobrosenthal/Goertzel
 * Neopixels: Adafruit
 * Guidance: Todd, Park, Jay Zuerdorfer, and Scott Gililland
 * Created by Ceara Byrne
 * Pulsing was adapted from: https://myriadwhimsies.wordpress.com/2011/03/20/i-love-you-like-nature-loves-sine-waves/
*/

#include <Goertzel.h>
#include <Adafruit_NeoPixel.h>

// Microphone
int sensorPin = A5;
const float TARGET_FREQUENCY = 700; 
const int N = 100;  
const float SAMPLING_FREQUENCY = 8900; 



// For Clint: This is the threshold for the clapping
const float THRESHOLD = 20;  

//
int mode = 0;

// Sets of 3
#define PIN1    6
#define PIN2    9

// Individual Jewels
#define PIN3    10
#define PIN4    11
#define PIN5    12
#define PIN6    13

// Sets of 3
#define N2_LEDS 21

// Individual Jewels
#define N_LEDS 7


//bigger = faster
float timeAdditionValueAmb = 0.0005;
float timeAdditionValueApp = 0.005;
float tempSinVal;
float timeAroundLoop;
int const minBrightness = 10;
int const maxBrightness = 80;
float currentFadeAmount[21];
int currentBrightness[21];


#define FRAME 20000ul
#define PERIOD 1000000ul

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(N2_LEDS, PIN1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N2_LEDS, PIN2, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(N_LEDS, PIN3, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(N_LEDS, PIN4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(N_LEDS, PIN5, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(N_LEDS, PIN6, NEO_GRBW + NEO_KHZ800);



Goertzel goertzel = Goertzel(TARGET_FREQUENCY, N, SAMPLING_FREQUENCY);

void setup() {

  goertzel.sample(sensorPin); //Will take n samples
  
  float magnitude = goertzel.detect();  //check them for target_freq
  
  // Buttons
  Serial.begin(9600);
//  pinMode(led, OUTPUT); 
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);

  // NeoPixels
//  strip1.setBrightness(BRIGHTNESS);
  strip1.begin();
  strip1.show();
//  strip2.setBrightness(BRIGHTNESS);
  strip2.begin();
  strip2.show();
//  strip3.setBrightness(BRIGHTNESS);
  strip3.begin();
  strip3.show();
//  strip4.setBrightness(BRIGHTNESS);
  strip4.begin();
  strip4.show();
//  strip5.setBrightness(BRIGHTNESS);
  strip5.begin();
  strip5.show();
//  strip6.setBrightness(BRIGHTNESS);
  strip6.begin();
  strip6.show();
  for (int i = 0; i < strip1.numPixels(); i++) {
    currentFadeAmount[i] = random(1.0,15.0);
  }
}

void loop() {

  // Buttons
  int sensorValAppl = digitalRead(24);
  int sensorValAmb = digitalRead(23);
  int sensorValOff = digitalRead(22);
  
  if (sensorValAppl == LOW) {
    Serial.println("Applause clicked");
    
    mode = 1;
  }
  
  if (sensorValAmb == LOW) {
    Serial.println("Ambient clicked");
   
    mode = 2;
  }
  
  if (sensorValOff == LOW) {
    Serial.println("Off clicked");
    mode = 3;
  }

  if (mode == 1) {
    Serial.println("Applause");
    applause();
  } else if (mode == 2) {
     beAmbient();
  } else if (mode == 3) {
    Serial.println("Off");
    setOff();
  } else {
    Serial.println("Default");
  }
}

void applause() {

  goertzel.sample(sensorPin); //Will take n samples
  int magnitude = goertzel.detect();
//  magnitude = 200;
  //timeAdditionValue = 8
  if (magnitude>THRESHOLD) {
 
    timeAroundLoop += timeAdditionValueApp * (magnitude / 20);
    strip1.setBrightness(magnitude / 1.5);
    strip2.setBrightness(magnitude / 1.5);
    strip3.setBrightness(magnitude / 1.5);
    strip4.setBrightness(magnitude / 1.5);
    strip5.setBrightness(magnitude / 1.5);
    strip6.setBrightness(magnitude / 1.5);
  }
  else {
    timeAroundLoop = timeAroundLoop+timeAdditionValueApp;
    strip1.setBrightness(25);
    strip2.setBrightness(25);
    strip3.setBrightness(25);
    strip4.setBrightness(25);
    strip5.setBrightness(25);
    strip6.setBrightness(25);
  }
  for(uint16_t i=0; i<21; i++) {
    //current fade amount
    tempSinVal = timeAroundLoop * currentFadeAmount[i];
  
  //  
    tempSinVal = sin(tempSinVal);
    tempSinVal = abs(tempSinVal);
    tempSinVal = 1 - tempSinVal;
    int brightness = tempSinVal * maxBrightness;
  
    currentBrightness[i] = brightness;
    strip1.setPixelColor(i, strip1.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
    strip2.setPixelColor(i, strip2.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
    strip3.setPixelColor(i, strip3.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
    strip4.setPixelColor(i, strip4.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
    strip5.setPixelColor(i, strip5.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
    strip6.setPixelColor(i, strip6.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
  }
    strip1.show();
    strip2.show();
    strip3.show();
    strip4.show();
    strip5.show();
    strip6.show();
}

void beAmbient() {
//timeAdditionValue = 8
timeAroundLoop = timeAroundLoop+timeAdditionValueAmb;
for(uint16_t i=0; i<21; i++) {
  //current fade amount
  tempSinVal = timeAroundLoop * currentFadeAmount[i];

//  
  tempSinVal = sin(tempSinVal);
  tempSinVal = abs(tempSinVal);
  tempSinVal = 1 - tempSinVal;
  int brightness = tempSinVal * maxBrightness;

  currentBrightness[i] = brightness;
  
  strip1.setBrightness(25);
  strip1.setPixelColor(i, strip1.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
  strip2.setBrightness(25);
  strip2.setPixelColor(i, strip2.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
  strip3.setBrightness(25);
  strip3.setPixelColor(i, strip3.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
  strip4.setBrightness(25);
  strip4.setPixelColor(i, strip4.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
  strip5.setBrightness(25);
  strip5.setPixelColor(i, strip5.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
  strip6.setBrightness(25);
  strip6.setPixelColor(i, strip6.Color(0,brightness*200/maxBrightness,brightness*200/maxBrightness,brightness*20/maxBrightness));
}
    strip1.show();
    strip2.show();
    strip3.show();
    strip4.show();
    strip5.show();
    strip6.show();

}

void setOff() {
      strip1.setBrightness(0);
      strip1.show();
      strip2.setBrightness(0);
      strip2.show();
      strip3.setBrightness(0);
      strip3.show();
      strip4.setBrightness(0);
      strip4.show();
      strip5.setBrightness(0);
      strip5.show();
      strip6.setBrightness(0);
      strip6.show();
}
