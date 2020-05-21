/* Code for Control System plus User Interface of Easel
 * Team 5
 * Kenneth Mohs, Julia Zimmer, Austin Wetzel
 * 
 * The program below is meant to cycle through various 
 * available movement options as shown on an LCD 
 * screen which the user can then choose through
 * a button. While the button is pressed, the
 * desired movement direction is performed by
 * the appropriate motor, at the given motor speed.
*/

//Libraries
#include <LiquidCrystal.h>        //The liquid crystal library contains
                                  //  commands for printing to the [lcd] display
 
//Timing
int carryOver;                   //Keeps track of the total time elapsed since 
                                 //  the last movement choice change
int TpC = 2;                     //(Time per Choice) How long each choice is 
                                 //  available in seconds
int pause;                       //Important for timing when the button is pushed

//Motor Speed
int moveSpeed = 255;              //Movement speed of the motor. Can be any value
                                  //  from 0 to 255. Needs to be set here in advance

//Horizontal Motion Motor
const int AIN1 = 13;             //Red wire of motor
const int AIN2 = 12;             //Black wire of motor
const int PWMA = 11;             //Controls voltage supplied to motor

//Vertical Motion Motor
const int PWMB = 10;             //Black wire of motor
const int BIN2 = 9;              //Red wire of motor
const int BIN1 = 8;              //Controls voltage supplied to motor

//Movement options and related
int m = 4;                       //Number of movement choices
String moveOp[4] = {"UP", "DOWN", "LEFT", "RIGHT"};   //Movement choices

  //Corresponding screen starting location coordinates:
  //  UP = (7,0)
  //  DOWN = (6,1)
  //  LEFT = (0,0)
  //  RIGHT = (11,0)
int moveOpStLoc[8] = {7,0 ,
                     6,1 ,
                     0,0 ,
                     11,0}; //(Screen starting location Coordinates)

LiquidCrystal lcd(5, 4, 3, 2, 1, 0);   //Tell the RedBoard what pins are 
                                       //  connected to the display

void setup() {

  pinMode(6, INPUT_PULLUP);         //Tells the Arduino that this pin will read
                                    //  either on or off (based on button state)

  carryOver = 0;                    //Program starts at time zero
  pause = 0;                        //Program starts without any pause
  
  //LCD Setup
  lcd.begin(16, 2);                 //Tell the lcd library that we are using a 
                                    //  display that is 16 characters wide and 2
                                    //  characters high

  //Serial Setup
  //Serial.begin(9600);             //WARNING: SERIAL MONITOR AND LCD CANNOT
                                    //  FUNCTION SIMULTANEOUSLY - Serial
                                    //  is only used for testing purposes
                                    
  lcd.clear();                      //Clear the lcd display

  //Motor Setup
  
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}

void loop() {

  for (int i = 0; i < m; i++) {     //Loop through the movement options

    for (; ((millis()/1000) - carryOver) < (TpC + pause);) {   //
      /* The amount of time a movement option is available for defined by 
       *  time(TpC) and as influenced by the button. The button causes 
       *  the options to wait until the desired amount of movement is 
       *  achieved before resuming where it left off. */

      lcd.setCursor((int)moveOpStLoc[(2*i)], (int)moveOpStLoc[(2*i+1)]);        
      //  Set the cursor to the position first character location for the
      //      movement option [on the LCD screen]
      lcd.print(moveOp[i]);  //The option currently available is displayed in the LCD

      while (digitalRead(6) == LOW) {       //If the button is pressed, perform 
                                            //  the movement option
        if (moveOp[i] == "UP") {
          verticalMotor(moveSpeed);
        }
        else if (moveOp[i] == "DOWN"){
          verticalMotor(-moveSpeed);
        }
        else if (moveOp[i] == "LEFT") {
          horizontalMotor(-moveSpeed);
        }
        else if (moveOp[i] == "RIGHT") {
          horizontalMotor(moveSpeed);
        }
        else{
          lcd.setCursor(0,0);
          lcd.print("ERROR: MOTION OP");
          lcd.setCursor(0,1);
          lcd.print("UNSPECIFIED");
        }
                
        pause = (millis()/1000 - carryOver);  //Pauses the counter at 
                                              //  the choice being used
      }

      if (digitalRead(6) != LOW) {
        verticalMotor(0);
        horizontalMotor(0);
      }

      //TESTING
      //lcd.setCursor(0,1);
      //lcd.print(millis()/1000);

      
    }
    carryOver = millis()/1000;          //Reset the time since the last
                                        //  movement option, which will 
                                        //  change after this
    pause = 0;                      //Reset any pause time caused by the
                                    //  pressing of the button for the 
                                    //  next option
    lcd.clear();                    //Remove any text from the lcd
  }
}


//MOTOR FUNCTIONS

/************************************************/
void verticalMotor(int motorSpeed) {  //Motor responsible for y-axis/vertical motion
  if (motorSpeed > 0){        //positive y-direction
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }
  else if (motorSpeed <0){    //negative y-direction
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  else {                      //no motion (To try to resist movement and lock the
                              //  position of the canvas, a high voltage is induced across the motor)
   digitalWrite(AIN1, HIGH); 
   digitalWrite(AIN2, HIGH);
  }
  analogWrite(PWMA, abs(motorSpeed));
}

/************************************************/
void horizontalMotor(int motorSpeed) {  //Motor responsible for x-axis/horizontal motion
  if (motorSpeed > 0){        //positive x-direction
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  }
  else if (motorSpeed < 0){   //negative x-direction
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  else {                      //No motion (To try to resist movement and lock the 
                              //  position of the canvas, a high voltage is induced across the motor)
    digitalWrite(BIN1, HIGH); 
    digitalWrite(BIN2, HIGH);
  }
  analogWrite(PWMB, abs(motorSpeed));
}
