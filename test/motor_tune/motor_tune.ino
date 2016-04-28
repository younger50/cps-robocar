
int sPinR = A0;   // sensor R input
int sPinL = A1;   // sensor L input

int mPinR = 10;   // motor R pin
int mPinL = 11;   // motor L pin

int sValueR = 0;  // sensor value R
int sValueL = 0;  // sensor value L

//average sensor value
float sAvgR = 0;
float sSmpR = 3;
float sAvgL = 0;
float sSmpL = 3;

//motor pulse control speed
int pulseR = 1300;
int pulseL = 1700;

void setup() {
  pinMode(mPinR, OUTPUT);
  pinMode(mPinL, OUTPUT);
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
  //forward
  digitalWrite(mPinR, HIGH);
  digitalWrite(mPinL, HIGH);
  delayMicroseconds(pulseR);
  digitalWrite(mPinR, LOW);
  delayMicroseconds(pulseL-pulseR);
  digitalWrite(mPinL, LOW);
}
