#include <Util.h>

struct Vec
{
  int x, y;
};

class Mover
{
public:
  rgb color;
  int tail;
  Vec pos;
  Vec vel;

  Mover(rgb color, int tail = 2, Vec pos = {0, 0}, Vec velocity = {1, 0})
    : color(color), tail(tail), pos(pos), vel(velocity)
  {
    // nop
  }

  void render()
  {
    Vec oldPos = pos;
    Vec oldVel = vel;

    vel.x = -vel.x;
    vel.y = -vel.y;
    for (int i = 0; i <= tail; ++i)
    {
      led_add(pos.x, pos.y, color, 1.0 / pow(2, i));
      move();
    }
    
    pos = oldPos;
    vel = oldVel;
  }

  void move()
  {
    pos.x = (LED_MATRIX_WIDTH + pos.x + vel.x) % LED_MATRIX_WIDTH;
    pos.y = (LED_MATRIX_HEIGHT + pos.y + vel.y) % LED_MATRIX_HEIGHT;
  }
};
 
void setup() 
{
  randomSeed(analogRead(0));
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright black color.

  pixels.show();
}


const int delayval = 200;

struct MoverItem
{
  Mover mov;
  MoverItem* next;
  MoverItem* prev;
};

MoverItem* movers = NULL;
int moverCount = 0;

void spawn()
{
  rgb color{(double)random(255)/255.0, (double)random(255)/255.0, (double)random(255)/255.0};
  int tail = random(3,6);
  Vec pos{random(LED_MATRIX_WIDTH),random(LED_MATRIX_HEIGHT)};
  Vec vel{random(0,3)-1,random(0,3)-1};
  
  MoverItem* spawned = new MoverItem{Mover(color,tail,pos,vel),NULL,NULL};
  spawned->next = movers;
  movers->prev = spawned;
  movers = spawned;

  moverCount++;
}

void moverReplace()
{
  int chance = 15;
  MoverItem* it = movers;
  while (it != NULL)
  {
    if (random(100) < chance)
    {
      if (it->prev != NULL)
        it->prev->next = it->next;
      if (it->next != NULL)
        it->next->prev = it->prev;

      if (it == movers)
        movers = it->next;
      MoverItem* toDel = it;
      it = it->next;
      delete toDel;
      chance -= 5;
      moverCount--;
    }
    else
    {
      it = it->next;
    }
  }
  
  if (moverCount < 6)
  {
    if (random(10) < 10)
      spawn();
  }
}

void loop() 
{
  for (int i = 0; i < NUMPIXELS; ++i)
  {
    led(i, {0,0,0});
  }

  moverReplace();

  MoverItem* it = movers;
  while(it != NULL)
  {
    it->mov.move();
    it->mov.render();
    it = it->next;
  }
  
  pixels.show();
  delay(delayval);
}
