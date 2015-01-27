#define Motora_f 2
#define Motora_b 3
#define Motora_e 4
#define Motorb_f 5
#define Motorb_b 6
#define Motorb_e 7
#define Motor_start 100
#define Motor_max 255

#include <RF24.h>
#include <SPI.h>
#include <RF24Network.h>

RF24 radio(8,9);

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
     else if(states[0] == 5)
     {
       mtrstp();
     }
     
}
void motorinit()
{
  pinMode(Motora_f,OUTPUT);
  pinMode(Motora_b,OUTPUT);
  pinMode(Motorb_f,OUTPUT);
  pinMode(Motorb_b,OUTPUT);
  /* Below is the arming procedure for the motors to initialize */
  analogWrite(Motora_f,Motor_start);
  analogWrite(Motora_b,Motor_start);
  analogWrite(Motorb_f,Motor_start);
  analogWrite(Motorb_b,Motor_start);
  digitalWrite(Motora_e,HIGH);
  digitalWrite(Motorb_e,HIGH);

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

