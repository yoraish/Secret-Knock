/*  
 *   
 *  Knocking Sequence Enabled Action arduino program 
 *  
 *  
 *  Created by Yorai Shaoul. The program is open source and can be used for 
 *  personal applications. The use of this code for profit is prohibited 
 *  unless otherwise specified by the creator.
 *  I can be reached at yorai[at]mit[dot]edu
 *  
 *  The physical setup required for this program to function is simple.
 *  It consists of a DIGITAL microphone or button, and some components 
 *  that will do something once the correct rhythm of knocks is inputted.
 *  
 *  For this example, I used a button/microphone, a servo at pin 8, and 
 *  several LEDs connected to the analog pins.
 *  
 *  
 *  Keep on making! 
 *  Yorai :)


*/



#include <EEPROM.h>
#include<Servo.h> // servo library

Servo lockServo ; // define the servo that will control the lock 
int lockPin = 8; // the pin connected to the lock servo

int buttonPin = A0; /* the sensor pin - microphone can return 
                       digital signal when it hears a sound stronger than a set volume
                       and therefore be treated as a button */
                       
int buttonState = HIGH; // 0 is  press, 1 is no press

float firstPress;  // press times 
float secondPress;
float thirdPress;
float fourthPress;

float firstBreak;  //break times
float secondBreak;
float thirdBreak;

float firstBreakVar; // breaks as variables
float secondBreakVar;
float thirdBreakVar;

float firstBreakCode ;  // the breaks (as vars) of the code
float secondBreakCode ;
float thirdBreakCode ;

float threshold = 0.3;

int  combinationApproval ;

int x =0;

boolean wasTheDoorOpenJustNow = false;

void setup(){
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  

    
    delay(100);
  
  calibration();
  
  delay(500);
  
  
 float a = EEPROM.read(100);
 float b = EEPROM.read(101);
 float c = EEPROM.read(102);
 
 
   firstBreakCode = a/100 ;
   secondBreakCode = b/100 ;
   thirdBreakCode = c/100;
   
   delay(300);
    digitalWrite(A2,HIGH);
    digitalWrite(A1,HIGH);
    delay(1000);
    digitalWrite(A2,LOW);
    digitalWrite(A1,LOW);
}


void loop(){
 
  
 Serial.println("");
 Serial.println("Welcome to secrete knock. Please enter code now");
 Serial.println("");

  readPress(); // we are presented with the three breaks vars
  
Serial.print("    --   ");
Serial.print(firstBreakVar);
Serial.print("***");
Serial.print(secondBreakVar);
Serial.print("***");
Serial.print(thirdBreakVar);
Serial.print("    --   ");



if( checkCombination() == 1){
Serial.print("Access Granted"); // if the sequence is correct, take action 
   delay(100);
   
       // unlock door - you need to lock it manually, or make a wrong combination
    
     lockServo.attach(lockPin);
     delay(100);
      lockServo.write(80); // servo on unlocked position
     delay(300);
      lockServo.detach();


grantedLedSequence(); //flash lights


    
    wasTheDoorOpenJustNow = true; // take note that the door is unlocked

}
else if (checkCombination() == 0){
 delay(500);
Serial.print("Access Denied");// if the sequence is incorrect, take action

// lock door only if it was open 
 
 if(wasTheDoorOpenJustNow == true){  
    
     lockServo.attach(lockPin);
     delay(100);
      lockServo.write(0); //servo locked position
     delay(300);
      lockServo.detach();
 }
      // flash lights
      
   deniedLedSequence(); 
    
    wasTheDoorOpenJustNow = false; //take note that the door is closed
     
}


}



void readPress(){


 while( buttonState != LOW){ 
   buttonState = digitalRead(buttonPin);
} //wait for press1

 firstPress = millis(); //time of first press
 buttonState = HIGH;

   digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    
delay(100);


//--------------------

while( buttonState != LOW){

  buttonState = digitalRead(buttonPin);
  
if(millis() - firstPress > 1000){
secondPress = millis();
buttonState = LOW;

}

} //wait for press2
 
secondPress = millis(); //time of first press
  buttonState = HIGH;

  digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);

delay(100);


//--------------------

