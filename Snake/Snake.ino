#include <Util.h>

const int tickTime = 1000;
const int deathTime = 3;      // ticks spent dead
const int intelligence = 100; // chance in percent to move intelligently

struct Pos
{
  int x, y;
} point;

bool eaten = false;
int deathTimer = 3;

struct SnakeSegment
{
  int x, y;
  SnakeSegment* next;
} snakeHead;

enum 
{
  UP    = 0,
  DOWN  = 1,
  LEFT  = 2,
  RIGHT = 3
} direction;

void clear()
{
  for (int i = 0; i < NUMPIXELS; i++)
    led(i, {0,0,0});
}

void setup() 
{
  Serial.begin(9600);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  
  pixels.begin();
  clear();

  pixels.show();

  snakeHead.x = 0;
  snakeHead.y = 0;
  snakeHead.next = NULL;
  point.x = -1;
  direction = random(4);
}

void spawnPoint()
{
  do {
    point.x = random(LED_MATRIX_WIDTH);
    point.y = random(LED_MATRIX_HEIGHT);

    rgb color = led_get(point.x, point.y);
    if (color.r == 0 && color.g == 0 && color.b == 0)
      break;
  } while(true);
}

void respawns()
{
  if (point.x == -1)
  {
    spawnPoint();
  }

  if (deathTimer > 0)
  {
    deathTimer--;
    if (deathTimer == 0)
    {
      snakeHead.x = random(LED_MATRIX_WIDTH);
      snakeHead.y = random(LED_MATRIX_HEIGHT);
      direction = random(4);

      SnakeSegment* seg = snakeHead.next;
      while (seg != NULL)
      {
        SnakeSegment* toDel = seg;
        seg = seg->next;
        delete toDel;
      }
      snakeHead.next = NULL;
    }
  }
}

void grow()
{
  // copy head
  SnakeSegment* next = new SnakeSegment;
  next->x = snakeHead.x;
  next->y = snakeHead.y;
  next->next = snakeHead.next;
  snakeHead.next = next;
}

void moveIntelligently()
{
  if (point.x < snakeHead.x)
    direction = LEFT;
  else if (point.x > snakeHead.x)
    direction = RIGHT;
  else if (point.y < snakeHead.y)
    direction = DOWN;
  else if (point.y > snakeHead.y)
    direction = UP;
}

void moveRandomly()
{
  // chance of 2/5 to change direction
  if (random(5) < 2)
  {
    switch (direction)
    {
    case UP:
    case LEFT:
      direction = (direction + random(2,4)) % 4;
      break;
    case DOWN:
    case RIGHT:
      direction = (direction + random(1,3)) % 4;
      break;
    }
  }
}

void move()
{
  grow();

  // Chance 
  if (random(100) < intelligence)
  {
    moveIntelligently();
  }
  else
  {
    moveRandomly();
  }

  // move head
  switch (direction)
  {
  case UP:
    snakeHead.y = (snakeHead.y + 1) % LED_MATRIX_HEIGHT;
    break;
  case DOWN:
    if (snakeHead.y == 0)
      snakeHead.y = LED_MATRIX_HEIGHT - 1;
    else
      snakeHead.y--;
    break;
  case RIGHT:
    snakeHead.x = (snakeHead.x + 1) % LED_MATRIX_WIDTH;
    break;
  case LEFT:
    if (snakeHead.x == 0)
      snakeHead.x = LED_MATRIX_WIDTH - 1;
    else
      snakeHead.x--;
    break;
  }

  // remove last tail segment
  if (!eaten)
  {
    SnakeSegment* seg = &snakeHead;
    while (seg->next != NULL)
    {
      if (seg->next->next == NULL)
      {
        delete seg->next;
        seg->next = NULL;
        break;
      }
      seg = seg->next;
    }
  }
  else
  {
    eaten = false;
  }
}

void eat()
{
  if (point.x == snakeHead.x && point.y == snakeHead.y)
  {
    point.x = -1;

    eaten = true;
  }
  else
  {
    rgb color = led_get(snakeHead.x, snakeHead.y);

    if (color.r > 0 || color.g > 0 || color.b > 0)
    {
      deathTimer = deathTime;
    }
  }
}

void display()
{
  clear();

  // Tail
  SnakeSegment* seg = &snakeHead;
  while (seg->next != NULL)
  {
    seg = seg->next;
    led (seg->x, seg->y, {1,1,1});
  }

  // Point
  led (point.x, point.y, {0,0,1});

  // Head
  if (deathTimer > 0)
  {
    led (snakeHead.x, snakeHead.y, {1,0,0});
  }
  else
  {
    led (snakeHead.x, snakeHead.y, {0,1,0});
  }
  
  pixels.show();
}

void loop() 
{
  if (deathTimer == 0)
  {
    move();
  
    eat();
  }
  
  respawns();

  display();

  delay(tickTime);
}
