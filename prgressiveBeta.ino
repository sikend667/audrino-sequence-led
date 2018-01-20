/*
Shift Register Example
for TPIC6B595 shift register by Jens C Brynildsen

This sketch turns reads serial input and uses it to set the pins
of a TPIC6B595 shift register.

Hardware:
* TPIC6B595 shift register attached to pins 7, 8, 11 and 12 of the Arduino,
as detailed below.
* LEDs attached to each of the outputs of the shift register

Based on the example created 23 Mar 2010 by Tom Igoe

*/

//Pin to clear the register
const int clearPin = 7;
//Pin connected to latch pin (RCK) of TPIC6B595
const int latchPin = 8;
//Pin connected to clock pin (SRCK) of TPIC6B595
const int clockPin = 12;
////Pin connected to Data in (SER IN) of TPIC6B595
const int dataPin = 11;

int counter = 0;
int numLedsInUse = 8;
byte dataArray[10];

void setup() {
 //set pins to output because they are addressed in the main loop
 pinMode(clearPin, OUTPUT);
 pinMode(latchPin, OUTPUT);
 pinMode(dataPin, OUTPUT);  
 pinMode(clockPin, OUTPUT);
 Serial.begin(9600);

// Determain which LEDS are on / off start with all off.
dataArray[0] = 0x00; //0b00000000
dataArray[1] = 0x80; //0b10000000
dataArray[2] = 0xC0; //0b11000000
dataArray[3] = 0xE0; //0b11100000
dataArray[4] = 0xF0; //0b11110000
dataArray[5] = 0xF8; //0b11111000
dataArray[6] = 0xFC; //0b11111100
dataArray[7] = 0xFE; //0b11111110
dataArray[8] = 0xFF; //0b11111111
  
 // Always start by setting SRCLR high
 digitalWrite( clearPin, HIGH);

 // delay a little and then set 
 delay(100);
}

void loop() {
 // Display LED's running
 if (counter < numLedsInUse) {
  registerWrite(counter, HIGH, dataArray[counter]);
  counter++;
 }
 else {
  registerWrite((counter - 1), HIGH, dataArray[counter]);
  counter = 0;
 }
 
 delay(200);
}

// This method sends bits to the shift register:
void registerWrite(int whichPin, int whichState, byte data) {
  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);
  // shift the bits out
  shiftOut(dataPin, clockPin, MSBFIRST, data);

  // turn on the output so the LEDs can light up:
     digitalWrite(latchPin, HIGH);
}



