//ALL NUMBERS ARE AT FULL CHARGE BATTERY
#define RD 7
#define LD 4
#define RS 6
#define LS 5
#define BUTTON 12 //start button
//#define LBUMP 3 // left bumper pin
//#define RBUMP 9 // right bumper pin
#define IRCOMM 10 //IR reciever
#define LTL A0  //Linetracker Left
#define LTC A2  //Linetracker Centre
#define LTR A1  //Linetracker Right
#include <QSerial.h>
#include <Servo.h>

QSerial myIRserial;
Servo pan, tilt, grip;
int val=60;
int mode;
int receivedNum;
int LTHRESH = 950;//CALIBRATE!!!!
int CTHRESH = 950;//^^
int RTHRESH = 950;//^^
int s = 133;

void setup() {
  pinMode(RD,OUTPUT);
  pinMode(LD,OUTPUT);
  pinMode(RS,OUTPUT);
  pinMode(LS,OUTPUT);
  pinMode(LTL, INPUT);
  pinMode(LTC, INPUT);
  pinMode(LTR, INPUT);
  pinMode(BUTTON,INPUT);
 // pinMode(LBUMP,INPUT);
 // pinMode(RBUMP,INPUT);
  //IR Comm
  pinMode(IRCOMM,INPUT);
  myIRserial.attach(IRCOMM,-1);
  
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
  mode = -1;
  while(1){
    switch(mode){
      case(-1)://find next beacon from home
      {
        mode=find();
        break;
      };
      case(0):
      {
        driveToBeacon();
        delay(500);
        tilt.write(85);
        delay(500);
        while(val < 120){
        val+=10;
        grip.write(val);
        delay(100);
        }
        tilt.write(160);
        delay(500);
        driveToHome();
        delay(750);
        NinetyLeft();
        dropoff();  
        mode = -1;
        
        break;
      };
      case(1):
      {
        driveToBeacon();
        delay(500);
        tilt.write(85);
        delay(500);
        while(val < 120){
        val+=10;
        grip.write(val);
        delay(100);
        }
        tilt.write(160);
        delay(500);
        driveToHome();
        delay(750);
        dropoff();        
        mode = -1;

        break;
      };
      case(2):
      {
        driveToBeacon();
        delay(500);
        tilt.write(85);
        delay(500);
        while(val < 120){
        val+=10;
        grip.write(val);
        delay(100);
        }
        tilt.write(160);
        delay(500);
        driveToHome();
        delay(750);
        NinetyRight();
        dropoff();        
        mode = -1;
        
        break;
      };
    } 
  }
}

void driveToBeacon(){
  int leftVal = analogRead(LTL);
  int centreVal = analogRead(LTC);
  int rightVal = analogRead(LTR);
  while(1){
    leftVal = analogRead(LTL);
    centreVal = analogRead(LTC);
    rightVal = analogRead(LTR);
    Serial.print(leftVal);
    Serial.print(centreVal);
    Serial.print(rightVal);
    if((leftVal < LTHRESH)&&(centreVal > CTHRESH)&&(rightVal < RTHRESH)){
      digitalWrite(RD,HIGH);
      digitalWrite(LD,HIGH);
      analogWrite(RS,s);
      analogWrite(LS,s);
     }else if((leftVal > LTHRESH)&&(centreVal < CTHRESH)&&(rightVal < RTHRESH)){
      digitalWrite(RD,HIGH);
      digitalWrite(LD,HIGH);
      analogWrite(RS,(s+30));
      analogWrite(LS,(s-5));
     }else if((leftVal < LTHRESH)&&(centreVal < CTHRESH)&&(rightVal > RTHRESH)){
      digitalWrite(RD,HIGH);
      digitalWrite(LD,HIGH);
      analogWrite(RS,(s-5));
      analogWrite(LS,(s+30));
     }else if((leftVal < LTHRESH)&&(centreVal > CTHRESH)&&(rightVal > RTHRESH)){
      digitalWrite(RD,HIGH);
      digitalWrite(LD,HIGH);
      analogWrite(RS,s);
      analogWrite(LS,(s+15));
     }else if((leftVal > LTHRESH)&&(centreVal > CTHRESH)&&(rightVal < RTHRESH)){
      digitalWrite(RD,HIGH);
      digitalWrite(LD,HIGH);
      analogWrite(RS,(s+15));
      analogWrite(LS,s);
     }else if((leftVal > LTHRESH)&&(centreVal > CTHRESH)&&(rightVal > RTHRESH)){
      analogWrite(RS,0);
      analogWrite(LS,0);
      return;
     }
     delay(25); // may or may not need this
    }
}

void driveStop(){
  analogWrite(RS, 0);
  analogWrite(LS, 0);
}//end driveStop()

void NinetyRight(){
    digitalWrite(RD, LOW);
    digitalWrite(LD, HIGH);
    analogWrite(RS, s);
    analogWrite(LS, s);
  delay(617);
  driveStop();
  delay(250);
}

void NinetyLeft(){
  digitalWrite(RD, HIGH);
    digitalWrite(LD, LOW);
    analogWrite(RS, s);
    analogWrite(LS, s);
  delay(617);
  driveStop();
  delay(250); 
}

void dropoff(){
  driveToBeacon();
  delay(500);
  digitalWrite(RD, HIGH);
  digitalWrite(LD, HIGH);
  analogWrite(RS, 128);
  analogWrite(LS, 128); // test lol
  delay(210);
  
  tilt.write(120); // need to test this
  delay(500); // need to test this
  grip.write(60);
  delay(500); // test this
  tilt.write(160);
  delay(500); // ^^

  driveToHome();
}

int find(){
  int counter = 0;
  int tcount=2;
  while(1){
    receivedNum = myIRserial.receive(200);
  delay(200);
    if(receivedNum == 48 || receivedNum == 49 || receivedNum == 50){
      if(receivedNum == 48){
        return 0;
      }else if(receivedNum == 49){
        return 1;
      }else{
        return 2;
      }
    } else if (counter == 40){//calibrate counter for timing
      NinetyLeft();
      counter = 0;
    if(tcount==3){
      NinetyLeft();
      tcount=0;
    }
    tcount++;
    }else{
      counter++;
    }
  }
  return -1;
}

void driveToHome(){
  NinetyLeft();
  NinetyLeft();
    delay(750);
  driveToBeacon();
  analogWrite(RS, s);
  analogWrite(LS, s);
  delay(420);
  driveStop();
  }
