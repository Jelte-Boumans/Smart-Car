#include <Wire.h>
#include <NewPing.h>

#define IR A6

#define TRIG 1
#define ECHO 2

#define SDA A4
#define SCL A5

NewPing sonar(TRIG, ECHO);

byte lijnPins[] = {23, 24, 14, 13, 12, 11, 10, 9};
bool lijnWaardes[] = {};
byte irAfstand, ultrasonAfstand, lijnWaarde;

void setup() {
  Wire.begin(8);
  Wire.onRequest(request);
  
  pinMode(IR, INPUT);

  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);
  
  for(byte i = 0; i < 8; i++)
    pinMode(lijnPins[i], INPUT);
}

void loop() {
  irAfstand = map(analogRead(IR), 0,1023,0,255);
  ultrasonAfstand = sonar.ping_cm();

  for(byte i = 0; i < 8; i++)
    lijnWaardes[i] = digitalRead(lijnPins[i]);
}

void request() {
  for(byte i = 0; i < 8; i++)
    lijnWaarde |= lijnWaardes[i] << i;

  Wire.write(irAfstand);
  Wire.write(ultrasonAfstand);
  Wire.write(lijnWaarde);
}
