
#include <Adafruit_NeoPixel.h>
#include <hsv.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      60
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void led(int pos, rgb color, double intensity = 1.0)
{
      pixels.setPixelColor(pos, pixels.Color((int)(color.r * 255.0 * intensity), (int)(color.g * 255.0 * intensity), (int)(color.b * 255.0 * intensity)));
}
 
void setup() {
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright black color.

  pixels.show();
}


const int delayval = 90;
const int length_max = 10;
const int length_min = 3;

const int steps = length_max - length_min;
const double interval = 0.9 / (double)steps;

const rgb color{0.1,0.2,0.9};

int x = 0;
int current_length = length_min;
bool expand = true;

void loop() {
  for (int i = 0; i < NUMPIXELS; ++i)
  {
    led((x+i)%60, {0,0,0});
  }

  for (int i = 0; i < current_length; ++i)
  {
    int step = length_max - current_length;
    double intensity = 0.1 + step * interval;
    led((x+i)%60, color, intensity);
  }
  
  pixels.show();
  delay(delayval);

  if (expand)
  {
    ++current_length;
    if (current_length >= length_max)
      expand = false;
  }
  else
  {
    ++x;
    --current_length;
    if (current_length == length_min)
      expand = true;
  }
}
