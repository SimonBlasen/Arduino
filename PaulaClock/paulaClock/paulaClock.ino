#include <Wire.h>
#include "DS3231.h";




//#include <Adafruit_GFX.h>
//#include <gfxfont.h>

//#include <Adafruit_SSD1306.h>

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

#define SD_ChipSelectPin 4 //Chip select is pin number 4
TMRpcm music; //Lib object is named "music"

int song_number=2;
boolean debounce1=true;
boolean debounce2=true;
boolean play_pause;


DS3231 ds;






/*
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

bool foundICDevice = false;
byte address = 0x00;

*/










int counter = 0;



void setup(){
music.speakerPin = 9; //Auido out on pin 9
Serial.begin(9600); //Serial Com for debugging 
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}

pinMode(2, INPUT_PULLUP); //Button 1 with internal pull up to chage track
pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to play/pause
pinMode(3, INPUT_PULLUP); //Button 2 with internal pull up to fast forward

music.setVolume(3);    //   0 to 7. Set volume level
music.quality(1);        //  Set 1 for 2x oversampling Set 0 for normal
//music.volume(0);        //   1(up) or 0(down) to control volume
//music.play("filename",30); plays a file starting at 30 seconds into the track    

  music.loop(true);
  music.play("2.wav"); //Play song 2 from 33rd second 


  Wire.begin(); // Initiate the Wire library
/*

  rtc.begin();

  
  rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014*/
  Serial.println("SET");


    ds.setSecond(2);
    ds.setMinute(1);
    ds.setHour(8);
    ds.setDoW(1);
    ds.setDate(6);
    ds.setMonth(5);
    ds.setYear(2018);
    ds.setClockMode(false);
  
}

bool lift1 = true;
bool lift2 = true;

int volumeNow = 3;

void loop()
{ 



    

    
if (digitalRead(2)==LOW && lift1 ) //Button 1 Pressed
{
    lift1 = false;
  Serial.println("KEY PRESSED");

volumeNow++;
if (volumeNow > 6)
volumeNow = 6;
  
music.setVolume(volumeNow);
}
else if (digitalRead(2)!=LOW && lift1 == false)
{
    lift1 = true;
}


if (digitalRead(3)==LOW && lift2 ) //Button 1 Pressed
{
    lift2 = false;
  Serial.println("KEY PRESSED 33");
  
volumeNow--;
if (volumeNow < 1)
volumeNow = 1;

music.setVolume(volumeNow);

}
else if (digitalRead(3)!=LOW && lift2 == false)
{
    lift2 = true;
}

  
 /* if (digitalRead(2)==LOW  && debounce1 == true) //Button 1 Pressed
  {
  song_number++;
  if (song_number==5)
  {song_number=1;}
  debounce1=false;
  Serial.println("KEY PRESSED");
  Serial.print("song_number=");
  Serial.println(song_number);

  if (song_number ==1)
  {music.play("1.wav");} //Play song 1 from 10th second 

  if (song_number ==2)
  {music.play("2.wav");} //Play song 2 from 33rd second 

  if (song_number ==3)
  {music.play("3.wav");} //Play song 3 from start

  if (song_number ==4)
  {music.play("4.wav",25);} //Play song 4 from 25th second 

  if (digitalRead(3)==LOW  && debounce2 == true) //Button 2 Pressed
  {
  music.pause();  Serial.println("PLAY / PAUSE");
  debounce2=false;
  }

  if (digitalRead(2)==HIGH) //Avoid debounce
  debounce1=true;

  if (digitalRead(3)==HIGH)//Avoid debounce
  debounce2=true;
}*/

}









