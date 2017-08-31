#include <Util.h>
#include <Mover.h>
 
#define PIN            48
#define NUMPIXELS      100
#define NUMMOVERS      5
#define COLOR_R        255
#define COLOR_G        255
#define COLOR_B        255



 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
int delayval = 5;
bool on = false;

byte reds[] = {255, 0, 0, 255, 255};
byte greens[] = {255, 0, 255, 0, 255};
byte blues[] = {255, 255, 0, 0, 0};

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
    
    pixels.setPixelColor(movers[i]->GetOldPos(), pixels.Color(0,0,0));
    
    pixels.setPixelColor(movers[i]->GetCurPos(), pixels.Color(reds[i],greens[i],blues[i]));
  }
  
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delayval); // Delay for a period of time (in milliseconds).
}

