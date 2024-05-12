#include <ESP32Servo.h>

int servoPosDown = 100;    // servo position when is up
int servoPosUp = 190;    // servo position when down
int servoPos = servoPosDown; 
int servoLowestNudgePosition = 150;

Servo myservo;

void moveUp(){
  if (servoPos != servoPosUp){
    Serial.println("move up");
    for (servoPos = servoPosDown; servoPos <= servoPosUp; servoPos += 1) { // goes from 0 degrees to 180 degrees
      myservo.write(servoPos);    // tell servo to go to position in variable 'pos'
      delay(30);             // waits 15ms for the servo to reach the position
    }  
    delay(1000);
  }
  else{
    Serial.println("Tried to move up mirror, but mirror is already up");        
  }
  servoPos = servoPosUp;
  mirrorState = 1;
}


void moveDown(){
  if (servoPos != servoPosDown){
    Serial.println("move down");    
    for (servoPos = servoPosUp; servoPos >= servoPosDown; servoPos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(servoPos);    // tell servo to go to position in variable 'pos'
      delay(30);             // waits 15ms for the servo to reach the position
    }  
    delay(1000);  
  }
  else{
    Serial.println("Tried to move down mirror, but mirror is already down");      
  }
  servoPos = servoPosDown;
  mirrorState = 0;
}

void nudgeMovement(){
  for (servoPos = servoPosUp; servoPos >= servoLowestNudgePosition; servoPos -= 3) { // goes from 180 degrees to 0 degrees
    myservo.write(servoPos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }  
  for (servoPos = servoLowestNudgePosition; servoPos <= servoPosUp; servoPos += 3) { // goes from 180 degrees to 0 degrees
    myservo.write(servoPos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  } 
  servoPos = servoPosUp;
}

void nudgeAction(int numberOfNudges){
  if (servoPos == servoPosUp) {
    Serial.println("starting to nudge");
    for(int i=0; i<numberOfNudges; i++){
        nudgeMovement();
        delay(70);
    }
    Serial.println("Finished nudging");
  }
}
