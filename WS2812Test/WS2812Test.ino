
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
 
#define PIN            48
#define NUMPIXELS      10
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
int delayval = 100;
bool on = false;
 
void setup() {
  pixels.begin();
}
 
void loop() {
  for(int i=0; i<NUMPIXELS; i++){
    if (on)
    {
    pixels.setPixelColor(i, pixels.Color(255,255,255)); // Moderately bright green color.
    }
    else
    {
    pixels.setPixelColor(i, pixels.Color(0,255,0)); // Moderately bright green color.
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }

  on = !on;
}
