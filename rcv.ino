#define Motora_f 2
#define Motora_b 3
#define Motora_e 4

#define Motorb_f 5
#define Motorb_b 6
#define Motorb_e 7
/* 
 * LEAVE 8 and 9 PINS ALONE.
 * CE 8
 * CSN 9
 * SCK 52
 * MOSI 51
 * MISO 50
 */
#define arm1 22
#define arm2 23
#define arm_enable 24

#define hand1 25
#define hand2 26
#define hand_enable 27

#define Motor_start 100
#define Motor_max 255

#include <RF24.h>
#include <SPI.h>
#include <RF24Network.h>

RF24 radio(8,9);
// CE 8
// CSN 9
// SCK 52
// MOSI 51
// MISO 50
const uint64_t pipe = 0xE8E8F0F0E1LL;
uint8_t states[2];

void setup()
{
  Serial.begin(115200);
  Serial.println("On Board Controlling System");
  radio.begin();
  radio.printDetails();
  radio.openReadingPipe(1, pipe);
  radio.startListening();

}

void loop()
{
  bool done; 
  if(radio.available())
  {
    done = false;
    while(!done)
    {
         done = radio.read(states,2);
    }
  }
    if(states[0] == 1)
     { 
       mtrfw(); 
     }
     else if(states[0] == 2)
     {
       mtrrotR();
     }
     else if(states[0] == 3)
     {
      mtrrotL(); 
     }
     else if(states[0] == 4)
     {
       mtrbw();
     }
     else
     {
       mtrstp();
     }
     
     if(states[1] == 1) {
      // grab();
       stop_arm();  //  added
       stop_hand();  // added
       Serial.println("grabbing that stuff ;-)");
     }
     else if(states[1] == 2) {
      // loose();
        grab();
       Serial.println("looose your body baby B-)");
     }
     else {
       stop_hand();
       Serial.println("Talk to the hand");
     }
     
     if(states[1] == 3){
      // arm_front();
        arm_back();
       Serial.println("HAND FRONT");
     }
     else if(states[1] == 4){
      //  arm_back();
        arm_front();
        Serial.println("HAND BACK"); 
     }
     else {
        stop_arm();
        Serial.println("HAND BLOCKED");
     }
}
void motorinit()
{
  pinMode(Motora_f,OUTPUT);
  pinMode(Motora_b,OUTPUT);

  pinMode(Motorb_f,OUTPUT);
  pinMode(Motorb_b,OUTPUT);

  pinMode(Motora_e, OUTPUT);
  pinMode(Motorb_e, OUTPUT)
  ;
  pinMode(arm1, OUTPUT);
  pinMode(arm2, OUTPUT);
  
  pinMode(hand1,OUTPUT);
  pinMode(hand2, OUTPUT);
  
  pinMode(hand_enable,OUTPUT);
  pinMode(arm_enable, OUTPUT);
  
  /* Below is the arming procedure for the motors to initialize */
  digitalWrite(Motora_e,HIGH);
  digitalWrite(Motorb_e,HIGH);
  digitalWrite(hand_enable, HIGH);
  digitalWrite(arm_enable, HIGH);

}

void mtrfw()
{
      // Motor1
      digitalWrite(Motora_f,HIGH);
      digitalWrite(Motora_b,LOW);
      Serial.println("Forward");
      //Motor2
      digitalWrite(Motorb_f,HIGH);
      digitalWrite(Motorb_b,LOW);  
   
}
void mtrbw()
{
      // Motor1
      digitalWrite(Motora_f,LOW);
      digitalWrite(Motora_b,HIGH);
      Serial.println("Back");
      //Motor2
      digitalWrite(Motorb_f,LOW);
      digitalWrite(Motorb_b,HIGH);   
   
}
void mtrrotR()
{
      // Motor1
      digitalWrite(Motora_f,HIGH);
      digitalWrite(Motora_b,LOW);
      Serial.println("Right");
      //Motor2
      digitalWrite(Motorb_f,LOW);
      digitalWrite(Motorb_b,HIGH);   
   
}
void mtrrotL()
{
      // Motor1
      digitalWrite(Motora_f,LOW);
      digitalWrite(Motora_b,HIGH);
      Serial.println("Left");
      //Motor2
      digitalWrite(Motorb_f,HIGH);
      digitalWrite(Motorb_b,LOW);   
   
}
void mtrstp()
{
      // Motor1
      digitalWrite(Motora_f,LOW);
      digitalWrite(Motora_b,LOW);
      Serial.println("Stay");
      //Motor2
      digitalWrite(Motorb_f,LOW);
      digitalWrite(Motorb_b,LOW);  
   
}
void loose(void) {
      digitalWrite(hand1, HIGH);
      digitalWrite(hand2, LOW); 
}

void grab(void) {
      digitalWrite(hand1, HIGH);
      digitalWrite(hand2, LOW);
}


void arm_front(void){
      digitalWrite(arm1, HIGH);
      digitalWrite(arm2, LOW);
}


void arm_back(void){
      digitalWrite(arm1, LOW);
      digitalWrite(arm2, HIGH);
}

void stop_arm(void){
      digitalWrite(arm1, LOW);
      digitalWrite(arm2, LOW); 
}

void stop_hand(void){
      digitalWrite(hand1, LOW);
      digitalWrite(hand2, LOW); 
}