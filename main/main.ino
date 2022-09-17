
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <AFMotor.h>
#include <Servo.h>   
#define echoPin 13 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D3 Arduino to pin Trig of HC-SR04
//#define LED 11  // The pin the LED is connected to
//
AF_DCMotor motor1(1);  
Servo servo1;  

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 5; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 2; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void setup() {
  Serial.begin(115200);           // set up Serial library at 9600 bps
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);
  
  motor1.setSpeed(255);     // set the speed to 200/255
  servo1.attach (10);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
//  pinMode(LED, OUTPUT); // Declare the LED as an output

  player.setTimeOut(500); //Set serial communictaion time out 500ms
  Serial.println("Setup complete");
}

bool trigger() {
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement
  int triggerDistance = 2;
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

void speak(){
  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
    Serial.println("OK");

    // Set volume to maximum (0 to 30).
    player.volume(5);
    // Play the first MP3 file on the SD card
    player.play(1);
//    if (player.available()) {
//      printDetail(player.readType(), player.read()); //Print the detail message from DFPlayer to handle different errors and states.
//    }
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
    Serial.println(player.available());
    if (player.available()) {
      printDetail(player.readType(), player.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}

void loop() {
  if (trigger()) {
    Serial.println("tick");
    // start playing sound
    speak();
    // start waving the hand
    motor1.run(FORWARD);
    // start nodding the head
    nodTheHead(10);
    // stop waving
    motor1.run(RELEASE);
    delay(6000);
  }
}