while( buttonState != LOW){
  
  buttonState = digitalRead(buttonPin);

if(millis() - firstPress > 2000){
thirdPress = millis();
buttonState = LOW;

}

} //wait for press3
 
 thirdPress = millis(); //time of first press
  buttonState = HIGH;

 digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    
delay(100);


//--------------------

while( buttonState != LOW){
  
  buttonState = digitalRead(buttonPin);

if(millis() - firstPress > 2500){
fourthPress = millis();
buttonState = LOW;
}

} //wait for press4
 
 fourthPress = millis(); //time of first press
 buttonState = HIGH;

 digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    
delay(100);


//--------------------


firstBreak = secondPress - firstPress;
secondBreak = thirdPress - secondPress;
thirdBreak = fourthPress - thirdPress;

firstBreakVar = 1;
secondBreakVar = secondBreak / firstBreak;
thirdBreakVar = thirdBreak / secondBreak;

//------------------

  /* delay(200); //wait  and repeat the sequence
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    delay(firstBreakVar*200);
    
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    delay(secondBreakVar*200);
    
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    delay(thirdBreakVar*200);
    
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
*/
}

int checkCombination(){

  if(abs(secondBreakCode - secondBreakVar) < threshold){
  if(abs(thirdBreakCode - thirdBreakVar) < threshold){
    
 combinationApproval = 1;

  }}
else{
 combinationApproval = 0;

}

return combinationApproval;
}


void calibration(){ 
  /* calibrates the breaks for the code sequence, only if their spot in the eeprom is 0.
     reads the presses normally, and stores the breaks to firstBreakCode etc.
  
*/
if(EEPROM.read(100) == 0){
    if(EEPROM.read(101)==0){
      if(EEPROM.read(102)==0){      

 Serial.println("Welcome to calibration, please enter your sequence now.   ");

        
        
 readPress();
 
 delay(200); //wait  and repeat the sequence
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    delay(firstBreakVar*200);
    
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    delay(secondBreakVar*200);
    
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);
    delay(thirdBreakVar*200);
    
    digitalWrite(A2,HIGH);
    delay(50);
    digitalWrite(A2,LOW);


 firstBreakCode = firstBreakVar;  // the breaks (as vars) of the code
 secondBreakCode = secondBreakVar;
 thirdBreakCode = thirdBreakVar;
  


    EEPROM.write(100,firstBreakCode*100);
        EEPROM.write(101,secondBreakCode*100);
            EEPROM.write(102,thirdBreakCode*100);
            
 
Serial.println("Thank you. Your Breaks are:");
Serial.print("First:"); Serial.print(firstBreakCode);
Serial.println("");
Serial.print("Second:"); Serial.print(secondBreakCode);
Serial.println("");
Serial.print("Third:"); Serial.print(thirdBreakCode);
Serial.println("");
      }}}



}



void grantedLedSequence(){

for(int ledCount = 14; ledCount < 19 ; ledCount ++){

  digitalWrite(ledCount, HIGH);
  delay(30);
  digitalWrite(ledCount, LOW);
  delay(30);

}
for(int ledCount = 19; ledCount > 13 ; ledCount --){

  digitalWrite(ledCount, HIGH);
  delay(30);
  digitalWrite(ledCount, LOW);
  delay(30);


}
for(int ledCount = 15; ledCount < 19 ; ledCount ++){

  digitalWrite(ledCount, HIGH);
  delay(30);
  digitalWrite(ledCount, LOW);
  delay(30);

}
for(int ledCount = 19; ledCount > 13 ; ledCount --){

  digitalWrite(ledCount, HIGH);
  delay(30);
  digitalWrite(ledCount, LOW);
  delay(30);


}
}

void deniedLedSequence(){


for(int ledCount = 14; ledCount < 19 ; ledCount ++){

  digitalWrite(ledCount, HIGH);
  


}
delay(500);
for(int ledCount = 19; ledCount > 13 ; ledCount --){

  digitalWrite(ledCount, LOW);
  


}
delay(500);

for(int ledCount = 14; ledCount < 19 ; ledCount ++){

  digitalWrite(ledCount, HIGH);
  


}
delay(500);
for(int ledCount = 19; ledCount > 13 ; ledCount --){

  digitalWrite(ledCount, LOW);
  


}

}
