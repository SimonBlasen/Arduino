// NeoPixelTest
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.
//
// Included but commented out are examples of configuring a NeoPixelBus for
// different color order including an extra white channel, different data speeds, and
// for Esp8266 different methods to send the data.
// NOTE: You will need to make sure to pick the one for your platform 
//
//
// There is serial output of the current state so you can confirm and follow along
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 4; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 8;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

int reds[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
int greens[] = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int blues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};

int brightness = 50;
int brightnessRed = 27;


RgbColor colors[] = {green, green, green, green, green, red, red, red, red, red, red, blue, blue, blue, blue, blue};

int rpmIndex = 1;

int incomingByte = 0;





void setup()
{
    //Serial.begin(115200);
    Serial.begin(9600);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();


    Serial.println();
    Serial.println("Running...");





    for (int i = 0; i < 4; i++)
      {
        strip.SetPixelColor(i, RgbColor(brightness, brightness, brightness));
      }

     
    strip.Show();
}

bool isInPit = false;
bool pitBlinkOn = false;


int flash_count = 0;
bool flash_on = false;

int pitBlink = 0;

int mode = 0;

void loop()
{


    if (flash_on)
    {
        strip.SetPixelColor(0, RgbColor(brightness, brightness, brightness));
        strip.SetPixelColor(1, RgbColor(brightness, brightness, brightness));
        strip.SetPixelColor(2, RgbColor(brightness, brightness, brightness));
        strip.SetPixelColor(3, RgbColor(brightness, brightness, brightness));
    }
    else
    {
        strip.SetPixelColor(0, RgbColor(0, 0, 0));
        strip.SetPixelColor(1, RgbColor(0, 0, 0));
        strip.SetPixelColor(2, RgbColor(0, 0, 0));
        strip.SetPixelColor(3, RgbColor(0, 0, 0));
    }
    strip.Show();

    
    flash_on = !flash_on;


    delay(500);

  
}

/*
void loop()
{
    rpmIndex++;
    if (rpmIndex > 16)
    {
      rpmIndex = -30;
    }

    if (rpmIndex > 0)
    {
      for (int i = 0; i < rpmIndex; i++)
      {
        strip.SetPixelColor(i, colors[i]);
      }
     for (int i = rpmIndex; i < 16; i++)
     {
        strip.SetPixelColor(i, black);
     }
    }
    else
    {
     for (int i =0; i < 16; i++)
     {
        strip.SetPixelColor(i, black);
     }
    }

  
    delay(40);

    strip.Show();

}*/

