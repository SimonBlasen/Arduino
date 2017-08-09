
#include <Adafruit_NeoPixel.h>
#include <hsv.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      60
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void led_add(int pos, rgb color, double intensity = 1.0)
{
  uint32_t color = pixels.getPixelColor(pos);
  uint8_t red = color >> 16;
  uint8_t green = color >> 8;
  uint8_t blue = color;

  red   = min(255, red   + color.r * 255.0 * intensity);
  green = min(255, green + color.g * 255.0 * intensity);
  blue  = min(255, blue  + color.b * 255.0 * intensity);
  
  pixels.setPixelColor(pos, pixels.Color(red, green, blue));
}

void led(int pos, rgb color, double intensity = 1.0)
{
  pixels.setPixelColor(pos, pixels.Color((int)(color.r * 255.0 * intensity), (int)(color.g * 255.0 * intensity), (int)(color.b * 255.0 * intensity)));
}

class Mover
{
public:
  rgb color;
  int tail;
  int pos;
  int velocity;

  Mover(rgb color, int tail = 2, int pos = 0, int velocity = 1)
    : color(color), tail(tail), pos(pos), velocity(velocity)
  {
    // nop
  }

  void render()
  {
    for (int i = 0; i <= tail; ++i)
      led_add((pos + ((velocity > 0) ? -i : i)) % NUMPIXELS, color, 1.0 / pow(2, i));
  }

  void move()
  {
    pos += velocity;
    if (pos < 0)
      pos = NUMPIXELS + pos;
  }
};
 
void setup() {
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright black color.

  pixels.show();
}


const int delayval = 1000;

const int numMovers = 2;
Mover movers[numMovers] = {Mover({0, 1, 0}, 6, 0, 1),
                           Mover({0, 0, 1}, 6, 12, -1)};

int x = 0;

void loop() {
  for (int i = 0; i < NUMPIXELS; ++i)
  {
    led((x+i)%60, {0,0,0});
  }

  for (int i = 0; i < numMovers; ++i)
  {
    movers[i].render();
    movers[i].move();
  }
  
  pixels.show();
  delay(delayval);
}
