#define RD 5
#define LD 6
#define RS 4
#define LS 7
#define BUTTON 12
#define LBUMP 10 // left bumper pin
#define RBUMP 9 // right bumper pin
#define distance A4
#include <Servo.h>
const int DISTBUMP = 525;
int val=60;
Servo pan, tilt, grip;
int mode;
int receivedNum;

void setup() {
  pinMode(RD,OUTPUT);
  pinMode(LD,OUTPUT);
  pinMode(RS,OUTPUT);
  pinMode(LS,OUTPUT);
  pinMode(distance, INPUT);
  //pinMode(encodeR,INPUT);
  //pinMode(encodeL,INPUT);
  pinMode(BUTTON,INPUT);
  pinMode(LBUMP,INPUT);
  pinMode(RBUMP,INPUT);
  //attachInterrupt(1,encode,CHANGE);
  Serial.begin(9600);
  pan.attach(8);
  tilt.attach(11);
  grip.attach(13);
  
  while(digitalRead(BUTTON)){}
  delay(50);
  while(!digitalRead(BUTTON)){} 
  
  pan.write(90);    //gripper arm centred
  delay(100);
  tilt.write(160);  //gripper arm in semi-vertical position
  delay(100);
  grip.write(val);   //grip fully open
  delay(100);

}

void loop() {
  driveForwardBeacon();
  delay(1000);
  tilt.write(90);
  delay(1000);
  while(val < 120){
        val+=10;
        grip.write(val);
        delay(500);
        }
  delay (1000);
  tilt.write(160);
  while(1){ 
  }
}

void driveForwardBeacon(){
  
  digitalWrite(RD, HIGH);
  digitalWrite(LD, HIGH);
  analogWrite(RS, 128);
  analogWrite(LS, 255);
  while(1){
  int x=analogRead(distance);//0 to 1023
  
  if (x > DISTBUMP)
  {
    driveStop();
    delay(1000);
    digitalWrite(RD, HIGH);
    digitalWrite(LD, HIGH);
    analogWrite(RS, 0);
    analogWrite(LS, 255);
    delay(150);
    driveStop();
    break;
  }     
  }  
}//end driveForwardsBeacon

void driveStop(){
  analogWrite(RS, 0);
  analogWrite(LS, 0);
}//end driveStop()
