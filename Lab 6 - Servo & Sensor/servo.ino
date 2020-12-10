// Experiment 1

void setup() {
  // DDRD &= 0b11110011; // 2-3 as Input,
  DDRD |= 0b00010000; // 5 as Output

  PORTD &= 0b11101111; // Sets output to 0.

}

void loop() {
  PORTD |= 0b00001000; // Sets it on 
  delay(1);
  PORTD &= 0b11110111; // Sets it off
  delay(19);
}
