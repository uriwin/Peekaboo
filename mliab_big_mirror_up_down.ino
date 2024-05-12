#include "ConnectToWifi.h"

#define BLYNK_TEMPLATE_ID "TMPL6bv7KTsWe"
#define BLYNK_TEMPLATE_NAME "mirror  milba"
#define BLYNK_AUTH_TOKEN "fsEd6GNF6MezMY9KkVmsobasAHAM5r-n"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>


int isButtonpPressed = 0;  // variable for reading the pushbutton status
int mirrorState = 0; // mirror is down
int nudgeStatus = 0;
int nudgetsCount = 2; 

#include "ServoPosition.h"

const int buttonPin = 16;  // the number of the pushbutton pin
int servoPin = 13;

void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500); // attaches the servo onto pin
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  // initialize the serial port
  Serial.begin(115200);
  
  connectToWifi();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void switchMirrorState(){
  Serial.println("switch mirror position");
  if (mirrorState == 0){
    Serial.println("trying moving mirror down");
    moveDown();
   }
   else {
    Serial.println("trying moving mirror up");    
    moveUp();
   }
}

void loop() {
  Blynk.run();
  
  isButtonpPressed = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (isButtonpPressed == HIGH) {
    Serial.println("button was pressed");
    initializeMirrorPos();
  } 
}

BLYNK_WRITE(V0) {
  mirrorState = param.asInt();
  switchMirrorState();
}

BLYNK_WRITE(V1) {
  nudgeStatus = param.asInt();
  Serial.println("Got siganl to nudge");
  if (nudgeStatus == 1){
    nudgeAction(nudgetsCount);
    Blynk.virtualWrite(V1, 0);
  }
}

void initializeMirrorPos(){
  Serial.println("init mirror position in down position");
  myservo.write(servoPosDown);
  servoPos = servoPosDown;
  Blynk.virtualWrite(V0, 0);
  delay(1000);
}
