
#include <AFMotor.h>
#include <Servo.h>   
#define echoPin 13 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D3 Arduino to pin Trig of HC-SR04

AF_DCMotor motor1(1);  
Servo servo1;  

// Globals
// Se the distance at which a person will trigger the robot
int triggerDistance = 20;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  motor1.setSpeed(200);     // set the speed to 200/255
  servo1.attach (10);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.println("Setup complete");
}

bool trigger() {
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //Serial.print("Less than ");
  //Serial.println(triggerDistance);
  if (distance > 0 && distance < triggerDistance) {
    Serial.println("Returning true");
    return true;
  }
  else {
    //Serial.println("Returning false");
    return false;
  }
}

void nodTheHead(int num_waves) {
  Serial.println("Nodding the head");
  int servo_position = 0 ;
  int x = 0;
  for (x = 0; x < num_waves; x +=1){
   for (servo_position = 0; servo_position <=120; servo_position +=1){
      servo1.write(servo_position);
      delay(1);
    }
    for (servo_position=120; servo_position >= 0; servo_position -=1){
      servo1.write(servo_position);
      delay(1);
    }
  }
}

void loop() {
  if (trigger()) {
    Serial.println("tick");
    // start waving the hand
    motor1.run(FORWARD);
    // start nodding the head
    nodTheHead(10);
    // stop waving
    motor1.run(RELEASE);
    delay(2000);
  }
}
