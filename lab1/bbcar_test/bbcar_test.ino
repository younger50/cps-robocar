/*
bbcar test
lab 1
*/

int carcmd = 0;   // 0:forward 1:right 2:left
int sPinR = A0;   // sensor R input
int sPinL = A1;   // sensor L input
int tPinR = A4;   // sensor R threshold 
int tPinL = A5;   // sensor L threshold 
int mPinR = 10;   // motor R pin
int mPinL = 11;   // motor L pin
int sValueR = 0;   // sensor value R
int sValueL = 0;   // sensor value L
int tValueR = 0;
int tValueL = 0;
unsigned long msR = 0;    // timer of right servo
int statR = 0;   // stat (0 in LOW, 1 in HIGH, 2 in speed delay control)
unsigned long tRH = 1400;   // timer thresthhold of right servo PWM HIGH signal (us)
unsigned long tRL = 60000;   // timer thresthhold of right servo PWM LOW signal (us)
unsigned long msL = 0;
int statL = 0;
unsigned long tLH = 1600;
unsigned long tLL = 60000;

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
  tValueR = analogRead(tPinR);
  tValueL = analogRead(tPinL);
  //led_diplay(sValueR>>1, sValueR>>2, sValueR>>3, sValueR>>4);
  //Serial.println(sValueR);
  unsigned long time = micros();
  if( sValueR > tValueR && sValueL < tValueL){
    carcmd = 1;
  }
  else if( sValueR < tValueR && sValueL > tValueL){
    carcmd = 2;
  }
  else{
    carcmd = 0;
  }
  if ( time - msR > tRH && statR == 1) {
    digitalWrite(mPinR, LOW);
    msR = time;
    statR = 0;
  }
  if ( time - msR > tRL && statR == 0 && carcmd!=1 ) {
    digitalWrite(mPinR, HIGH);
    msR = time;
    statR = 1;
  }
  if ( time - msL > tLH && statL == 1) {
    digitalWrite(mPinL, LOW);
    msL = time;
    statL = 0;
  }
  if ( time - msL > tLL && statL == 0 && carcmd!=2 ) {
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

