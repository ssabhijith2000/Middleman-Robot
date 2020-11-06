#include <Servo.h>

//transmitter pins
#define front_back_stick A6 //ch1
#define left_right_stick A5 //ch2
#define front_shift_stick A4 //ch3
const int voltageSensor = A7;//voltage sensor

//motor driver 1
#define dirA1 43//23
#define pwA1 8 
#define dirB1 45//25
#define pwB1 9
#define brkA1 47//27
#define brkB1 49//29

//motor driver 2
#define dirA2 42 //22
#define pwA2 10
#define dirB2 44//24
#define pwB2 11
#define brkA2 46//26
#define brkB2 48//28

//charge pins
#define led1 50//30
#define led2 51//31
#define led3 52//32
#define led4 53//33
Servo myservo; 

//thresholds
int upper_stick_threshold=1800; //servo stick
int lower_stick_threshold=1300;
int upperf_stick_threshold=1600;//joystick
int lowerf_stick_threshold=1400;
int maxspeed=200; //change this to adjust maximum rpm 
int maxspeedlr=100;
//initialisations
int oldstate =0;
int newstate = 0; 
int oldstatelr=0;
int newstatelr=0;
int oldstatefb=0;
int newstatefb=0;
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

int lrmap;
int fbmap;
int pos=0;
void setup()
{
  Serial.begin(9600);
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
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(voltageSensor,INPUT);
for(int j=0; j<3; j++)
{ int i=50;
  for(i=50; i<=53; i++)
  {
    digitalWrite(i, HIGH);
    delay(150);
    digitalWrite(i, LOW);
  }
  digitalWrite(i, LOW);
  digitalWrite(i+1, LOW);
  digitalWrite(i+2, LOW);
  digitalWrite(i+3, LOW);
}
bat();
  Serial.begin(9600);
  myservo.attach(12); //servo pin
  myservo.write(0);
  pos=0;
  brake(20);
  
}

void loop()
{
   int front_back =pulseIn (front_back_stick, HIGH); 
   Serial.println(front_back); 
   
    //Serial.print("  front");
   
   if(front_back>upperf_stick_threshold)
  {
 fbmap=map(front_back,upperf_stick_threshold,2050,0,maxspeed);
 }
 if(front_back<lowerf_stick_threshold)
 {
   fbmap=map(front_back,lowerf_stick_threshold,920,0,maxspeed);
 }
 delay(10);// Checks the value of front_back
 Serial.print(front_back);
Serial.print("     "); 
 int left_right = (pulseIn (left_right_stick, HIGH));
 Serial.print(left_right);
  //Serial.print("  right");
 if(left_right>upperf_stick_threshold)
 {
  lrmap=map(left_right,upperf_stick_threshold+100,2050,0,maxspeedlr);
 }
 if(left_right<lowerf_stick_threshold)
 {
   lrmap=map(left_right,lowerf_stick_threshold-100,920,0,maxspeedlr);
 }
 
 //Serial.print(left_right);
 int front_shift = (pulseIn (front_shift_stick, HIGH));
  //Serial.print(front_shift);
  //Serial.print("  front_shift");
 //Serial.print("     "); 
// Serial.println(front_shift);
 
 
 //digital conversion of analog input of shift switch
 if (front_shift>upper_stick_threshold)
 {
     newstate=1;
 }
 if (front_shift<lower_stick_threshold)
 {
     newstate=0;
 }
 if (left_right>upperf_stick_threshold+100)
 {
   newstatelr=4;
 }
 if (left_right<lowerf_stick_threshold-100)
 {
   newstatelr=3;
 }
 if (left_right>lowerf_stick_threshold-100 && left_right<upperf_stick_threshold+100 )
 {
   newstatelr=0;
 }
 if (front_back<lowerf_stick_threshold)
 {
  newstatefb=0;
 }
 if (front_back>upperf_stick_threshold)
 {
  newstatefb=1;
 }
 Serial.println(newstatelr);
 //code switching based on shift switch input
 switch(newstate)
  {
    case 1:
            if(newstate!=oldstate)
            {   brake(20);
                Serial.println("Servo at 0 degree");
                  if(pos==180)
                  {
                  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(5);                       // waits 15ms for the servo to reach the position
            }//end for
                  }//end if
                  else
                  {
                    myservo.write(0);
                  } //end else
                  pos=0;
            }
            if (front_back==0 || left_right==0)
            {
              bat();
              brake(20);
            }            
            else if(newstatelr!=oldstatelr && newstatelr!=0)
            {
              brakehard(20);
              Serial.println("braking left");
              delay(50);
            }
            else if(newstatefb!=oldstatefb)
            {
              brake(20);
              Serial.println("braking left");
            }
            

            else if (front_back > upperf_stick_threshold && left_right < upperf_stick_threshold && left_right> lowerf_stick_threshold)
            {
            forward(fbmap);
            //Serial.println("forward");
            }
            else if (front_back < lowerf_stick_threshold && left_right < upperf_stick_threshold && left_right> lowerf_stick_threshold) 
            {
            backward(fbmap);
            //Serial.println("backward");
            }
            else if (left_right > upperf_stick_threshold+100)
            {
              
            right(lrmap);
            //Serial.println("right");
            }
            else if(left_right < lowerf_stick_threshold-100)
            {
            left(lrmap);
            //Serial.println("left");
            }
            else{
              brake(20);
              bat();
              }
    break;
    case 0:
            
            if (front_back==0 && left_right==0)
            {
              brake(20);
            }
            if(newstate!=oldstate)
            {
                brake(20);
                Serial.println("Servo at 180 degree");
                if(pos==0)
                  {
                  for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(5);                       // waits 15ms for the servo to reach the position
            }//end for
            pos=180;
                  }//end if
                  else
                  {
                    myservo.write(180);
                    pos=180;
                  }
                  
            }
            if (front_back==0 || left_right==0)
            {
              bat();
              brake(20);
            }  
            else if(newstatelr!=oldstatelr && newstatelr!=0)
            {
              brakehard(20);
              //Serial.println("braking left");
              delay(50);
            }
            else if(newstatefb!=oldstatefb)
            {
              brake(20);
              Serial.println("braking left");
            }
            else if (front_back > upperf_stick_threshold && left_right < upperf_stick_threshold && left_right> lowerf_stick_threshold)
            {
            backward(fbmap);
           // Serial.println("backward");
            }
            else if (front_back < lowerf_stick_threshold && left_right < upperf_stick_threshold && left_right> lowerf_stick_threshold) 
            {
            forward(fbmap);
           // Serial.println("forward");
            }
            else if (left_right > upperf_stick_threshold+100)
            {
            right(lrmap);
            //Serial.println("right");
            }
            else if(left_right < lowerf_stick_threshold-100)
            {
            left(lrmap);
            //Serial.println("left");
            }
             else{
              bat();
              brake(20);
              
              }
    break;
  }
