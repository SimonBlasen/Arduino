#include <Util.h>

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


const int delayval = 100;

const int numMovers = 3;
Mover movers[numMovers] = {Mover({0, 1, 0}, 4, 0, 1),
                           Mover({0, 0, 1}, 4, 12, -2),
                           Mover({1, 0, 0}, 4, 40, -1)};

int x = 0;

void loop() {
  for (int i = 0; i < NUMPIXELS; ++i)
  {
    led((x+i)%NUMPIXELS, {0,0,0});
  }

  for (int i = 0; i < numMovers; ++i)
  {
    movers[i].render();
    movers[i].move();
  }
  
  pixels.show();
  delay(delayval);
}
