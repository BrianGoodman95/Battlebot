// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#include <PS2X_lib.h>

// These are used to set the direction of the bridge driver.
#define ENA 3      //ENA
#define MOTORA_1 4 //IN3
#define MOTORA_2 5 //IN4
#define MOTORB_1 8 //IN1
#define MOTORB_2 7 //IN2
#define ENB 6      //ENB

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;

void setup(){

 // Configure output pins
 pinMode(ENA, OUTPUT);
 pinMode(MOTORA_1, OUTPUT);
 pinMode(MOTORA_2, OUTPUT);
 pinMode(ENB, OUTPUT);
 pinMode(MOTORB_1, OUTPUT);
 pinMode(MOTORB_2, OUTPUT);

 // Disable both motors
 digitalWrite(ENA,0);
 digitalWrite(ENB,0);

 // Start serial communication
 Serial.begin(57600);
  
 error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

 // Check for error
 if(error == 0){
  Serial.println("Found Controller, configured successful");
 }
   
 else if(error == 1)
  Serial.println("No controller found, check wiring or reset the Arduino");
   
 else if(error == 2)
  Serial.println("Controller found but not accepting commands");
  
 else if(error == 3)
  Serial.println("Controller refusing to enter Pressures mode, may not support it.");
   
 // Check for the type of controller
 type = ps2x.readType();
 switch(type) {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    break;
  }

digitalWrite(MOTORA_1,LOW);
digitalWrite(MOTORA_2,HIGH);
digitalWrite(MOTORB_1,HIGH);
digitalWrite(MOTORB_2,LOW);
analogWrite(ENB, 1023);
analogWrite(ENA, 1023);
Serial.println("Move forward");
}

// Main loop
void loop(){
   
 if(error == 1) //skip loop if no controller found
  return; 
  
 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate); // disable vibration of the controller
    Serial.print(error);
     // Perform movements based on D-pad buttons
     
     // MOVE FORWARD
     if(ps2x.Button(PSB_PAD_UP)) {
        digitalWrite(MOTORA_1,LOW);
        digitalWrite(MOTORA_2,HIGH);
        digitalWrite(MOTORB_1,HIGH);
        digitalWrite(MOTORB_2,LOW);
        analogWrite(ENB, 1023);
        analogWrite(ENA, 1023);
        Serial.println("Move forward");
      }
      // TURN RIGHT
      if(ps2x.Button(PSB_PAD_RIGHT)){
        digitalWrite(MOTORA_1,HIGH);
        digitalWrite(MOTORA_2,LOW);
        digitalWrite(MOTORB_1,HIGH);
        digitalWrite(MOTORB_2,LOW);
        analogWrite(ENB, 1023);
        analogWrite(ENA, 1023);
        Serial.println("Turn right");
      }
      // TURN LEFT
      if(ps2x.Button(PSB_PAD_LEFT)){
        digitalWrite(MOTORA_1,LOW);
        digitalWrite(MOTORA_2,HIGH);
        digitalWrite(MOTORB_1,LOW);
        digitalWrite(MOTORB_2,HIGH);
        analogWrite(ENB, 1023);
        analogWrite(ENA, 1023);
        Serial.println("Turn left");
      }
      // MOVE BACK
      if(ps2x.Button(PSB_PAD_DOWN)){
        digitalWrite(MOTORA_1,HIGH);
        digitalWrite(MOTORA_2,LOW);
        digitalWrite(MOTORB_1,LOW);
        digitalWrite(MOTORB_2,HIGH);
        analogWrite(ENB, 1023);
        analogWrite(ENA, 1023);         
        Serial.println("Move back");
      }  
      if (!ps2x.Button(PSB_PAD_DOWN) && !ps2x.Button(PSB_PAD_UP) && !ps2x.Button(PSB_PAD_RIGHT) && !ps2x.Button(PSB_PAD_LEFT)) {
        analogWrite(ENB, 0);
        analogWrite(ENA, 0);
      }
          
 delay(50);
 }    
}