oldstate=newstate;
oldstatelr=newstatelr;
oldstatefb=newstatefb;
}

void forward(int a)
{ 
 //Serial.print("forward  ");
 // Serial.println(a);
  digitalWrite(dirA1, LOW);
  analogWrite(pwA1, a);
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
{
  
  //Serial.print("backward  ");
  // Serial.println(a);
  digitalWrite(dirA1, HIGH);
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
{   
//Serial.print("left  ");
//  Serial.println(a);
  digitalWrite(dirA1, LOW);
  analogWrite(pwA1, a);
  digitalWrite(dirB1, LOW);
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

void right(int a)
{ 
 //Serial.print("right  ");
//  Serial.println(a);
  digitalWrite(dirA1, HIGH);
  analogWrite(pwA1, a);
  digitalWrite(dirB1, HIGH);
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

void brake(int a)
{
  //Serial.print("brake  ");
// Serial.println(a);
  digitalWrite(dirA1, LOW);
 analogWrite(pwA1, 0);
 digitalWrite(dirB1, LOW);
 analogWrite(pwB1, 0);
  digitalWrite(dirA2, LOW);
 analogWrite(pwA2, 0);
 digitalWrite(dirB2, LOW);
 analogWrite(pwB2, 0);
 delay(250);
 digitalWrite(brkA1, HIGH);
 digitalWrite(brkB1, HIGH);
  digitalWrite(brkA2, HIGH);
 digitalWrite(brkB2, HIGH);
}
void brakehard(int a)
{
  //Serial.print("brake  ");
// Serial.println(a);
  digitalWrite(dirA1, LOW);
 analogWrite(pwA1, 0);
 digitalWrite(dirB1, LOW);
 analogWrite(pwB1, 0);
  digitalWrite(dirA2, LOW);
 analogWrite(pwA2, 0);
 digitalWrite(dirB2, LOW);
 analogWrite(pwB2, 0);
 digitalWrite(brkA1, HIGH);
 digitalWrite(brkB1, HIGH);
  digitalWrite(brkA2, HIGH);
 digitalWrite(brkB2, HIGH);
}

void bat()
{
  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));
  delay(100);
  float  b = map(vIN, 8.5, 13.6, 0, 100); //vIN
   //Serial.println(b);
  if(b < 20)
  {
   digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW); 
  }
  if(b>20 && b<50)
  {
   digitalWrite(led1, HIGH);
   digitalWrite(led2, HIGH);
   digitalWrite(led3, LOW);
   digitalWrite(led4, LOW);
 
  }
  if(b>50 && b<80)
  {
   digitalWrite(led1, HIGH);
   digitalWrite(led2, HIGH);
   digitalWrite(led3, HIGH);
   digitalWrite(led4, LOW);
  }
  if(b>80 && b<=100)
  {
   digitalWrite(led1, HIGH);
   digitalWrite(led2, HIGH);
   digitalWrite(led3, HIGH);
   digitalWrite(led4, HIGH);
   }
}
