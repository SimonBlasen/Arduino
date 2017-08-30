
#include <Adafruit_NeoPixel.h>
#include <hsv.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            48
#define NUMPIXELS      100
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright black color.

  pixels.show();
}


void loop() {
  
}
