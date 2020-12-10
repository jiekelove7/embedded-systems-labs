// Experiment 3
// Code reliant on specific pin mapping.
// All binary number codes must be rewritten according to
// custom pin mapping.

#define DIVISOR 11 // Used to change servo reading into appropriate reading
                   // Adjst value as required

int input;

void setup() {
  
  DDRC = 0b00000000;

  //ADMUX = 0b01100000; //ADC0
  //ADCSRA |= 0b10000111;

  ADMUX = (0<<REFS1) | (1<<REFS0) | (1<<ADLAR) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
 
  DDRD |= 0b11111100;
  DDRB |= 0b00011111;
  PIND &= 0b00000011;
  //PINB &= 0b11100000;
  PINB &= 0b11111110;
  

  
  DDRB |= 0b00000010;
  TCCR1A |= 0b10000010;
  TCCR1A &= 0b10111110;
  TCCR1B |= 0b00011011;
  TCCR1B &= 0b11111011;
  ICR1 = 4999; // TOP of 20ms period
  OCR1A = 249; // TOP of 1ms period
  
  // For debugging purposes
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  
  ADCSRA |= (1 << ADSC);
  
  

  while((ADCSRA & (1 << ADIF)) == 0) {
  }
  input = ADCH;

  int timer = 249 + (input / 160) * 250;
  OCR1A = timer;

  delay(25); // Delay to more easily observe readings
  Serial.println(input / DIVISOR); // 
  PIND &= 0b00000011;
  PINB &= 0b11100000;
  // Case for each possible value to be shown
  // Displayed into a value on the display
  switch((input / DIVISOR) - 1) {
    case 0:
      PIND |= 0b00011000;
      //PINB |= 0b00000010;
      break;
    case 1:
      PIND |= 0b01101100;
      PINB |= 0b00000001;
      //PINB |= 0b00000100;
      break;
    case 2:
      PIND |= 0b00111100;
      PINB |= 0b00000001;
      //PINB |= 0b00000110;
      break;
    case 3:
      PIND |= 0b10011000;
      PINB |= 0b00000001;
      //PINB |= 0b00001000;
      break;
    case 4:
      PIND |= 0b10110100;
      PINB |= 0b00000001;
      //PINB |= 0b00001010;
      break;
    case 5:
      PIND |= 0b11110100;
      PINB |= 0b00000001;
      //PINB |= 0b00001100;
      break;
    case 6:
      PIND |= 0b00011100;
      //PINB |= 0b00001110;
      break;
    case 7:
      PIND |= 0b11111100;
      PINB |= 0b00000001;
      //PINB |= 0b00010000;
      break;
    case 8:
      PIND |= 0b10111100;
      PINB |= 0b00000001;
      //PINB |= 0b00010010;
      break;
    case 9:
      PIND |= 0b11011100;
      PINB |= 0b00000001;
      //PINB |= 0b00010100;
      break;
    case 10:
      PIND |= 0b11110000;
      PINB |= 0b00000001;
      //PINB |= 0b00010110;
      break;
    case 11:
      PIND |= 0b11100100;
      //PINB |= 0b00011000;
      break;
    case 12:
      PIND |= 0b01111000;
      PINB |= 0b00000001;
      //PINB |= 0b00011001;
      break;
    case 13:
      PIND |= 0b11000100;
      PINB |= 0b00000001;
      //PINB |= 0b00011010;
      break;
    case 14:
      PIND |= 0b11000100;
      PINB |= 0b00000001;
      //PINB |= 0b00011011;
      break;
    default: // If value is over, just read as maximum possible value
      PIND |= 0b11000100;
      PINB |= 0b00000001;
      //PINB |= 0b00011100;
    
  }
  
  
  
}
