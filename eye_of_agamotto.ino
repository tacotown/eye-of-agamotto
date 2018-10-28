#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

/*
 * Eye of Agamotto
 * 
 * 3D printed model sources
 * 
 * original
 * link:
 * 
 * improved base
 * link:
 * 
 * motor adapter with eye construction instructions
 * link:
 * 
 * Hardware:
 * This code assumes an Arduino UNO with an
 * Adafruit Motor shield v2.3
 * link:
 * 
 * Attach an LED and BUTTON to the shiled and attach a motor.
 * Ports/pin inputs/outputs can be configured below.
 * 
 * 
 */

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// stepper motor.  Steps per resoltion, and shield motor port
Adafruit_StepperMotor *myMotor = AFMS.getStepper(400, 1);

/*
 * Modifiable constants
 */
const boolean STARTING_OPEN_STATE = false;
// debounce; button must stay closed for this many continuous reads
const int MAX_DEBOUNCE = 300;
// if button is held down, counter will reset at this value
const int MAX_STUCK = 30000;
// number of steps motor takes when moving in either direction
const int MAX_STEP = 30;
// eye opens this many milli-seconds after button press
// this is so you can pretend to do the eye opening spell
const int SPELL_OPEN_PAUSE = 3000;
// eye closes after this many milli-seconds after button press
// this is if you want to do an eye closing action
const int SPELL_CLOSE_PAUSE = 0;
// if the eye is having trouble staying closed, set to true
// note: this will eat up the batteries much faster
const boolean HOLD_CLOSED = true;
// if the eye is having trouble staying open, set to true
const boolean HOLD_OPEN = false;
// LED output pin
const int LED = 2;
// Button input pin (Normally open)
const int BUTTON = 12;

/*
 * state variables
 */
// state of eye of agamotto
boolean OPEN = STARTING_OPEN_STATE;
// duration button has been held down
int count = 0;

void setup() {
  
  pinMode(BUTTON,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  AFMS.begin();
  myMotor->setSpeed(5);
  myMotor->release();
  // reset
  if (STARTING_OPEN_STATE)
  {
    open();
  }
  else
  {
    close();
  }
}

void loop() {
  if (triggered())
  {
     if (OPEN)
     {
       delay(SPELL_CLOSE_PAUSE);
       close();
     }
     else
     {
       delay(SPELL_OPEN_PAUSE);
       open();
     }
  }
}

boolean triggered()
{
    return buttonPressed();
}

boolean buttonPressed()
{
   if (0 == digitalRead(BUTTON))
   {
      count++;
   }
   else
   {
      count = 0;
   }

    if (count > MAX_STUCK)
    {
       count = 0;
    }

   return (count > MAX_DEBOUNCE);
}

void open()
{
  digitalWrite(LED,HIGH);
  moveMotor(BACKWARD);
  OPEN=true;
  if (!HOLD_OPEN)
  {
    myMotor->release();
  }
}

void close()
{
  moveMotor(FORWARD);
  digitalWrite(LED,LOW);
  OPEN=false;
  if (!HOLD_CLOSED)
  {
    myMotor->release();
  }
}

void moveMotor(int direction)
{
  myMotor->step(MAX_STEP, direction, MICROSTEP); 
}
