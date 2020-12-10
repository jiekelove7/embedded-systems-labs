#include <avr/io.h>

int a;
int b;

void setup() {
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
}

void loop() {
  int regD = PIND;
  
  // if button a is pressed
  if((regD & 0b00000100) == 0b00000100 && !b) {
    a = 1;
    PORTB = PORTB & 0b11111000;
    PORTB = PORTB | 0b00000111;
    PORTD = PORTD & 0b00001111;
    PORTD = PORTD | 0b01110000;
  }
  // if buttong b is pressed
  if((regD & 0b00001000) == 0b00001000 && !a) {
    b = 1;
    PORTB = PORTB & 0b11111000;
    PORTB = PORTB | 0b00000111;
    PORTD = PORTD & 0b00001111;
    PORTD = PORTD | 0b11000000;
  }
}
