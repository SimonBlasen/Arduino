#include <Util.h>
#include <Mover.h>
 
#define NUMMOVERS      5
#define COLOR_R        255
#define COLOR_G        255
#define COLOR_B        255

int delayval = 10;
bool on = false;

byte reds[] = {1, 0, 0, 1, 1};
byte greens[] = {1, 0, 1, 0, 1};
byte blues[] = {1, 1, 0, 0, 0};

Mover **movers = new Mover*[NUMMOVERS];

void setup() {
  //movers = new Mover[NUMMOVERS];
  for (int i = 0; i < NUMMOVERS; i++)
  {
    movers[i] = new Mover(NUMPIXELS);
  }
  pixels.begin();
}
 
void loop() {

  for (int i = 0; i < NUMMOVERS; i++)
  {
    long randomNumber = random(2 * NUMPIXELS);
    int walkDist = randomNumber - NUMPIXELS;
    movers[i]->moveDistance(walkDist);

    movers[i]->performMove();

    led(movers[i]->GetOldPos(), {0,0,0});

    led(movers[i]->GetCurPos(), {reds[i],greens[i],blues[i]});
  }
  
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delayval); // Delay for a period of time (in milliseconds).
}

