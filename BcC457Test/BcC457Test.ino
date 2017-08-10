
#define PIN            48




int delayval = 1000;








void setup() {
  pinMode(PIN, OUTPUT);      // sets the digital pin as output
}

void loop() {
  
  digitalWrite(PIN, HIGH);   // sets the LED on

  delay(delayval); // Delay for a period of time (in milliseconds).
  
  digitalWrite(PIN, LOW);   // sets the LED off

  delay(delayval); // Delay for a period of time (in milliseconds).
}
