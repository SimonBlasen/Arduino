// Die Uhrzeit auslesen von der Uhr RTC3231
// Die Uhr ist sehr genau mit einem Temperatur kompensierten Takt
// Der Fehler beträgt +-2ppm bei 0-40°C oder max ca. 1 Minute pro Jahr
//
// Matthias Busse 6.11.2016 Version 1.0
#include "Wire.h"
#define DS3231_ADDRESSE 0x57
void setup() {
  Wire.begin();
  Serial.begin(38400);
  
  einstellenDS3231zeit(30, 34, 10, 2, 7,  11,  16);
}
void loop() {
  zeigeZeit(); // Zeit ausgeben
  delay(1000); // jede Sekunde
}

void einstellenDS3231zeit(byte sekunde, byte minute, byte stunde, byte wochentag, byte tag, byte monat, byte jahr) {
  // Datum und Uhrzeit einstellen
  Wire.beginTransmission(DS3231_ADDRESSE);
  Wire.write(0);
  Wire.write(decToBcd(sekunde)); // Sekunden einstellen
  Wire.write(decToBcd(minute)); // Minuten einstellen
  Wire.write(decToBcd(stunde));
  Wire.write(decToBcd(wochentag)); // 1=Sonntag ... 7=Samstag
  Wire.write(decToBcd(tag));
  Wire.write(decToBcd(monat));
  Wire.write(decToBcd(jahr)); // 0...99
  Wire.endTransmission();
}

void leseDS3231zeit(byte *sekunde, byte *minute,byte *stunde, byte *wochentag, byte *tag, byte *monat, byte *jahr) {
  Wire.beginTransmission(DS3231_ADDRESSE);
  Wire.write(0); // DS3231 Register zu 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_ADDRESSE, 7); // 7 Byte Daten vom DS3231 holen
  *sekunde = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *stunde = bcdToDec(Wire.read() & 0x3f);
  *wochentag = bcdToDec(Wire.read());
  *tag = bcdToDec(Wire.read());
  *monat = bcdToDec(Wire.read());
  *jahr = bcdToDec(Wire.read());
}
void zeigeZeit(){
byte sekunde, minute, stunde, wochentag, tag, monat, jahr;
  leseDS3231zeit(&sekunde, &minute, &stunde, &wochentag, &tag, &monat, &jahr);   // Daten vom DS3231 holen
  if (tag < 10) { Serial.print("0");} 
  Serial.print(tag); // ausgeben T.M.J H:M:S
  Serial.print(":");
  if (monat < 10) { Serial.print("0");}
  Serial.print(monat);
  Serial.print(":20");
  Serial.print(jahr);
  Serial.print(" ");
  if (stunde < 10) { Serial.print("0");}
  Serial.print(stunde, DEC); // byte in Dezimal zur Ausgabe
  Serial.print(":");
  if (minute < 10) { Serial.print("0");}
  Serial.print(minute, DEC);
  Serial.print(":");
  if (sekunde < 10) { Serial.print("0"); }
  Serial.println(sekunde, DEC);
}
byte decToBcd(byte val) {
// Dezimal Zahl zu binary coded decimal (BCD) umwandeln
  return((val/10*16) + (val%10));
}
byte bcdToDec(byte val) {
// BCD (binary coded decimal) in Dezimal Zahl umwandeln
  return((val/16*10) + (val%16));
}
