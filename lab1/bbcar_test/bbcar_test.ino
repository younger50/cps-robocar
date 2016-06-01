/*
bbcar test
lab 1
*/

int carcmd = 0;   // 0:forward 1:right 2:left 3:hard right 4:hard left
int sPinR = A0;   // sensor R input
int sPinL = A1;   // sensor L input
int sPinF = A2;   // sensor Front input
int tPinR = A4;   // sensor R threshold 
int tPinL = A5;   // sensor L threshold 
int mPinR = 10;   // motor R pin
int mPinL = 11;   // motor L pin
int sValueR = 0;  // sensor value R
int sValueL = 0;  // sensor value L
int sValueF = 0;  // sensor value Front
int tValueR = 0;
int tValueL = 0;
unsigned long msR = 0;    // timer of right servo
int statR = 0;   // stat (0 in LOW, 1 in HIGH, 2 in speed delay control)
unsigned long tRH = 1350;   // timer thresthhold of right servo PWM HIGH signal (us)
unsigned long tRL = 60000;   // timer thresthhold of right servo PWM LOW signal (us)
unsigned long msL = 0;
int statL = 0;
unsigned long tLH = 1650;
unsigned long tLL = 60000;

//average sensor value
float sAvgR = 0;
float sSmpR = 20;

float sAvgL = 0;
float sSmpL = 20;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(mPinR, OUTPUT);
  pinMode(mPinL, OUTPUT);
  digitalWrite(mPinR, LOW);
  digitalWrite(mPinR, LOW);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sValueR = analogRead(sPinR);
  sValueL = analogRead(sPinL);
  sValueF = analogRead(sPinF);
  tValueR = analogRead(tPinR);
  tValueL = analogRead(tPinL);
  //calculate average sensor reading
  if(sAvgR == 0){
      sAvgR = sValueR;
  }
  else{
      sAvgR = sAvgR*(sSmpR - 1)/sSmpR + sValueR/sSmpR;  
  }
  if(sAvgL == 0){
      sAvgL = sValueL;
  }
  else{
      sAvgL = sAvgL*(sSmpL - 1)/sSmpL + sValueL/sSmpL;  
  }
  //debug log
  Serial.print("R: ");
  Serial.print(sValueR);
  Serial.print(" L: ");
  Serial.print(sValueL);
  Serial.print(" F: ");
  Serial.print(sValueF);
  Serial.print("  RA: ");
  Serial.print(sAvgR);
  Serial.print(" LA: ");
  Serial.print(sAvgL);
  Serial.print(" State: ");
  Serial.println( carcmd);
  //timer
  unsigned long time = micros();
  // Right turn
  if( sAvgR < 420){
    carcmd = 1;
  }
  // Right Hard 
  else if( sAvgR > 500){
    carcmd = 3;
  }
  // Left turn
  else if( sAvgL < 400){
    carcmd = 2;
  }
  // Left Hard
  else if( sAvgL > 480){
    carcmd = 4;
  }
  else if( carcmd==3 || carcmd==1){
    carcmd = 1;
  }
  else if( carcmd==4 || carcmd==2){
    carcmd = 2;
  }
  else{
    carcmd = 0;
  }
  // Forward
  if ( time - msR > tRH && statR == 1) {
    msR = time;
    statR = 0;
  }
  if ( time - msR > tRL && statR == 0 && carcmd!=1 && carcmd!=3 ) {
    digitalWrite(mPinR, HIGH);
    if( carcmd==3){
      delayMicroseconds(tLH);
    }
    else{
      delayMicroseconds(tRH);
    }
    digitalWrite(mPinR, LOW);
    msR = time;
    statR = 1;
  }
  if ( time - msL > tLH && statL == 1) {
    msL = time;
    statL = 0;
  }
  if ( time - msL > tLL && statL == 0 && carcmd!=2 && carcmd!=4 ) {
    digitalWrite(mPinL, HIGH);
    if( carcmd==4){
      delayMicroseconds(tRH);
    }
    else{
      delayMicroseconds(tLH);
    }
    digitalWrite(mPinL, LOW);
    msL = time;
    statL = 1;
  }
  delayMicroseconds(100);
}

