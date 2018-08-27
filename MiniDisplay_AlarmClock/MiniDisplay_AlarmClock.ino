
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

bool foundICDevice = false;
byte address = 0x00;

void setup()
{     
  Serial.begin(9600);
  Wire.begin();
  
  byte error, addressTemp;
  int nDevices;
 
  nDevices = 0;
  for(addressTemp = 1; addressTemp < 127; addressTemp++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(addressTemp);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
  address = addressTemp;
 
      nDevices++;
    }
    else if (error==4)
    {
    }    
  }
  if (nDevices > 0)
{
      Serial.println("Found device");
  foundICDevice = true;
      Serial.print(address,HEX);
}
else
{
  
      Serial.println("No device found");
}

  //delay(5000);

  if (foundICDevice)
  {
      //Serial.println("Displaying examples");
    display.begin(SSD1306_SWITCHCAPVCC, address);  // initialize with the I2C addr 0x3D (for the 128x64)

    display.display();
  
  display.clearDisplay();
  }
}

int counter = 0;

void loop() {
  display.clearDisplay();

    counter++;

    //drawClock();
    drawLCDClock(13, 5, counter % 2 == 0);



    display.display();











    delay(1000);
  
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.println("Hello, world!");
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  //display.println(3.141592);
  
  
  
  
  //display.setTextSize(2);
  //display.setCursor(0,0);
  //display.setTextColor(WHITE);
  //display.print("Speed:"); display.println(0x158 + (counter % 100), HEX);

  //display.setCursor(0,16);
  //display.setTextColor(WHITE);
  //display.print("RPM:  "); display.println(0x5964 + (counter * 3 % 1000), HEX);

  //display.setCursor(0,32);
  //display.setTextColor(WHITE);
  //display.print("Oil:  "); display.print(0x86  + (counter % 12), HEX); display.println("C");

  /*

  display.setTextSize(7);
  display.setCursor(64 - 17, 16);
  display.setTextColor(WHITE);
  if (counter == 0)
  {
    display.print("1");
  }
  else if (counter == 1)
  {
    display.print("N");
  }
  else if (counter == 2)
  {
    display.print("2");
  }
  else if (counter == 3)
  {
    display.print("3");
  }
  else if (counter == 4)
  {
    display.print("4");
  }
  else if (counter == 5)
  {
    display.print("5");
  }
  else if (counter == 6)
  {
    display.print("6");
  }

  
  int step = 16;
  int offset = 8;

    
  display.setTextSize(2);
  display.setCursor(offset + step * counter,0);
  if (counter == 0)
  {
    display.print("1");
  }
  else if (counter == 1)
  {
    display.print("N");
  }
  else if (counter == 2)
  {
    display.print("2");
  }
  else if (counter == 3)
  {
    display.print("3");
  }
  else if (counter == 4)
  {
    display.print("4");
  }
  else if (counter == 5)
  {
    display.print("5");
  }
  else if (counter == 6)
  {
    display.print("6");
  }





  
  display.display();

counter++;
counter = counter % 7;
  delay(1000);*/
}












void drawLCDClock(int hour, int minute, bool doublePointBlink)
{
    int xPos = 6;
    int yPos = 16;

    if (hour < 10)
    {
        xPos += 20;
    }
    
  
    display.setTextColor(WHITE);
    display.setTextSize(4);
    display.setCursor(xPos,yPos);
    
    //display.print("6");
    if (minute < 10)
    {
        if (doublePointBlink)
        {
            display.print(String(hour) + ":0" + String(minute));
        }
        else
        {
            display.print(String(hour) + " 0" + String(minute));
        }
    }
    else
    {
        if (doublePointBlink)
        {
            display.print(String(hour) + ":" + String(minute));
        }
        else
        {
            display.print(String(hour) + " " + String(minute));
        }
    }
}







void drawClock()
{
    int midX = 64;
    int midY = 40;
    int radius = 23;
    int linesLengths = 5;
  
    display.drawCircle(midX, midY, radius, 1);

    for (int i = 0; i < 12; i++)
    {
        float x = sin(radians(i * 30.0));
        float y = cos(radians(i * 30.0));
        
        display.drawLine((int16_t)(midX + x * radius), (int16_t)(midY + y * radius), (int16_t)(midX + x * (radius - linesLengths)), (int16_t)(midY + y * (radius - linesLengths)), 1);
    }
}

void drawClockPointers(float hour, float minute)
{
    int midX = 64;
    int midY = 40;
    int radius = 23;
    int minuteLength = 16;
    int hourLength = 10;


    //hour = hour % 12;

    float xHour = sin(radians(hour * 30.0));
    float yHour = cos(radians(hour * 30.0));
    float xMinute = sin(radians(minute * 6.0));
    float yMinute = cos(radians(minute * 6.0));

    
    display.drawLine((int16_t)(midX), (int16_t)(midY), (int16_t)(midX + xHour * (hourLength)), (int16_t)(midY - yHour * (hourLength)), 1);
    display.drawLine((int16_t)(midX), (int16_t)(midY), (int16_t)(midX + xMinute * (minuteLength)), (int16_t)(midY - yMinute * (minuteLength)), 1);
    
}











void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
  delay(1);
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    delay(1);
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    delay(1);
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
    delay(1);
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    delay(1);
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
    delay(1);
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
    delay(1);
  }
  delay(250);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
  delay(1);
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}
