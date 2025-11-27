#include <Servo.h>
int val = 0 ;  
Servo servo1;
Servo servo2;
Servo servo3;

#define s0 4        
#define s1 5
#define s2 6
#define s3 7
#define out 8

#define LED_R A0  
#define LED_G A1
#define LED_B A2

int Red=0, Blue=0, Green=0;

void setup() 
{   
// Attach servo motors to pins
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  
  // Set initial positions for servo motors
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  
  pinMode(LED_R,OUTPUT);      
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);
  
  pinMode(s0,OUTPUT);    
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  
  Serial.begin(9600);   
  digitalWrite(s0,HIGH);  
  digitalWrite(s1,HIGH);    
}

void loop()
{ 
  GetColors();
  if   (Red<Blue && Red<Green && Red<=18){
       Serial.println("Red");
       digitalWrite(LED_R, HIGH);   
    servo1.write(10);
    delay(2000);
    servo3.write(170);
    delay(2000);
    servo1.write(90);
    delay(2000);
    servo2.write(0);
    delay(2000);
    servo1.write(20);
    delay(2000);
    servo3.write(0);
    delay(2000);
    servo1.write(90);
    delay(2000);
    servo2.write(90);
    delay(2000);
  }  
    
  else{
    digitalWrite(LED_R, LOW); 
  }
  
  if   (Blue<Green && Blue<Red && Blue<20) {
    Serial.println("Blue");
    digitalWrite(LED_B, HIGH); 
    servo1.write(10);
    delay(2000);
    servo3.write(170);
    delay(2000);
    servo1.write(90);
    delay(2000);
    servo2.write(179);
    delay(2000);
    servo1.write(20);
    delay(2000);
    servo3.write(0);
    delay(2000);
    servo1.write(90);
    delay(2000);
    servo2.write(90);
    delay(2000);
  }
    
   else{
     digitalWrite(LED_B, LOW);
   }
  
   if (Green<Red && Green<Blue && Green<= 8){
    Serial.println("Green");
    digitalWrite(LED_G, HIGH);
    servo1.write(10);
    delay(2000);
    servo3.write(170);
    delay(2000);
    servo1.write(90);
    delay(2000);
    servo2.write(29);
    delay(2000);
    servo1.write(20);
    delay(2000);
    servo3.write(0);
    delay (2000);
    servo1.write(90);
    delay(2000);
    servo2.write(90);
    delay(2000);
   }
     
   else{
    digitalWrite(LED_G, LOW);
   }
   delay(2000);
}

void GetColors()  
{    
  digitalWrite(s2, LOW);                                           
  digitalWrite(s3, LOW);                                           
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       
  digitalWrite(s3, HIGH);                                         
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
  digitalWrite(s2, HIGH);  
  Green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
}

