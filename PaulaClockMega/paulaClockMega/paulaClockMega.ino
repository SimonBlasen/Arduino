#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <NeoPixelBus.h>




#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>

/*
Arduino Based Music Player

 This example shows how to play three songs from SD card by pressing a push button

 The circuit:
 * Push Button on pin 2 and 3
 * Audio Out - pin 9
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 

 created  25 Jun 2017
 by Aswinth Raj

 This example code was created for CircuitDigest.com

 */
 
#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

#define SD_ChipSelectPin 53 //Chip select is pin number 4
TMRpcm music; //Lib object is named "music"

int song_number=2;
boolean debounce1=true;
boolean debounce2=true;
boolean play_pause;


DS1307 rtc;




const uint16_t PixelCount = 4;
const uint8_t PixelPin = 2;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);









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







// LEDs

int brightnessIdle = 60;
int brightnessIdleTop = 100;













void setup(){
music.speakerPin = 11; //Auido out on pin 9
Serial.begin(9600); //Serial Com for debugging 
  Wire.begin();


  rtc.begin();
if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}

pinMode(42, OUTPUT);
        digitalWrite(42, LOW);

pinMode(30, INPUT_PULLUP);
pinMode(31, INPUT_PULLUP);
pinMode(32, INPUT_PULLUP);
pinMode(33, INPUT_PULLUP);
pinMode(34, INPUT_PULLUP);
pinMode(35, INPUT_PULLUP);


//pinMode(2, INPUT_PULLUP); //Button 1 with internal pull up to chage track
//pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to play/pause
//pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to fast forward

music.quality(1);        //  Set 1 for 2x oversampling Set 0 for normal
music.setVolume(5);    //   0 to 7. Set volume level
//music.volume(0);        //   1(up) or 0(down) to control volume
//music.play("filename",30); plays a file starting at 30 seconds into the track    

  music.loop(true);
  music.play("2.wav"); //Play song 2 from 33rd second 

Serial.println("All good with SD Card");







    strip.Begin();
    strip.Show();


    strip.SetPixelColor(0, RgbColor(0, 0, 0));
    strip.SetPixelColor(1, RgbColor(0, 0, 0));
    strip.SetPixelColor(2, RgbColor(0, 0, 0));
    strip.SetPixelColor(3, RgbColor(0, 0, 0));

    strip.Show();














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
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

    display.display();
  
  display.clearDisplay();
  }





  
}

int ledMode = 0;
int ledCounter = 0;

int ledModeIntern = 0;
int ledGlobalCounter = 0;



int pressTime = 0;
bool lift1 = true;
bool lift2 = true;
bool lift3 = true;
bool lift4 = true;
bool lift5 = true;
bool lift6 = true;
bool waitForPresstime = false;

int volumeNow = 5;

int counter = 0;



int state = 0;

// Alarm
int alarmState = 0;
int alarmHour = 18, alarmMinute = 05;
int setAlarmState = 0;
int displayTextEnd = 0;
int endScreenCounter = 0;
int alarmCounter = 0;


// Set time
int sYear, sMonth, sDay, sHour, sMinute;
int setTimeState = 0;



// Good night text
int gnTextCounter = 0;


int dowC = 0;
int alarmVolumeCounter = 0;

bool firstStartAlarmSound = true;
bool alarmRinging = false;
void alarmStart()
{
    if (alarmRinging == false)
    {
        alarmVolumeCounter = 0;
        ledMode = 2;
        digitalWrite(42, LOW);
        alarmRinging = true;
        music.setVolume(5);  

        music.loop(true);
        music.play("3.wav"); //Play song 2 from 33rd second 
    }
}

void alarmStop()
{
    if (alarmRinging)
    { 
        ledMode = 0;
        digitalWrite(42, HIGH);
        alarmState = 0;
        alarmRinging = false;
        music.stopPlayback();
    }
}


