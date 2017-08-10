// Currently Pinset for MEGA
// 



#include <Adafruit_NeoPixel.h>
#include <Mover.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
 
#define PIN            48 // Digital PIN on the MEGA
#define NUMPIXELS      60
#define NUMMOVERS      5
#define COLOR_R        255
#define COLOR_G        255
#define COLOR_B        255



 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
int delayval = 16;

byte colorWhite[] = {255, 255, 255};

byte reds[] = {255, 0, 0, 255, 255};
byte greens[] = {255, 0, 255, 0, 255};
byte blues[] = {255, 255, 0, 0, 0};


int whitePos = 0;
int otherCounter = 0;

void setup() {

  whitePos = 0;
  
  pixels.begin();
}
 
void loop() {

  otherCounter++;
  whitePos++;
  whitePos = (whitePos >= NUMPIXELS ? 0 : (whitePos < 0 ? (NUMPIXELS - 1) : whitePos));
  otherCounter = (otherCounter >= 70 ? 0 : (otherCounter < 0 ? (70) : otherCounter));

  for (int i = 0; i < NUMPIXELS; i++)
  {
    if (i == whitePos)
    {
      pixels.setPixelColor(i, pixels.Color(colorWhite[0],colorWhite[1],colorWhite[2]));
    }
    else
    {
      pixels.setPixelColor(i, pixels.Color(otherCounter,0,0));
    }
    
  }
  
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delayval); // Delay for a period of time (in milliseconds).
}

