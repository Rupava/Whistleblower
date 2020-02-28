#include <SoftwareSerial.h>

SoftwareSerial mySerial(A1, A2);

#define led 13
#define redSig 9
#define greenSig 10
#define blueSig 11

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;

byte prevKeyState = HIGH;         // button is active low
const byte keyPin = 8;     

int wC[3]={0,255,0};
int iC[3]={255,10,10};
int mC[3]={0,100,255};
int State=1;
char command;
String string, recentNote="", secondNote="", thirdNote="",fourthNote="", masterState="OFF";
boolean ledon = false;



void setup(){
  Serial.begin(57600);
  mySerial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(redSig, OUTPUT);
  pinMode(blueSig, OUTPUT);
  pinMode(greenSig, OUTPUT);
  pinMode(keyPin, INPUT_PULLUP);
}

void loop(){
  /////////////////////////////////////BTN_OUTPUT////////////////////
  // key management section
    if (millis() - keyPrevMillis >= keySampleIntervalMs) {
        keyPrevMillis = millis();
        byte currKeyState = digitalRead(keyPin);
        if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
            keyPress();}
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();}
        else if (currKeyState == LOW) {
            longKeyPressCount++;}       
        prevKeyState = currKeyState;
    }
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////SERIAL_READ///////////////////
  if (mySerial.available() > 0) 
    {string = "";}
    while(mySerial.available() > 0){  
      command = ((byte)mySerial.read());
      if(command == ':'){break;}
      else{string += command;}
      delay(1);
      State=1;}
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////SET_MASTER_STATE////////////// 
   if(string =="&TO"){masterState="ON";}

   if(string =="&TF"){masterState="OFF";}

   if(string =="&CLR"){
    recentNote="", secondNote="", thirdNote="", string="";
    analogWrite(redSig,150);
    analogWrite(greenSig,255);
    analogWrite(blueSig,255);}
  ///////////////////////////////////////////////////////////////////  
    
   if(masterState == "ON"){
    if(string == "@com.instagram.android"){updateRecentNote();}
    if(string == "@com.whatsapp"){updateRecentNote();}
    if(string == "@com.facebook.orca"){updateRecentNote();}

    if(recentNote != ""){

     if(State==1){
        recentNoteFunc();
      }
     if(State==2){
        secondNoteFunc();
      }
     if(State==3){
        thirdNoteFunc();
      }
     
     if(State==4){
        fourthNoteFunc();
      }    
    
    }

    
    
   }
   if(masterState == "OFF"){
    analogWrite(redSig,150);
    analogWrite(greenSig,255);
    analogWrite(blueSig,255);
    } 
   
 }

 void updateRecentNote(){
  if(recentNote != string){
    fourthNote =  thirdNote;
    thirdNote = secondNote;
    secondNote = recentNote;
    recentNote = string;
    }
  }

 void setRGB(int R,int G, int B){
    analogWrite(redSig,R);
    analogWrite(greenSig,G);
    analogWrite(blueSig,B);
    } 
 void recentNoteFunc(){
  if(recentNote == "@com.instagram.android"){    
        setRGB(iC[0],iC[1],iC[2]);
      }
      if(recentNote == "@com.whatsapp"){
        setRGB(wC[0],wC[1],wC[2]);
      }
      if(recentNote == "@com.facebook.orca"){
        setRGB(mC[0],mC[1],mC[2]);
      }
  }

  void secondNoteFunc(){
    if(secondNote==""){State=1;}
  if(secondNote == "@com.instagram.android"){    
        setRGB(iC[0],iC[1],iC[2]);
      }
      if(secondNote == "@com.whatsapp"){
        setRGB(wC[0],wC[1],wC[2]);
      }
      if(secondNote == "@com.facebook.orca"){
        setRGB(mC[0],mC[1],mC[2]);
      }
  }

  void thirdNoteFunc(){
    if(thirdNote==""){State=1;}
  if(thirdNote == "@com.instagram.android"){    
        setRGB(iC[0],iC[1],iC[2]);
      }
      if(thirdNote == "@com.whatsapp"){
        setRGB(wC[0],wC[1],wC[2]);
      }
      if(thirdNote == "@com.facebook.orca"){
        setRGB(mC[0],mC[1],mC[2]);
      }
  }   
      
   void fourthNoteFunc(){
    if(fourthNote==""){State=1;}
  if(thirdNote == "@com.instagram.android"){    
        setRGB(iC[0],iC[1],iC[2]);
      }
      if(fourthNote == "@com.whatsapp"){
        setRGB(wC[0],wC[1],wC[2]);
      }
      if(fourthNote == "@com.facebook.orca"){
        setRGB(mC[0],mC[1],mC[2]);
      }   
  }


  void shortKeyPress() {
    State++;
    if(State>4){State=1;}
    Serial.println(State);
    Serial.println("RN:"+recentNote);
    Serial.println("SN:"+secondNote);
    Serial.println("TN:"+thirdNote);
}

void longKeyPress() {
  recentNote="", secondNote="", thirdNote="", string="";
    analogWrite(redSig,150);
    analogWrite(greenSig,255);
    analogWrite(blueSig,255);
    Serial.println("long");
}

void keyPress() {
    Serial.println("key press");
    longKeyPressCount = 0;
}

void keyRelease() {
    Serial.println("key release");
   
    if (longKeyPressCount >= longKeyPressCountMax) {
        longKeyPress();
    }
    else {
        shortKeyPress();
    }
}



 

    
