/*
  Analog Input
  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED)  connected to digital pin 13.
  The amount of time the LED will be on and off depends on
  the value obtained by analogRead().

  The circuit:
   Potentiometer attached to analog input 0
   center pin of the potentiometer to the analog pin
   one side pin (either one) to ground
   the other side pin to +5V
   LED anode (long leg) attached to digital output 13
   LED cathode (short leg) attached to ground

   Note: because most Arduinos have a built-in LED attached
  to pin 13 on the board, the LED is optional.


  Created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput

*/

int sPinR = A0;
int sPinL = A1;
int mPinR = 10;
int mPinL = 11;
int sValueR = 0;   // sensor value R
int sValueL = 0;   // sensor value L
int msR = 0;    // timer of right servo
int statR = 0;   // stat (0 in LOW, 1 inHIGH)
int tRH = 1300;   // timer thresthhold of right servo PWM HIGH signal
int tRL = 700;   // timer thresthhold of right servo PWM LOW signal
int msL = 0;
int statL = 0;
int tLH = 1700;
int tLL = 300;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(mPinR, OUTPUT);
  pinMode(mPinL, OUTPUT);
  digitalWrite(mPinR, LOW);
  digitalWrite(mPinR, LOW);
  //Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sValueR = analogRead(sPinR);
  sValueL = analogRead(sPinL);
  //Serial.println(sValueR);
  int time = micros();
  if ( time - msR > tRH && statR == 1) {
    digitalWrite(mPinR, LOW);
    msR = time;
    statR = 0;
  }
  if ( time - msR > tRL && statR == 0 && sValueR > 500) {
    digitalWrite(mPinR, HIGH);
    msR = time;
    statR = 1;
  }
  if ( time - msL > tLH && statL == 1) {
    digitalWrite(mPinL, LOW);
    msL = time;
    statL = 0;
  }
  if ( time - msL > tLL && statL == 0 && sValueL > 500) {
    digitalWrite(mPinL, HIGH);
    msL = time;
    statL = 1;
  }
  delayMicroseconds(10);
}
