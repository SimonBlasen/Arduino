
#include <Adafruit_NeoPixel.h>
#include <hsv.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      60
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void led(int pos, rgb color)
{
      pixels.setPixelColor(pos, pixels.Color((int)(color.r * 255.0), (int)(color.g * 255.0), (int)(color.b * 255.0)));
}
 
void setup() {
  pixels.begin();
  for (int i = 0; i < 60; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright black color.

  pixels.show();
}


const int delayval = 200;
const int length = 5;
const double vmult = 0.1;
const int vsteps = 10;
const int fadeIn = 1;
const int fadeOut = 1;

int x = -1;
double v = 0;

void loop() {
  for (int i = 0; i < fadeIn; ++i)
  {
    hsv colorInHsv;
    colorInHsv.s = 1.0;
    colorInHsv.v = vmult * (v) / (vsteps - 1);
    colorInHsv.h = (x + i + 1) % 360;
    
    rgb colorInRgb = hsv2rgb(colorInHsv);
    
    led((x + i + 1) % 60, colorInRgb);
  }

  for (int i = 0; i < fadeOut && x - length >= i; ++i)
  {
    hsv colorOutHsv;
    colorOutHsv.s = 1.0;
    colorOutHsv.v = (vsteps - 1 - v - i) / (vsteps - 1) * vmult;
    colorOutHsv.h = (x - length - i) % 360;
    
    rgb colorOutRgb = hsv2rgb(colorOutHsv);

    led((x - length - i) % 60, colorOutRgb);
  }
  
  
  pixels.show();
  delay(delayval);

  v++;
  if (v >= vsteps)
  {
    v = 0;
    x++;
  }
}
