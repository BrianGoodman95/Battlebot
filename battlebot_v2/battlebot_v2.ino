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

// while (error != 0) {
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
//   delay(5000);
   Serial.println(error);
//  }
}

// Main loop
void loop(){
   
 if(error != 0) { //skip loop if no controller found
//  delay(1000);
  Serial.println(error);
  error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 }
//  return; 
  
 else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); // disable vibration of the controller

    int nJoyL = ps2x.Analog(PSS_LY); // read left stick
    int nJoyR = ps2x.Analog(PSS_RY); // read right stick
   
    nJoyL = map(nJoyL, 0, 255, 1023, -1023);
    nJoyR = map(nJoyR, 0, 255, -1023, 1023);

    // Perform movements based on both analog sticks
     if(nJoyR>50) {
        digitalWrite(MOTORA_1,LOW);
        digitalWrite(MOTORA_2,HIGH);
         analogWrite(ENA, 1023);
        Serial.println("Right stick is down");
     }
     if(nJoyR<-50) {
        digitalWrite(MOTORA_1,HIGH);
        digitalWrite(MOTORA_2,LOW);
        analogWrite(ENA, 1023);
        Serial.println("Right stick is up");
     }
     if (abs(nJoyR)<50) {
       analogWrite(ENA, 0);
       Serial.println("Right stick is nothing");
     }
     
     if(nJoyL>50) {
       digitalWrite(MOTORB_1,HIGH);
       digitalWrite(MOTORB_2,LOW);
       analogWrite(ENB, 1023);
        Serial.println("Left stick is up");
     }
     if(nJoyL<-50) {
       digitalWrite(MOTORB_1,LOW);
       digitalWrite(MOTORB_2,HIGH);
       analogWrite(ENB, 1023);
        Serial.println("Left stick is down");
     }
     if (abs(nJoyL)<50) {
       analogWrite(ENB, 0);
       Serial.println("Left stick is nothing");
     }     


     if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
       Serial.println("Start is being held");
     if(ps2x.Button(PSB_SELECT))
       Serial.println("Select is being held");
       
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
       digitalWrite(MOTORB_1,HIGH);
       digitalWrite(MOTORB_2,LOW);
       analogWrite(ENB, 1023);
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
       analogWrite(ENA, 0);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
       analogWrite(ENB, 0);
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.print("DOWN held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
       digitalWrite(MOTORA_1,HIGH);
       digitalWrite(MOTORA_2,LOW);
       analogWrite(ENA, 1023);
      }   
  
    
//      vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
                                              //how hard you press the blue (X) button    
    
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {   
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");
         
    }   
         
    
    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");
         
    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just released");     
    
    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");    
    
    
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC); 
    } 

 }   
 delay(50);
 
}
