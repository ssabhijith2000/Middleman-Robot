
#include <Servo.h>

//transmitter pins
#define front_back_stick A9
#define left_right_stick A10
#define front_shift_stick A11

//motor driver 1
#define dirA1 23
#define pwA1 25
#define dirB1 26
#define pwB1 28
#define brkA1 29
#define brkB1 29

//motor driver 2
#define dirA2 30
#define pwA2 31
#define dirB2 21
#define pwB2 33
#define brkA2 34
#define brkB2 29
Servo myservo; 

//thresholds
int upper_stick_threshold=1500;
int lower_stick_threshold=1300;
int maxspeed=100;

//initialisations
int oldstate =0;
int newstate = 0; 

void setup()
{
  pinMode(dirA1, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(dirB2, OUTPUT);
  pinMode(pwA1, OUTPUT);
  pinMode(pwB1, OUTPUT);
  pinMode(pwA2, OUTPUT);
  pinMode(pwB2, OUTPUT);
  pinMode(brkA1, OUTPUT);
  pinMode(brkB1, OUTPUT);
  pinMode(brkA2, OUTPUT);
  pinMode(brkB2, OUTPUT);
  Serial.begin(9600);
  myservo.attach(6);
}

void loop()
{
 int front_back = (pulseIn (front_back_stick, HIGH));  // Checks the value of front_back
 int left_right = (pulseIn (left_right_stick, HIGH));
 int front_shift = (pulseIn (front_shift_stick, HIGH));
 
 //digital conversion of analog input of shift switch
 if (front_shift>upper_stick_threshold)
 {
     newstate=1;
 }
 if (front_shift<lower_stick_threshold)
 {
     newstate=0;
 }
 
 //code switching based on shift switch input
 switch(newstate)
  {
    case 1:
            if(newstate!=oldstate)
            {
                myservo.write(0);
                delay(2000);
            }
            if (front_back > upper_stick_threshold)
            {
            forward(max_speed);
            Serial.println("forward");
            }
            if (front_back < lower_stick_threshold) 
            {
            backward(maxspeed);
            Serial.println("backward");
            }
            if (left_right > upper_stick_threshold)
            {
            left(maxspeed);
            Serial.println("left");
            }
            if(left_right < lower_stick_threshold)
            {
            right(maxspeed);
            Serial.println("right");
            }
    break;
    case 0:
            if(newstate!=oldstate)
            {
                myservo.write(180);
                delay(2000);
            }
            if (front_back > upper_stick_threshold)
            {
            backward(max_speed);
            Serial.println("forward");
            }
            if (front_back < lower_stick_threshold) 
            {
            forward(maxspeed);
            Serial.println("backward");
            }
            if (left_right > upper_stick_threshold)
            {
            right(maxspeed);
            Serial.println("left");
            }
            if(left_right < lower_stick_threshold)
            {
            left(maxspeed);
            Serial.println("right");
            }
    break;
  }
oldstate=slidestate;
}

void forward(int a)
{ digitalWrite(dirA1, LOW);
  analogWrite(pwA, a);
  digitalWrite(dirB1, LOW);
  analogWrite(pwB1, a);
  digitalWrite(brkA1, LOW);
  digitalWrite(brkB1, LOW);
  digitalWrite(dirA2, LOW);
  analogWrite(pwA2, a);
  digitalWrite(dirB2, LOW);
  analogWrite(pwB2, a);
  digitalWrite(brkA2, LOW);
  digitalWrite(brkB2, LOW);
}

void backward(int a)
{ digitalWrite(dirA1, HIGH);
  analogWrite(pwA1, a);
  digitalWrite(dirB1, HIGH);
  analogWrite(pwB1, a);
  digitalWrite(brkA1, LOW);
  digitalWrite(brkB1, LOW);
  digitalWrite(dirA2, HIGH);
  analogWrite(pwA2, a);
  digitalWrite(dirB2, HIGH);
  analogWrite(pwB2, a);
  digitalWrite(brkA2, LOW);
  digitalWrite(brkB2, LOW);
  
  }

void left(int a)
{ digitalWrite(dirA1, HIGH);
  analogWrite(pwA1, a);
  digitalWrite(dirB1, LOW);
  analogWrite(pwB1, a);
  digitalWrite(brkA1, LOW);
  digitalWrite(brkB1, LOW);
  digitalWrite(dirA2, HIGH);
  analogWrite(pwA2, a);
  digitalWrite(dirB2, LOW);
  analogWrite(pwB2, a);
  digitalWrite(brkA2, LOW);
  digitalWrite(brkB2, LOW);
}

void right(int a)
{ digitalWrite(dirA1, LOW);
  analogWrite(pwA1, a);
  digitalWrite(dirB1, HIGH);
  analogWrite(pwB1, a);
  digitalWrite(brkA1, LOW);
  digitalWrite(brkB1, LOW);
  digitalWrite(dirA2, LOW);
  analogWrite(pwA2, a);
  digitalWrite(dirB2, HIGH);
  analogWrite(pwB2, a);
  digitalWrite(brkA2, LOW);
  digitalWrite(brkB2, LOW);
}

void brake(int a)
{digitalWrite(dirA1, LOW);
 analogWrite(pwA1, 0);
 digitalWrite(dirB1, LOW);
 analogWrite(pwB1, 0);
 digitalWrite(brkA1, HIGH);
 digitalWrite(brkB1, HIGH);
 digitalWrite(dirA2, LOW);
 analogWrite(pwA2, 0);
 digitalWrite(dirB2, LOW);
 analogWrite(pwB2, 0);
 digitalWrite(brkA2, HIGH);
 digitalWrite(brkB2, HIGH);
}
