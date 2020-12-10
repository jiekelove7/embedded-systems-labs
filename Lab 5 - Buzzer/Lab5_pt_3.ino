#include <avr/io.h>

int a;
int b;
int a_pressed = LOW;
int b_pressed = LOW;
int disqualified = LOW;

void setup() {
  // put your setup code here, to run once:
  DDRD = 0b11110000; // Notable pins: 2/3 as input, 4 - 7 as output
  DDRD = DDRD | 0b11110000; // Sets pins 4 - 7 as Output
  DDRD = DDRD & 0b11110011; // Sets pins 3 - 4 as Intput
  PORTD = PORTD & 0b00001111; // Sets outputs as 0
  
  DDRB = DDRB | 0b00000111; // Notable pins: 0 - 2 as output
  PORTB = PORTB & 0b11111000; // Sets output values to 0

  PORTD = PORTD | 0b11110000;
  PORTB = PORTB | 0b00000011;

  a = 0;
  b = 0;
  attachInterrupt(digitalPinToInterrupt(2), disq, RISING);
  attachInterrupt(digitalPinToInterrupt(3), disq, RISING);
  int rdm = random(10, 51);
  delay(rdm * 100);
  detachInterrupt(digitalPinToInterrupt(2));
  detachInterrupt(digitalPinToInterrupt(3));

  if(!disqualified) {
    PORTD = PORTD & 0b00001111;
    PORTB = PORTB & 0b11111000;
    PORTD = PORTD | 0b11110000;
    PORTB = PORTB | 0b00000111;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int regD = PIND;
  
  // if button a is pressed
  if((regD & 0b00000100) == 0b00000100 && !b && !disqualified) {
    a = 1;
    PORTB = PORTB & 0b11111000;
    PORTB = PORTB | 0b00000111;
    PORTD = PORTD & 0b00001111;
    PORTD = PORTD | 0b01110000;
  }
  // if buttong b is pressed
  if((regD & 0b00001000) == 0b00001000 && !a && !disqualified) {
    b = 1;
    PORTB = PORTB & 0b11111000;
    PORTB = PORTB | 0b00000111;
    PORTD = PORTD & 0b00001111;
    PORTD = PORTD | 0b11000000;
  }
}

void disq() {
  int regD = PIND;
  if((regD & 0b00000100) == 0b00000100 && !b) {
    a = 1;
    PORTB = PORTB & 0b11111000;
    PORTD = PORTD & 0b00001111;
    PORTD = PORTD | 0b01100000;
  }
  if((regD & 0b00001000) == 0b00001000 && !a) {
    b = 1;
    PORTB = PORTB & 0b11111000;
    PORTB = PORTB | 0b00000101;
    PORTD = PORTD & 0b00001111;
    PORTD = PORTD | 0b10110000;
  }  
  disqualified = HIGH;
}
