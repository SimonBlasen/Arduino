/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-TFTM070-4V2.1(800x480 Pixels)
Software SPI,8080 16-bit Interface 5V Power Supply

This program is a demo of how to use most of the functions
of the library with a supported display modules.

Tested and worked with:
Arduino Mega 2560,Arduino Due
Works with Arduino 1.6.0 IDE 
****************************************************/

#include <D:/Dokumente/Arduino/TFTTest/TftTest1/UTFT.h>
#include <D:/Dokumente/Arduino/TFTTest/TftTest1/UTouch.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t GearFont[];
extern uint8_t GearFontSmall[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41

// Remember to change the model parameter to suit your display module!
UTFT myGLCD(SSD1963_480272,38,39,40,41);  //(byte model, int RS, int WR, int CS, int RST, int SER)
UTouch      myTouch(6,5,4,3,2);

void setup()
{
    
    Serial.begin(115200);
    
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");

    
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();


  //myTouch.InitTouch();
  //myTouch.setPrecision(PREC_MEDIUM);


  myGLCD.clrScr();
  
  myGLCD.setFont(BigFont);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("Display started", 0, 0, 0); 



  
 // -------------------------------------------------------------
  pinMode(8, OUTPUT);  //backlight 
  digitalWrite(8, HIGH);//on
// -------------------------------------------------------------
 
  myGLCD.setFont(GearFont);


  
}


char data[256];
int dataCounter = 0;
void shiftData(int amount)
{
    for (int i = 0; i < 32; i++)
    {
        if (i + amount < 32)
        {
        data[i] = data[i + amount];
        }
    }
}

bool firstData = true;

int m_gear = 0;
int m_speed = 0;
int m_gearOld = 1;
int m_speedOld = 1;
int m_oilTemp = 0;
int m_oilTempOld = 1;
int m_rpm = 0;
int m_rpmOld = 0;

int incomingByte = 0;
int counter = 0;
int tempCounter = 0;

int oldCircleAmount = 0;


int rpmFaultyCount = 0;


int m_lastLapMin = 0;
int m_lastLapSec = 0;
int m_lastLapMil = 0;
int m_lastLapMinOld = 0;
int m_lastLapSecOld = 0;
int m_lastLapMilOld = 0;







const byte numBytes = 32;
byte receivedBytes[numBytes];
byte numReceived = 0;
boolean newData = false;
void loop()
{
    recvBytesWithStartEndMarkers();

    
    if (false && Serial.available() > 0) {

        


        Serial.readBytes(data, 256);

        tempCounter++;
        m_gear = data[0];
        m_speed = (data[1] << 8) | (data[2]);

        // Gear
        /*if (data[0] == 0 && dataCounter >= 2)
        {
            m_gear = data[1];

            //shiftData(2);
            dataCounter = 0;
        }


        
        
        // Speed
        if (data[0] == 1 && dataCounter >= 3)
        {
            m_speed = (data[1] << 8) | (data[2]);
            
            //myGLCD.setFont(SevenSegNumFont);
            //myGLCD.print(String(curSpeed), 4, 4, 0); 

            //shiftData(3);
            dataCounter = 0;
        }*/
        
        


    }

    
    if (newData == true) {
        
        if (firstData)
        {
            firstData = false;
            myGLCD.clrScr();
        }
        
        if (numReceived >= 2 && receivedBytes[0] == 0)
        {
            //m_gear = receivedBytes[1];
        }
        else if (numReceived >= 3 && receivedBytes[0] == 1)
        {
            int newSpeed = (receivedBytes[1] << 8) | (receivedBytes[2]);
            if (newSpeed >= 0 && newSpeed < 400)
                m_speed = (receivedBytes[1] << 8) | (receivedBytes[2]);
                
            if (numReceived >= 5 && receivedBytes[3] == 34)
            {
                if (receivedBytes[4] >= 0 && receivedBytes[4] <= 8)
                    m_gear = receivedBytes[4];
            }
            if (numReceived >= 8 && receivedBytes[5] == 35)
            {
                int newRpm = (receivedBytes[6] << 8) | (receivedBytes[7]);

                if (newRpm >= 0 && newRpm <= 10000)
                {
                    if (newRpm - m_rpm < 1500 && m_rpm - newRpm < 1500)
                    {
                        m_rpm = (receivedBytes[6] << 8) | (receivedBytes[7]);
                    }
                    else
                    {
                        rpmFaultyCount++;
                        if (rpmFaultyCount > 10)
                        {
                            m_rpm = (receivedBytes[6] << 8) | (receivedBytes[7]);
                        }
                    }
                }
                
            }
        }

        // Oil temp
        else if (numReceived >= 2 && receivedBytes[0] == 2)
        {
            m_oilTemp = receivedBytes[1];
            Serial.write(1);
        }

        // Last Lap Time
        else if (numReceived >= 3 && receivedBytes[0]== 3)
        {
            m_lastLapMin = receivedBytes[1];
            m_lastLapSec = receivedBytes[2];
            m_lastLapMil = (receivedBytes[3] << 8) | (receivedBytes[4]);
        }
        newData = false;
    }


    

    if (m_gear != m_gearOld)
    {
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(0, 255, 0);
        myGLCD.setFont(GearFontSmall);
        myGLCD.print(String(m_gear), 480 / 2 - 64 / 2, 70, 0); 
        m_gearOld = m_gear;
    }
    if (m_speed != m_speedOld)
    {
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(0, 255, 0);
        myGLCD.setFont(BigFont);

        int xPos = 380;
        int yPos = 80;
        if (m_speed < 10)
        {
            myGLCD.print(String(0), xPos, yPos, 0); 
            myGLCD.print(String(0), xPos + 16, yPos, 0); 
            xPos += 16 * 2;
        }
        else if (m_speed < 100)
        {
            myGLCD.print(String(0), xPos, yPos, 0); 
            xPos += 16;
        }
        
        myGLCD.print(String(m_speed), xPos, yPos, 0); 
        m_speedOld = m_speed;
    }
    if (m_oilTemp != m_oilTempOld)
    {
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(0, 255, 0);
        myGLCD.setFont(BigFont);
        myGLCD.print(String(m_oilTemp), 4, 200, 0); 
        m_oilTempOld = m_oilTemp;
    }
    if ((m_lastLapMil != m_lastLapMilOld || m_lastLapMin != m_lastLapMinOld || m_lastLapSec != m_lastLapSecOld))
    {
        myGLCD.setColor(0, 255, 0);
        myGLCD.setFont(SmallFont);
        myGLCD.print(String(m_lastLapMin), 4, 150, 0); 
        myGLCD.print(".", 4 + 16, 150, 0); 
        myGLCD.print(String(m_lastLapSec), 4 + 32, 150, 0); 
        myGLCD.print(".", 4 + 32 + 32, 150, 0); 
        myGLCD.print(String(m_lastLapMil), 4 + 32 + 32 + 16, 150, 0); 

        m_lastLapMilOld = m_lastLapMil;
        m_lastLapMinOld = m_lastLapMin;
        m_lastLapSecOld = m_lastLapSec;
    }
    if (m_rpm != m_rpmOld)
    {
        myGLCD.setColor(0, 255, 0);
        //myGLCD.setFont(SmallFont);
        //myGLCD.print(String(m_rpm), 220, 215, 0); 
        
        float ratioRpm = m_rpm / 10000.0f > 1.0f ? 1.0f : m_rpm / 10000.0f;

        ratioRpm = ratioRpm - 0.85f;
        ratioRpm *= 1.0f / (1.0f - 0.85f);

        if (ratioRpm < 0.0f)
        {
            ratioRpm = 0.0f;
        }


        int number = (int)(ratioRpm * 12.4f);

        if (number > 4 && number <= 8)
        {
            number = 8;
        }
        if (number > 8 && number <= 12)
        {
            number = 12;
        }
        if (number > 12)
        {
            number = 13;
        }



        
        //myGLCD.setFont(BigFont);
        //myGLCD.print(String(m_rpm), 4, 150, 0); 

        int rpmStart = 7500;
        int rpmStep = 200;
        int yPos = 30;
        int xPos = 30+24;
        int xWidth = 34;
        int radius = 12;

        int nowCircleAmount = 0;
        for (int i = 0; i < 12; i++)
        {
            //if (m_rpm >= (rpmStart + rpmStep * i))
            if (i < number)
            {
                nowCircleAmount++;
            }
            else
            {
            }
        }
        if (nowCircleAmount != oldCircleAmount)
        {
            int from = nowCircleAmount < oldCircleAmount ? nowCircleAmount : oldCircleAmount;
            int to = nowCircleAmount < oldCircleAmount ? oldCircleAmount : nowCircleAmount;
            
        for (int i = from; i < to; i++)
        {
            if (i < number)
            {
                if (i < 4)
                    myGLCD.setColor(0, 0, 255);
                else if (i < 8)
                    myGLCD.setColor(0, 255, 0);
                else if (i < 12)
                    myGLCD.setColor(255, 0, 0);
                myGLCD.fillCircle(xPos + xWidth * i, yPos, radius);
            }
            else
            {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillCircle(xPos + xWidth * i, yPos, radius);
            }
        }
            oldCircleAmount = nowCircleAmount;
        }
        
        
        m_rpmOld = m_rpm;
        
    }
    

    

// Clear the screen and draw the frame


 counter++;
 counter = counter % 9;



}









void recvBytesWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3C;
    byte endMarker = 0x3E;
    byte rb;
   

    while (Serial.available() > 0 && newData == false) {
        rb = Serial.read();

        if (recvInProgress == true) {
            if (rb != endMarker) {
                receivedBytes[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                }
            }
            else {
                receivedBytes[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }

        else if (rb == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in (HEX values)... ");
        for (byte n = 0; n < numReceived; n++) {
            Serial.print(receivedBytes[n], HEX);
            Serial.print(' ');
        }
        Serial.println();
        newData = false;
    }
}


















