#include <Servo.h>

//define swithes
#define s1 4
#define s2 5
#define s3 6
#define s4 7
#define s5 8
#define s6 9
#define s7 A0
#define s8 A1
#define s9 A2
#define s10 3
#define numSwithes 10

//dc motor pins
#define ch1 A4
#define ch2 A5

#define relay 2
#define servoPIN 11

short int time_a = 29; // time action dc between 2 switches(41)
int maskSwitches[10];
String maska = "";
short int modeWork = 0;
short int previousSwitch = 0;

Servo MyServo;

void setup() {
  MyServo.attach(servoPIN);
  
  pinMode(ch1,OUTPUT);
  pinMode(ch2,OUTPUT);
  /*
    0 - user push
    1 - don`t push
  */
  pinMode(s1,INPUT_PULLUP);
  pinMode(s2,INPUT_PULLUP);
  pinMode(s3,INPUT_PULLUP);
  pinMode(s4,INPUT_PULLUP);
  pinMode(s5,INPUT_PULLUP);
  pinMode(s6,INPUT_PULLUP);
  pinMode(s7,INPUT_PULLUP);
  pinMode(s8,INPUT_PULLUP);
  pinMode(s9,INPUT_PULLUP);
  pinMode(s10,INPUT_PULLUP);

  pinMode(relay, OUTPUT);
  
  MyServo.write(0);
  
  digitalWrite(relay, 0); 
}

void loop() {
  ScanSwitches();
  
  switch(modeWork) {
    case 1: {
      digitalWrite(relay, 1);
    }
    break;
    case 2: {
      digitalWrite(relay, 0); 
      int count = 0;
      
      for(int i = 0; i < numSwithes; i++) {
        if(maskSwitches[i] == 0) {
          DC_Action_Forward(i, previousSwitch, count);
          Servo_Action();
          
          previousSwitch = i+1;
          count++;  
        }   
      }
      
      DC_Action_Backward();
      maska = "";
    }
    break;
    default: modeWork = 0;
  }  
}

void Servo_Action(void) {
  MyServo.write(145);
  delay(660);      

  MyServo.write(40);
  delay(660); 
}

void DC_Action_Backward(void) {
  digitalWrite(ch1,1);
  digitalWrite(ch2,0);
  delay(400);//40

  digitalWrite(ch1,0);
  delay(50);  
}

void DC_Action_Forward(int switchNumber, int prevSwitch, int countSwitch) {
  float timeAction = 0;
  
  if(countSwitch == 0) { 
    int a = time_a - 1;

    timeAction = 1.03 * a;
    timeAction = timeAction*(switchNumber + 1);
  }
  else {
    timeAction = 1.03 * (time_a - 1);
    timeAction = timeAction * (switchNumber - prevSwitch + 1) + 13;
  }
    
  digitalWrite(ch1,0);
  digitalWrite(ch2,1);
  delay(timeAction);//41

  digitalWrite(ch2,0);
  delay(50);
  
  timeAction = 0;
  prevSwitch = 0; 
  previousSwitch = 0;
}

void DC_Action_Forward_First_Switch(void) {
  digitalWrite(ch1,0);
  digitalWrite(ch2,1);
  delay(15);

  digitalWrite(ch2,0);
  delay(50);
}

void ScanSwitches(void) {
  maska = "";
  delay(500);
  maskSwitches[0] = digitalRead(s1);
  maskSwitches[1] = digitalRead(s2);
  maskSwitches[2] = digitalRead(s3);
  maskSwitches[3] = digitalRead(s4);
  maskSwitches[4] = digitalRead(s5);
  maskSwitches[5] = digitalRead(s6);
  maskSwitches[6] = digitalRead(s7);
  maskSwitches[7] = digitalRead(s8);
  maskSwitches[8] = digitalRead(s9);
  maskSwitches[9] = digitalRead(s10);
  
  for(int i = 0; i < numSwithes; i++) {
    maska += maskSwitches[i];
  }

  if(maska == "1100110011") {
     modeWork = 1;   
  }
  else if(maska == "1111111111") {
    modeWork = 0;
  }
  else {
    modeWork = 2;
  } 
}

