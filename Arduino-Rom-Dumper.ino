/* Arduino pins 3/4/5/6/7 = ROM D3/D4/D5/D6/D7
 * Arduino pins 8/9/10 = ROM D0/D1/D2 
 */

const unsigned long SERIAL_SPEED = 115200;

const unsigned long ROM_SIZE = 64 * 1024UL;

// Delay between address write and data read (us)
const byte READ_DELAY = 50;

const byte SR_LATCH_PIN = A0;
const byte SR_DATA_PIN = A1;
const byte SR_CLOCK_PIN = A2;

void write_address (word address) {
  digitalWrite (SR_LATCH_PIN, LOW);
  shiftOut (SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, address >> 8);
  shiftOut (SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, address & 0xFF);
  digitalWrite (SR_LATCH_PIN, HIGH);
}

byte read_byte (word address) {
  write_address (address);
  delayMicroseconds (READ_DELAY);
  byte data = (PIND & 0xF8) | (PINB & 0x07);

  return data;
}

void setup () {
  // Data lines
  pinMode (3, INPUT);
  pinMode (4, INPUT);
  pinMode (5, INPUT);
  pinMode (6, INPUT);
  pinMode (7, INPUT);
  pinMode (8, INPUT);
  pinMode (9, INPUT);
  pinMode (10, INPUT);

  // SR control lines
  pinMode (SR_LATCH_PIN,  OUTPUT);
  pinMode (SR_DATA_PIN,  OUTPUT);
  pinMode (SR_CLOCK_PIN,  OUTPUT);
  digitalWrite (SR_LATCH_PIN, HIGH);

  // Pin
  pinMode (LED_BUILTIN, OUTPUT);

  // Serial port for I/O
  Serial.begin (SERIAL_SPEED);
}

void loop () {
  // Wait for a character on serial port
  bool start = false;
  while (!start) {
    digitalWrite (LED_BUILTIN, (millis () / 333) % 2);
    while (Serial.available ()) {
      Serial.read ();
      start = true;
    }
  }

  // Go!
  for (unsigned long i = 0; i < ROM_SIZE; i++) {
    byte b = read_byte ((word) i);
    Serial.write (b);
  }
}

