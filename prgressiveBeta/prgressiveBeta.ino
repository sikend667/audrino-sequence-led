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


#define LED_UP        0xFFC23D
#define LED_DOWN      0xFF22DD
#define UP_ARROW      0xFF906F //Increased number of degrees servo moves
#define DOWN_ARROW    0xFFE01F //Decrease number of degrees servo moves



// #define BUTTON_0 0xfd30cf  //Pushing buttons 0-9 moves to fixed positions
// #define BUTTON_1 0xfd08f7  // each 20 degrees greater
// #define BUTTON_2 0xfd8877
// #define BUTTON_3 0xfd48b7
// #define BUTTON_4 0xfd28d7
// #define BUTTON_5 0xfda857
// #define BUTTON_6 0xfd6897
// #define BUTTON_7 0xfd18e7
// #define BUTTON_8 0xfd9867
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

int maxNumLeds   = 8;
int numLedsInUse = 8;
byte dataArray[10];

int counter = 0;
byte inputOn;




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

// numLedsInUse = 8
inputOn = 0;

 // Always start by setting SRCLR high
 digitalWrite( clearPin, HIGH);

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
      case UP_ARROW:      ledSpeed = ledSpeedFunc(ledSpeed, (char*)"up"); break;
      case DOWN_ARROW:    ledSpeed = ledSpeedFunc(ledSpeed, (char*)"down"); break;
      case LED_UP:        numLedsInUse = ledChangeFunc(numLedsInUse, (char*)"led_up"); break;
      case LED_DOWN:      numLedsInUse = ledChangeFunc(numLedsInUse, (char*)"led_down"); break;
      }
      // myServo.write(pos); // tell servo to go to position in variable 'pos'
      Previous=myDecoder.value;
    }
    myReceiver.enableIRIn();
 }



 // Display LED's running
 if (counter <= numLedsInUse) {
  // Serial.println("===========");
  // Serial.println(String(inputOn, BIN));
  // Serial.println("===========");

  registerWrite(counter, HIGH, inputOn);

  counter++;
  inputOn = inputOn + round(pow(2.0, (numLedsInUse - counter)));
 }
 else {
  // registerWrite((counter - 1), HIGH, dataArray[counter]);
  // registerWrite((counter - 1), HIGH, (byte) pow(2, (counter + 1)));
  // inputOn = inputOn >> counter;
  counter = 0;
  inputOn = 0;

  // inputOn = inputOn + round(pow(2.0, (numLedsInUse - counter)));
  registerWrite(counter, HIGH, inputOn);

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
  Serial.println(speed);
  if (direction == "up") {
    if ((speed -7) > 0) {
      return speed - 7;
    }
    else {
      // @ToDo add solid LED state.
      // Current hardware can't handle < 7ms delay time.
      // Possible issues: IR commands are getting called mid loop and being duplicated.
      return 7;
    }
  }
  else {
    if ((speed +7) <= 2000) {
      return speed + 7;
    }
    else {
      return 2000;
    }
  }
}

int ledChangeFunc(int numLedsInUse, char* direction) {
  // Serial.println("=======");
  // Serial.println(numLedsInUse);
  // Serial.println(maxNumLeds);
  // Serial.println("=======");
  if (direction == "led_up") {
    Serial.println("led_up");
    if (numLedsInUse < maxNumLeds) {
      return (numLedsInUse + 1);
    }
    else {
      return maxNumLeds;
    }
  }
  else {
    Serial.println("led_down");
    if (numLedsInUse > 0) {
      return (numLedsInUse - 1);
    }
    else {
      return 0;
    }
  }
}