void loop()
{ 
    counter++;

    if (alarmRinging)
    {
        alarmVolumeCounter++;
        if (alarmVolumeCounter == 10)
        {
            music.setVolume(6); 
        }
        else if (alarmVolumeCounter == 25)
        {
            music.setVolume(6); 
        }
    }

    alarmCounter++;
    if (alarmCounter  >= 1000)
    {
        alarmCounter = 0;
        if (alarmState == 2)
        {
            DateTime now = rtc.now();
            if (now.hour() == alarmHour && now.minute() == alarmMinute)
            {
                alarmStart();
            }
            else if (alarmRinging)
            {
                alarmStop();
            }
        }
    }

    
    // Show clock
    if (state == 0)
    {
        if (counter == 1000)
        {
            if (firstStartAlarmSound)
            {
                firstStartAlarmSound = false;
                music.stopPlayback();
                digitalWrite(42, HIGH);
            }
            counter = 0;

            DateTime now = rtc.now();
            display.clearDisplay();

            if (alarmState == 2)
            {
                showAlarmClock();
                showAlarmTime(alarmHour, alarmMinute);
            }

            showDOW(now.dayOfWeek() - 1);
            showDate(now.day(),now.month(),now.year());
            
            drawLCDClock(now.hour(), now.minute(), now.second() % 2 == 0);
        
            display.display();
        }
    }

    // Set alarm
    else if (state == 1)
    {
        if (waitForPresstime && pressTime > 1500)
        {
            state = 2;
            setTimeState = 0;

            DateTime now = rtc.now();
            sYear = now.year();
            sMonth = now.month();
            sDay = now.day();
            sHour = now.hour();
            sMinute = now.minute();
        }
        
        if (counter == 100)
        {
            counter = 0;
            
            display.clearDisplay();
            

            if (setAlarmState == 0)
            {
                display.setTextColor(WHITE);
                display.setTextSize(1);
                display.setCursor(0,0);
                display.print("Wecker einstellen");
                display.setCursor(0,8);
                display.print(" Stunde");
                drawLCDClock(alarmHour, alarmMinute, true);
            }
            if (setAlarmState == 1)
            {
                display.setTextColor(WHITE);
                display.setTextSize(1);
                display.setCursor(0,0);
                display.print("Wecker einstellen");
                display.setCursor(0,8);
                display.print(" Minute");
                drawLCDClock(alarmHour, alarmMinute, true);
                endScreenCounter = 0;
            }
            if (setAlarmState == 2)
            {
                endScreenCounter++;
                display.setTextSize(2);
                display.setCursor(0,0);
                display.print("Gspeichert");

                if (endScreenCounter > 16)
                {
                    state = 0;
                }
            }
            
            display.display();
        }
    }

    // Set time
    else if (state == 2)
    {
        if (counter == 100)
        {
            counter = 0;
            
            display.clearDisplay();
            
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0,0);
            display.print("Zeit einstellen");
            
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0,8);

            if (setTimeState == 0)
            {
                display.print(" Jahr");
                drawLCDYear(sYear);
            }
            else if (setTimeState == 1)
            {
                display.print(" Monat");
                drawLCDDay(sDay, sMonth, true);
            }
            else if (setTimeState == 2)
            {
                display.print(" Tag");
                drawLCDDay(sDay, sMonth, true);
            }
            else if (setTimeState == 3)
            {
                display.print(" Stunde");
                drawLCDClock(sHour, sMinute, true);
            }
            else if (setTimeState == 4)
            {
                display.print(" Minute");
                drawLCDClock(sHour, sMinute, true);
            }

            
            
            display.display();
        }
    }


    // GuteNacht Text
    else if (state == 3)
    {
        if (counter == 1000)
        {
            gnTextCounter++;
            if (gnTextCounter > 100)
            {
                state = 0;
            }
            counter = 0;

            DateTime now = rtc.now();
            display.clearDisplay();

            if (alarmState == 2)
            {
                showAlarmClock();
                showAlarmTime(alarmHour, alarmMinute);
            }

            showDOW(now.dayOfWeek() - 1);
            showDate(now.day(),now.month(),now.year());
            
        



        
            display.setTextSize(1);
            display.setCursor(0,16);

            int text = 4;//displayTextEnd;

            String line1 = "";
            String line2 = "";
            String line3 = "";
            String line4 = "";
            String line5 = "";
            String line6 = "";
            
            if (text == 0)
            {   
               line1 = "Gute Nacht und schlaf";
               line2 = "gut :)";
            }
            else if (text == 1)
            {   
              //line1 = "Gute Nacht und schlaf";
                line1 = "Schlaf schoen und";
                line2 = "traeum was schoenes,";
                line3 = "hab dich lieb :*";
            }
            else if (text == 2)
            {   
              //line1 = "Gute Nacht und schlaf";
                line1 = "La problema es, que  ";
                line2 = "no conozco los       ";
                line3 = "acentos en espanol,  ";
                line4 = "pero la visualizacion";
                line5 = "no puede representar,";
                line6 = "pues no hay problema";
            }
            else if (text == 3)
            {   
              //line1 = "Gute Nacht und schlaf";
                line1 = "\"Los cuervos         ";
                line2 = "necesitan alcohol    ";
                line3 = "- Walter Bazar\"      ";
                line4 = "                     ";
                line5 = "        Chava Igesias";
                line6 = "";
            }
            else if (text == 4)
            {   
              //line1 = "Gute Nacht und schlaf";
                line1 = "Du:  Hmmmm           ";
                line2 = "Ich: Hahahaha        ";
                line3 = "Du:  Hahahahaha      ";
                line4 = "                     ";
                line5 = "";
                line6 = "";
            }
                
            display.print(String(line1 + line2 + line3 + line4 + line5 + line6));
            
            display.display();
        }

            
    }







    if (ledMode == 0)
    {
        ledMode = -1;
        ledModeIntern = 0;
        ledGlobalCounter = 0;
        ledCounter = 0;
        
        strip.SetPixelColor(0, RgbColor(0, 0, 0));
        strip.SetPixelColor(1, RgbColor(0, 0, 0));
        strip.SetPixelColor(2, RgbColor(0, 0, 0));
        strip.SetPixelColor(3, RgbColor(0, 0, 0));
        strip.Show();
    }
    else if (ledMode == 2)
    {
        ledCounter++;

        int multi = 200;
        if (ledCounter == 1)
        {
           strip.SetPixelColor(0, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(1, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(2, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(3, RgbColor(0, 0, 0));
           strip.Show();
        }
        else if (ledCounter == 5 * multi)
        {
           strip.SetPixelColor(0, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(1, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(2, RgbColor(0, 0, 0));
           strip.SetPixelColor(3, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.Show();
        }
        else if (ledCounter == 10 * multi)
        {
           strip.SetPixelColor(0, RgbColor(0, 0, 0));
           strip.SetPixelColor(1, RgbColor(0, 0, 0));
           strip.SetPixelColor(2, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(3, RgbColor(0, 0, 0));
           strip.Show();
        }
        else if (ledCounter == 15 * multi)
        {
           strip.SetPixelColor(0, RgbColor(0, 0, 0));
           strip.SetPixelColor(1, RgbColor(0, 0, 0));
           strip.SetPixelColor(2, RgbColor(0, 0, 0));
           strip.SetPixelColor(3, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.Show();
        }
        else if (ledCounter > 20 * multi)
        {
            ledCounter = 0;
        }
        /*if (ledCounter < 1 * multi)
        {
           float s = ((float)ledCounter) / multi;
           int val = (s * brightnessIdleTop);

           strip.SetPixelColor(0, RgbColor(val, val, val));
           strip.SetPixelColor(1, RgbColor(val, val, val));
           strip.SetPixelColor(2, RgbColor(val, val, val));
           strip.SetPixelColor(3, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
        }
        else if (ledCounter < 5 * multi)
        {
           strip.SetPixelColor(0, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(1, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(2, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
           strip.SetPixelColor(3, RgbColor(0, 0, 0));
        }
        else if (ledCounter < 6 * multi)
        {
           float s = ((float)(ledCounter - 5 * multi)) / multi;
           int val = (s * brightnessIdleTop);

           strip.SetPixelColor(0, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
           strip.SetPixelColor(1, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
           strip.SetPixelColor(2, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
           strip.SetPixelColor(3, RgbColor(val, val, val));
        }
        else if (ledCounter < 10 * multi)
        {
           strip.SetPixelColor(0, RgbColor(0, 0, 0));
           strip.SetPixelColor(1, RgbColor(0, 0, 0));
           strip.SetPixelColor(2, RgbColor(0, 0, 0));
           strip.SetPixelColor(3, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
        }
        else
        {
            ledCounter = 0;
        }*/

        
    }
    else if (ledMode == 1)
    {
        if (ledModeIntern == 0)
        {
            ledCounter++;
            if (ledCounter == 0)
            {
                strip.SetPixelColor(0, RgbColor(0, 0, 0));
                strip.SetPixelColor(1, RgbColor(0, 0, 0));
                strip.SetPixelColor(2, RgbColor(0, 0, 0));
                strip.SetPixelColor(3, RgbColor(0, 0, 0));
            }
            else if (ledCounter < 5000)
            {
                int val = (((float)ledCounter) / 5000) * brightnessIdle;
                
                strip.SetPixelColor(0, RgbColor(val, val, val));
                strip.SetPixelColor(1, RgbColor(val, val, val));
            }
            else if (ledCounter < 5000 + 2000)
            {
                int val = (((float)ledCounter - 5000) / 2000) * brightnessIdleTop;
                
                strip.SetPixelColor(2, RgbColor(val, val, val));
            }
            else
            {
                ledModeIntern = 1;
                ledCounter = 0;
                ledGlobalCounter = 0;
                strip.SetPixelColor(0, RgbColor(brightnessIdle, brightnessIdle, brightnessIdle));
                strip.SetPixelColor(1, RgbColor(brightnessIdle, brightnessIdle, brightnessIdle));
                strip.SetPixelColor(2, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
            }
            strip.SetPixelColor(3, RgbColor(0, 0, 0));
        }
        else if (ledModeIntern == 1)
        {
            int multi = 2000;
            ledCounter++;
        
            strip.SetPixelColor(0, RgbColor(brightnessIdle, brightnessIdle, brightnessIdle));
            strip.SetPixelColor(1, RgbColor(brightnessIdle, brightnessIdle, brightnessIdle));
            
            if (ledCounter == 0)
            {
            
                strip.SetPixelColor(2, RgbColor(brightnessIdleTop, brightnessIdleTop, brightnessIdleTop));
                strip.SetPixelColor(3, RgbColor(0, 0, 0));
            }
            if (ledCounter < 1 * multi)
            {
               float s = ((float)ledCounter) / multi;
               int val = (s * brightnessIdleTop);
            
               strip.SetPixelColor(2, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
               strip.SetPixelColor(3, RgbColor(val, val, val));
            }
            else if (ledCounter < 2 * multi)
            {
                float s = ((float)(ledCounter - 1 * multi)) / multi;
                int val = (s * brightnessIdleTop);
            
                strip.SetPixelColor(2, RgbColor(val, val, val));
                strip.SetPixelColor(3, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
            }
            else
            {
                ledCounter = 0;
            ledGlobalCounter++;
            }
            
            if ((ledGlobalCounter) > 14 && ledCounter == 0)
            {
                ledModeIntern = 2;
                ledCounter = 0;
            }
        }
        else if (ledModeIntern == 2)
        {
            ledCounter++;
            
            strip.SetPixelColor(3, RgbColor(0, 0, 0));
            if (ledCounter < 2000)
            {
                int val = (((float)ledCounter) / 2000) * brightnessIdleTop;
                
                strip.SetPixelColor(2, RgbColor(brightnessIdleTop - val, brightnessIdleTop - val, brightnessIdleTop - val));
            }
            else if (ledCounter < 8000 + 2000)
            {
                int val = (((float)ledCounter - 2000) / 8000) * brightnessIdle;
                
                strip.SetPixelColor(0, RgbColor(brightnessIdle - val, brightnessIdle - val, brightnessIdle - val));
                strip.SetPixelColor(1, RgbColor(brightnessIdle - val, brightnessIdle - val, brightnessIdle - val));
                strip.SetPixelColor(2, RgbColor(0, 0, 0));
                strip.SetPixelColor(3, RgbColor(0, 0, 0));
            }
            else
            {
                ledMode = 0;
                ledGlobalCounter = 0;
                ledModeIntern = 0;
                ledCounter = 0;
                strip.SetPixelColor(0, RgbColor(0, 0, 0));
                strip.SetPixelColor(1, RgbColor(0, 0, 0));
                strip.SetPixelColor(2, RgbColor(0, 0, 0));
                strip.SetPixelColor(3, RgbColor(0, 0, 0));
            }
        }



        
        
        strip.Show();
    }








    

    pressTime++;
if (digitalRead(30)==LOW && lift1 ) //Button 1 Pressed
{
    pressTime = 0;
    lift1 = false;
    Button1Lift();
}
else if (digitalRead(30)!=LOW && lift1 == false)
{
    waitForPresstime = false;
    lift1 = true;
    Button1Press();
}


if (digitalRead(31)==LOW && lift2 )
{
    pressTime = 0;
    lift2 = false;
  

}
else if (digitalRead(31)!=LOW && lift2 == false)
{
    waitForPresstime = false;
    lift2 = true;
    Button2Press();
}

if (digitalRead(32)==LOW && lift3 )
{
    waitForPresstime = false;
    lift3 = false;
}
else if (digitalRead(32)!=LOW && lift3 == false)
{
    pressTime = 0;
    lift3 = true;
    Button3Press();
}

if (digitalRead(33)==LOW && lift4 )
{
    pressTime = 0;
    lift4 = false;
}
else if (digitalRead(33)!=LOW && lift4 == false)
{
    waitForPresstime = false;
    lift4 = true;
    Button4Press();
}

if (digitalRead(34)==LOW && lift5 )
{
    pressTime = 0;
    lift5 = false;
}
else if (digitalRead(34)!=LOW && lift5 == false)
{
    waitForPresstime = false;
    lift5 = true;
    Button6Press();
}

if (digitalRead(35)==LOW && lift6 )
{
    pressTime = 0;
    lift6 = false;
}
else if (digitalRead(35)!=LOW && lift6 == false)
{
    waitForPresstime = false;
    lift6 = true;
    Button5Press();
}

}



















void Button1Press()
{
    counter = 0;
    volumeNow++;
    if (volumeNow > 6)
    volumeNow = 6;
  
    //music.setVolume(volumeNow);


    //TEMP TODO
    //if (alarmState == 0)
    alarmState = 2;
    //else
    //alarmState = 0;

    if (alarmState == 0)
    {
        alarmStop();
    }

    //state = 3;
    displayTextEnd = random(2);// Alarm text amounts
    
}


void Button1Lift()
{
    counter = 0;
    volumeNow++;
    if (volumeNow > 6)
    volumeNow = 6;
  
    //music.setVolume(volumeNow);


    //TEMP TODO
    //if (alarmState == 0)
    //alarmState = 2;
    //else
    alarmState = 0;

    if (alarmState == 0)
    {
        alarmStop();
    }

    //state = 3;
    displayTextEnd = random(2);// Alarm text amounts
    
}
void Button2Press()
{
    counter = 0;
    volumeNow--;
    if (volumeNow < 1)
    volumeNow = 1;



    

    if (alarmState == 2)
    {

        if (alarmRinging)
        {
            alarmMinute += 5;
            if (alarmMinute >= 60)
            {
                alarmMinute = alarmMinute - 60;
                alarmHour++;
                if (alarmHour >= 24)
                {
                    alarmHour = 0;
                }
            }
        }
        alarmStop();
        
        alarmState = 2;
    }


    
    

    ledMode = 1;

    //music.setVolume(volumeNow);
    
}
void Button3Press()
{
    counter = 0;
    Serial.println("Button 3");
    state++;
    state = state % 2;

    setAlarmState = 0;
    
    waitForPresstime = true;
}
void Button4Press()
{
    counter = 0;
    Serial.println("Button 4");

    if (state == 1)
    {
        setAlarmState++;
        if (setAlarmState >= 3)
        {
            state = 0;
        }
    }

    if (state == 2)
    {
        DateTime now = rtc.now();
        if (setTimeState == 0)
        {
            now.setyear(sYear);
            rtc.adjust(now);
        }
        else if (setTimeState == 1)
        {
            now.setmonth(sMonth);
            rtc.adjust(now);
        }
        else if (setTimeState == 2)
        {
            now.setday(sDay);
            rtc.adjust(now);
        }
        else if (setTimeState == 3)
        {
            now.sethour(sHour);
            rtc.adjust(now);
        }
        else if (setTimeState == 4)
        {
            now.setminute(sMinute);
            now.setsecond(0);
            rtc.adjust(now);
        }
        
        setTimeState++;
        if (setTimeState >= 5)
        {
            state = 0;
        }
    }
}
void Button5Press()
{
    if (alarmRinging)
    {
        volumeNow--;
        if (volumeNow < 1)
        {
            volumeNow = 1;
        }
        music.setVolume(volumeNow);    //   0 to 7. Set volume level
    }
    
    counter = 0;
    Serial.println("Button 5");


    if (state == 1)
    {
        if (setAlarmState == 0)
        {
            alarmHour--;
            if (alarmHour < 0)
            {
                alarmHour = 23;
            }
        }
        else if (setAlarmState == 1)
        {
            alarmMinute--;
            if (alarmMinute < 0)
            {
                alarmMinute = 59;
            }
        }
    }

    
    if (state == 2)
    {
        if (setTimeState == 0)
        {
            sYear--;
        }
        else if (setTimeState == 1)
        {
            sMonth--;
            if (sMonth < 0)
            {
                sMonth = 12;
            }
        }
        else if (setTimeState == 2)
        {
            sDay--;
            if (sDay < 0)
            {
                sDay = 31;
            }
        }
        else if (setTimeState == 3)
        {
            sHour--;
            if (sHour < 0)
            {
                sHour = 23;
            }
        }
        else if (setTimeState == 4)
        {
            sMinute--;
            if (sMinute < 0)
            {
                sMinute = 59;
            }
        }
    }
}
void Button6Press()
{
    if (alarmRinging)
    {
        volumeNow++;
        if (volumeNow > 7)
        {
            volumeNow = 7;
        }
        music.setVolume(volumeNow);    //   0 to 7. Set volume level
    }
    
    counter = 0;
    Serial.println("Button 6");

    if (state == 1)
    {
        if (setAlarmState == 0)
        {
            alarmHour++;
            alarmHour = alarmHour % 24;
        }
        else if (setAlarmState == 1)
        {
            alarmMinute++;
            alarmMinute = alarmMinute % 60;
        }
    }

    
    if (state == 2)
    {
        if (setTimeState == 0)
        {
            sYear++;
        }
        else if (setTimeState == 1)
        {
            sMonth++;
            sMonth = sMonth % 13;
        }
        else if (setTimeState == 2)
        {
            sDay++;
            sDay = sDay % 32;
        }
        else if (setTimeState == 3)
        {
            sHour++;
            sHour = sHour % 24;
        }
        else if (setTimeState == 4)
        {
            sMinute++;
            sHour = sHour % 60;
        }
    }
}






































void showDOW(int dow)
{
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);

    if (dow < 0)
    {
        dow += 7;
    }
    else if (dow > 6)
    {
        dow -= 7;
    }
    
    if (dow == 0)
    {
        display.print("Montag");
    }
    else if (dow == 1)
    {
        display.print("Dienstag");
    }
    else if (dow == 2)
    {
        display.print("Mittwoch");
    }
    else if (dow == 3)
    {
        display.print("Donnerstag");
    }
    else if (dow == 4)
    {
        display.print("Freitag");
    }
    else if (dow == 5)
    {
        display.print("Samstag");
    }
    else if (dow == 6)
    {
        display.print("Sonntag");
    }
}

void showDate(int dd, int mm, int yy)
{
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,8);

    String ddString = String(dd);
    if (dd < 10)
    {
        //ddString = "0" + ddString;
    }
    String mmString = String(mm);
    if (mm < 10)
    {
        //mmString = "0" + mmString;
    }

    display.print(ddString + "." + mmString + "." + String(yy));
}

int alarmClockXPos = 128 - 48;

void showAlarmTime(int hour, int minute)
{
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(alarmClockXPos + 18,4);
    
    if (minute < 10)
    {
        display.print(String(hour) + ":0" + String(minute));
        
    }
    else
    {
        display.print(String(hour) + ":" + String(minute));
    }
}







void showAlarmClock()
{
    int x = alarmClockXPos;
    
    display.drawPixel(x, 1, 1);
    display.drawPixel(x, 2, 1);
    display.drawPixel(x, 7, 1);
    display.drawPixel(x, 8, 1);
    display.drawPixel(x, 9, 1);
    display.drawPixel(x + 1, 0, 1);
    display.drawPixel(x + 1, 3, 1);
    display.drawPixel(x + 1, 5, 1);
    display.drawPixel(x + 1, 6, 1);
    display.drawPixel(x + 1, 10, 1);
    display.drawPixel(x + 1, 11, 1);
    display.drawPixel(x + 1, 15, 1);
    display.drawPixel(x + 2, 0, 1);
    display.drawPixel(x + 2, 3, 1);
    display.drawPixel(x + 2, 4, 1);
    display.drawPixel(x + 2, 12, 1);
    display.drawPixel(x + 2, 14, 1);
    display.drawPixel(x + 3, 1, 1);
    display.drawPixel(x + 3, 2, 1);
    display.drawPixel(x + 3, 3, 1);
    display.drawPixel(x + 3, 13, 1);
    display.drawPixel(x + 4, 2, 1);
    display.drawPixel(x + 4, 14, 1);
    display.drawPixel(x + 5, 2, 1);
    display.drawPixel(x + 5, 14, 1);
    display.drawPixel(x + 6, 0, 1);
    display.drawPixel(x + 6, 1, 1);
    display.drawPixel(x + 6, 2, 1);
    display.drawPixel(x + 6, 5, 1);
    display.drawPixel(x + 6, 6, 1);
    display.drawPixel(x + 6, 7, 1);
    display.drawPixel(x + 6, 8, 1);
    display.drawPixel(x + 6, 14, 1);
    display.drawPixel(x + 7, 2, 1);
    display.drawPixel(x + 7, 8, 1);
    display.drawPixel(x + 7, 14, 1);
    display.drawPixel(x + 8, 2, 1);
    display.drawPixel(x + 8, 8, 1);
    display.drawPixel(x + 8, 14, 1);
    display.drawPixel(x + 9, 1, 1);
    display.drawPixel(x + 9, 2, 1);
    display.drawPixel(x + 9, 3, 1);
    display.drawPixel(x + 9, 13, 1);
    display.drawPixel(x + 10, 0, 1);
    display.drawPixel(x + 10, 3, 1);
    display.drawPixel(x + 10, 4, 1);
    display.drawPixel(x + 10, 12, 1);
    display.drawPixel(x + 10, 14, 1);
    display.drawPixel(x + 11, 0, 1);
    display.drawPixel(x + 11, 3, 1);
    display.drawPixel(x + 11, 5, 1);
    display.drawPixel(x + 11, 6, 1);
    display.drawPixel(x + 11, 10, 1);
    display.drawPixel(x + 11, 11, 1);
    display.drawPixel(x + 11, 15, 1);
    display.drawPixel(x + 12, 1, 1);
    display.drawPixel(x + 12, 2, 1);
    display.drawPixel(x + 12, 7, 1);
    display.drawPixel(x + 12, 8, 1);
    display.drawPixel(x + 12, 9, 1);
}



void drawLCDClock(int hour, int minute, bool doublePointBlink)
{
    int xPos = 6;
    int yPos = 24;

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


void drawLCDDay(int hour, int minute, bool doublePointBlink)
{
    int xPos = 6;
    int yPos = 24;

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
            display.print(String(hour) + ".0" + String(minute));
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
            display.print(String(hour) + "." + String(minute));
        }
        else
        {
            display.print(String(hour) + " " + String(minute));
        }
    }
}


void drawLCDYear(int year)
{
    int xPos = 6;
    int yPos = 24;

    
    display.setTextColor(WHITE);
    display.setTextSize(4);
    display.setCursor(xPos,yPos);
    
    
    display.print(String(year));
      
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




