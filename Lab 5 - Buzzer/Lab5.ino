#include <avr/io.h>

#define A_PIN 2
#define B_PIN 3

int a;
int b;

void setup() {
  // Recommended not to use this method
  // See PT_2 / PT_3 for recommended method
  pinMode(A_PIN, INPUT); 
  pinMode(B_PIN, INPUT);
  // dont do this
  for(int i = 4; i < 11; i++) {
    pinMode(i, OUTPUT);
    if(i != 10) {
      digitalWrite(i, HIGH);
    }
  }
  
  a = 0;
  b = 0;
  
}

void loop() {
  int a_in = digitalRead(A_PIN);
  int b_in = digitalRead(B_PIN);

  if(a_in && !b) { // 7 is not on i.e. b is on
    a = 1;
    for(int i = 4; i < 11; i++) {
      if(i == 7) {
        digitalWrite(i, LOW);
        continue;
      }
      digitalWrite(i, HIGH);
    }
  }

  if(b_in && !a) {
    b = 1;
    for(int i = 4; i < 11; i++) {
      if(i < 6) {
        digitalWrite(i, LOW);
        continue;
      }
      digitalWrite(i, HIGH);
    }
  }
}
