#include <Arduino.h>
#include <Romi32U4.h>
#include "Chassis.h"
#include <IRdecoder.h>
#include "RemoteConstants.h"
#include <Timer.h>
#include "BlueMotor.h"
#include "Gripper.h"
#include <Romi32U4Buttons.h>

Chassis chassis;
IRDecoder decoder(14);

Gripper gripper;
BlueMotor motor;

Romi32U4ButtonA pa;
Romi32U4ButtonB pb;
Romi32U4ButtonC pc;
bool lastState = false;
bool newState;

bool paused = false;
const long msPerDegree = 1000 / 90;
const long msPerInch = 1000 / 12;
const float LED_PIN = 13;


Timer driveTimer(msPerInch * 12);

enum States{
  Start,
  Driving,
  Turning,
  Paused
};

States state = Start;

void doDriving(){
  switch (state)
  {
  case Start:
    chassis.driveDistance(12);
    state = Driving;
    break;
  
  case Driving:
    chassis.driveDistance(12);
    if(chassis.doneDriving()){
      chassis.encoderReset();
      state = Turning;
    }
    else chassis.driveDistance(12);
    break;

  case Turning:
    chassis.turnAngle(90);
    if(chassis.doneDriving()){
      chassis.encoderReset();
      state = Start;
    }
    else chassis.turnAngle(90);
    break;
  
  case Paused:
    chassis.pauseDriving();
    break;
  }
} 


Timer blinkingTimer(250);
char lightOn = 0;

void doLED(){
  if(blinkingTimer.isExpired()){
    lightOn = !lightOn;
    digitalWrite(LED_PIN, lightOn);
  }
}

void checkRemote(){
  int16_t code = decoder.getKeyCode();
  States previousState = Start;
  switch (code){
    case remoteVolMinus:
      paused = true;
      previousState = state;
      state = Paused;
      break;

    case remoteVolPlus:
      paused = false;
      state = previousState;
      break;
  }
}


void setup() 
{
  Serial.begin(9600);
  decoder.init();
  motor.setup();
  chassis.encoderReset();
  gripper.init();
}
  

void loop() 
{
  newState = pc.isPressed();
  if(newState && !lastState){
    gripper.toggle();
  }
  lastState = newState;

  if(pb.isPressed()){
    unsigned long endTime = millis() + 100;
    motor.setEffort(300);
    while(millis() < endTime);
  }

  if(pa.isPressed()){
    unsigned long endTime = millis() + 100;
    motor.setEffort(-300);
    while(millis() < endTime);
  }
  else{
    motor.setEffort(0);
    motor.reset();
  }
}