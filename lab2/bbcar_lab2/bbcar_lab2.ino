/*
bbcar test
lab 1
*/

int carcmd = 0;   // 0:forward 1:right 2:left 3:hard right 4:hard left
int sPinR = A0;   // sensor R input
int sPinL = A1;   // sensor L input
int sPinF = A2;   // sensor Front input
int mPinR = 10;   // motor R pin
int mPinL = 11;   // motor L pin

int sValueR = 0;  // sensor value R
int sValueL = 0;  // sensor value L
int sValueF = 0;  // sensor value Front

//average sensor value
float sAvgR = 0;
float sSmpR = 3;

float sAvgL = 0;
float sSmpL = 3;

float sAvgF = 0;
float sSmpF = 3;


unsigned long msR = 0;    // timer of right servo
int statR = 0;   // stat (0 in LOW, 1 in HIGH, 2 in speed delay control)
unsigned long tRH = 1350;   // timer thresthhold of right servo PWM HIGH signal (us)
unsigned long tRL = 60000;   // timer thresthhold of right servo PWM LOW signal (us)
unsigned long tRLUB = 80000; // upper bound, slowest speed
unsigned long tRLLB = 40000; // lower bound, faster speed

unsigned long msL = 0;
int statL = 0;
unsigned long tLH = 1650;
unsigned long tLL = 60000;
unsigned long tLLUB = 80000; // upper bound, slowest speed
unsigned long tLLLB = 40000; // lower bound, faster speed

unsigned long tSStep = 200; // speed step of delay time

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
  if(sAvgF == 0){
      sAvgF = sValueF;
  }
  else{
      sAvgF = sAvgF*(sSmpF - 1)/sSmpF + sValueF/sSmpF;  
  }
  //debug log
  Serial.print("R: ");
  Serial.print(sValueR);
  Serial.print(" L: ");
  Serial.print(sValueL);
  Serial.print(" F: ");
  Serial.print(sValueF);
  Serial.print("   RA: ");
  Serial.print(sAvgR);
  Serial.print(" LA: ");
  Serial.print(sAvgL);
  Serial.print(" FA: ");
  Serial.print(sAvgF);
  Serial.print(" State: ");
  Serial.println( carcmd);
  // Speed control
  if(sAvgF > 250){
      tRL = tRLUB;
      tLL = tLLUB;
  }
  else if(sAvgF < 100){
      tRL = tRLLB;
      tLL = tLLLB;
  }
  else if(sAvgF < 200){
      tRL -= tSStep;
      tLL -= tSStep;
  }
  
  // Turning state machine
  // Right turn
  if( sAvgR < 420){
    carcmd = 1;
  }
  // Right Hard 
  if( sAvgR > 550){
    carcmd = 3;
  }
  // Left turn
  else if( sAvgL < 400){
    carcmd = 2;
  }
  // Left Hard
  else if( sAvgL > 530){
    carcmd = 4;
  }
  // Continue turn if no threshold met
//  else if( carcmd==3){
//    carcmd = 1;
//  }
//  else if( carcmd==4){
//    carcmd = 2;
//  }
  // Forward
  else{
    carcmd = 0;
  }
  //timer
  unsigned long time = micros();
  //motor signal control
  if ( time - msR > tRH && statR == 1) {
    msR = time;
    statR = 0;
  }
  if ( time - msR > tRL && statR == 0 && carcmd!=1 ) {
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

