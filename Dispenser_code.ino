#include <ESP32Servo.h>
#include <Arduino.h>
#include "BluetoothSerial.h"
#include "DHT.h"
#define DHTPIN 33  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11

const int LEDRedh = 32;
const int LEDRedt = 27;
const int LEDblue_one = 23;
const int LEDblue_two = 25;
const int LEDblue_three = 26;
static const int servoPin1 = 13;
static const int servoPin2 = 16;
static const int servoPin3 = 4;

Servo servo1; //first servo object
Servo servo2;
Servo servo3;
BluetoothSerial SerialBT;
DHT dht(DHTPIN, DHTTYPE);

// notes in the melody:
int melody[] = {
  440, 440, 0, 440, 0, 349, 440, 0, 523, 0, 392, 0,
  349, 392, 0, 294, 392, 440, 415, 392, 349, 440, 523, 587
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4,  // First melody, first line
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4,  // First melody, second line
};

void setup() {
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  dht.begin();
  pinMode(LEDRedt, OUTPUT);
  pinMode(LEDRedh, OUTPUT);
  pinMode(LEDblue_one, OUTPUT);
  pinMode(LEDblue_two, OUTPUT);
  pinMode(LEDblue_three, OUTPUT);
  SerialBT.begin("ESP32-Bluetooth");
}

void humidity_temp(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
  return; 
}
if(h>90){
  digitalWrite(LEDRedh, HIGH);
}
else{
  digitalWrite(LEDRedh, LOW);
}
if(t>40){
  digitalWrite(LEDRedt, HIGH);
}
else{
  digitalWrite(LEDRedt, LOW);
}
 delay(200); 
}

void playMelody(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 24; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(14, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(14);
  }
}

void Bluetooth_control(){
  if (SerialBT.available()){
    char command = SerialBT.read();
    
        if(command == '1'){
          move_one();
          delay(500);
          digitalWrite(LEDblue_one, HIGH);
          playMelody();
          digitalWrite(LEDblue_one, LOW);
        }
        else if(command == '2'){
          move_two();
          delay(500);
          digitalWrite(LEDblue_two, HIGH);
          playMelody();
          digitalWrite(LEDblue_two, LOW);
        }
        else if(command == '3'){
          move_three();
          delay(500);
          digitalWrite(LEDblue_three, HIGH);
          playMelody();
          digitalWrite(LEDblue_three, LOW);
        }    
  }
}

void move_one() {
  servo1.write(45);
  delay(450);
  servo1.write(90);
  exit;
}

void move_two() {
  servo2.write(135);
  delay(420);
  servo2.write(90);
  exit;
}

void move_three() {
  servo3.write(135);
  delay(600);
  servo3.write(0);
  exit;
}

void loop() {
  Bluetooth_control();
  delay(100);
  humidity_temp();
}
