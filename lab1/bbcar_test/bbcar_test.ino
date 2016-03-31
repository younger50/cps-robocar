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

int sPinR = A0;   // sensor R input
int sPinL = A1;   // sensor L input
int tPin = A5;   // sensor threshold pin
int mPinR = 10;
int mPinL = 11;
int sValueR = 0;   // sensor value R
int sValueL = 0;   // sensor value L
int tValue = 0;
int msR = 0;    // timer of right servo
int statR = 0;   // stat (0 in LOW, 1 inHIGH)
int tRH = 1300;   // timer thresthhold of right servo PWM HIGH signal
int tRL = 20000;   // timer thresthhold of right servo PWM LOW signal
int msL = 0;
int statL = 0;
int tLH = 1700;
int tLL = 20000;

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
  tValue = analogRead(tPin);
  //led_diplay(sValueR>>1, sValueR>>2, sValueR>>3, sValueR>>4);
  //Serial.println(sValueR);
  int time = micros();
  if ( time - msR > tRH && statR == 1) {
    digitalWrite(mPinR, LOW);
    msR = time;
    statR = 0;
  }
  if ( time - msR > tRL && statR == 0 && sValueR > tValue) {
    digitalWrite(mPinR, HIGH);
    msR = time;
    statR = 1;
  }
  if ( time - msL > tLH && statL == 1) {
    digitalWrite(mPinL, LOW);
    msL = time;
    statL = 0;
  }
  if ( time - msL > tLL && statL == 0 && sValueL > tValue) {
    digitalWrite(mPinL, HIGH);
    msL = time;
    statL = 1;
  }
  delayMicroseconds(10);
}

int led_diplay(int v1, int v2, int v3, int v4) {
    int pin1 = 2;
    int pin2 = 3;
    int pin3 = 4;
    int pin4 = 5;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
    digitalWrite(pin1, v1);
    digitalWrite(pin2, v2);
    digitalWrite(pin3, v3);
    digitalWrite(pin4, v4);
}

