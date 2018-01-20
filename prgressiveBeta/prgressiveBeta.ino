#include <IRLibAll.h>
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


#define MY_PROTOCOL NEC



// @ToDo remap all button to constants.
#define RIGHT_ARROW   0xfd50af //Move several clockwise
#define LEFT_ARROW    0xfd10ef //Move servo counterclockwise
#define SELECT_BUTTON 0xfd906f //Center the servo



#define UP_ARROW      0xFF906F //Increased number of degrees servo moves
#define DOWN_ARROW    0xFFE01F //Decrease number of degrees servo moves



#define BUTTON_0 0xfd30cf  //Pushing buttons 0-9 moves to fixed positions
#define BUTTON_1 0xfd08f7  // each 20 degrees greater
#define BUTTON_2 0xfd8877
#define BUTTON_3 0xfd48b7
#define BUTTON_4 0xfd28d7
#define BUTTON_5 0xfda857
#define BUTTON_6 0xfd6897
#define BUTTON_7 0xfd18e7
#define BUTTON_8 0xfd9867
#define BUTTON_9 0xfd58a7



IRrecv myReceiver(4); //pin number for the receiver
IRdecode myDecoder;

// Servo myServo;  // create servo object to control a servo
int16_t pos;         // variable to store the servo position
int16_t Speed;       // Number of degrees to move each time a left/right button is pressed
uint32_t Previous;//handles NEC repeat codes




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



int ledSpeed = 150;

void setup() {

  // myServo.attach(9);      // attaches the servo on pin 9 to the servo object
  pos = 90;               // start at midpoint 90 degrees
  Speed = 3;              // servo moves 3 degrees each time left/right is pushed
  // myServo.write(pos);     // Set initial position
  myReceiver.enableIRIn(); // Start the receiver


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
//  delay(100);
}

void loop() {



  if (myReceiver.getResults()) {
    myDecoder.decode();
    if(myDecoder.protocolNum==MY_PROTOCOL) {
      if(myDecoder.value==0xFFFFFFFF)
        myDecoder.value=Previous;

//      Serial.println(myDecoder.value);
      Serial.println(myDecoder.value, HEX);
      switch(myDecoder.value) {
      case LEFT_ARROW:    pos=min(180,pos+Speed); break;
      case RIGHT_ARROW:   pos=max(0,pos-Speed); break;
      case SELECT_BUTTON: pos=90; break;
      case UP_ARROW:      ledSpeed = ledSpeedFunc(ledSpeed, (char*)"up"); break;
      case DOWN_ARROW:    ledSpeed = ledSpeedFunc(ledSpeed, (char*)"down"); break;
      case BUTTON_0:      pos=0*20; break;
      case BUTTON_1:      pos=1*20; break;
      case BUTTON_2:      pos=2*20; break;
      case BUTTON_3:      pos=3*20; break;
      case BUTTON_4:      pos=4*20; break;
      case BUTTON_5:      pos=5*20; break;
      case BUTTON_6:      pos=6*20; break;
      case BUTTON_7:      pos=7*20; break;
      case BUTTON_8:      pos=8*20; break;
      case BUTTON_9:      pos=9*20; break;
      }
      // myServo.write(pos); // tell servo to go to position in variable 'pos'
      Previous=myDecoder.value;
    }
    myReceiver.enableIRIn();
 }



 // Display LED's running
 if (counter < numLedsInUse) {
  registerWrite(counter, HIGH, dataArray[counter]);
  counter++;
 }
 else {
  registerWrite((counter - 1), HIGH, dataArray[counter]);
  counter = 0;
 }

 delay(ledSpeed);
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

int ledSpeedFunc(int speed, char* direction) {
  if (direction == "up") {
    if ((speed -5) >= 0) {
      return speed - 5;
    }
    else {
      return 0;
    }
  }
  else {
    if ((speed +5) <= 2000) {
      return speed + 5;
    }
    else {
      return 2000;
    }
  }
}











