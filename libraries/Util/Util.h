#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            48
#define NUMPIXELS      100
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include <hsv.h>

//   strip pos        software pos
// ^ y              ^ y
// |                |
// | 99.....90      | 90.......99
// | ........       | ...........
// | 19.....10      | 19.......10
// |  9 .... 0      |  0........9
// |------------> x --------------> x

const int LED_MATRIX_WIDTH = 10;
const int LED_MATRIX_HEIGHT = 10;

rgb led_get(int x, int y)
{
    int x1 = LED_MATRIX_WIDTH - x - 1;
    int pos = y * LED_MATRIX_WIDTH + x1;
    
    uint32_t color_old = pixels.getPixelColor(pos);
    uint8_t red = color_old >> 16;
    uint8_t green = color_old >> 8;
    uint8_t blue = color_old;
    
    return {red / 255, green / 255, blue / 255};
}

void led(int x, int y, rgb color, double intensity = 1.0)
{
    int x1 = LED_MATRIX_WIDTH - x - 1;
    int pos = y * LED_MATRIX_WIDTH + x1;
    
    pixels.setPixelColor(pos, pixels.Color((int)(color.r * 255.0 * intensity), (int)(color.g * 255.0 * intensity), (int)(color.b * 255.0 * intensity)));
}

void led_add(int x, int y, rgb color, double intensity = 1.0)
{
    int x1 = LED_MATRIX_WIDTH - x - 1;
    int pos = y * LED_MATRIX_WIDTH + x1;
    
    uint32_t color_old = pixels.getPixelColor(pos);
    uint8_t red = color_old >> 16;
    uint8_t green = color_old >> 8;
    uint8_t blue = color_old;

    red   = min(255, red   + color.r * 255.0 * intensity);
    green = min(255, green + color.g * 255.0 * intensity);
    blue  = min(255, blue  + color.b * 255.0 * intensity);

    pixels.setPixelColor(pos, pixels.Color(red, green, blue));
}

void led_add(int pos, rgb color, double intensity = 1.0)
{
    int y = pos / LED_MATRIX_HEIGHT;
    int x = pos - (y * LED_MATRIX_HEIGHT);
    if (y % 2 == 1)
    {
        x = LED_MATRIX_WIDTH - 1 - x;
    }
    
    led_add(x, y, color, intensity);
}

void led(int pos, rgb color, double intensity = 1.0)
{
    int y = pos / LED_MATRIX_HEIGHT;
    int x = pos - (y * LED_MATRIX_HEIGHT);
    if (y % 2 == 1)
    {
        x = LED_MATRIX_WIDTH - 1 - x;
    }
    
    led(x, y, color, intensity);
}