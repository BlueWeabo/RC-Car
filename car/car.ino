#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE 7
#define CSN 8

#define MOTOR 3
#define DIR 4
#define SERVO 5

//#define DEBUG

Servo servo;
RF24 radio(CE, CSN); // CE, CSN

const byte address[6] = "CRACK";

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    #endif
    pinMode(MOTOR, OUTPUT);
    pinMode(DIR, OUTPUT);
    servo.attach(SERVO, 500, 2500);
    while(!radio.begin()) {
        #ifdef DEBUG
        Serial.println("radio is not working");
        #endif
    }
    radio.openReadingPipe(0, address);
    radio.startListening();
}

void loop() {
    if (!radio.available()) {
        #ifdef DEBUG
        Serial.println("Hey we got nothing");
        #endif
        digitalWrite(DIR, 1);
        analogWrite(MOTOR, 0);
        servo.write(83);
        return;
    }
    byte data[3] = "";
    radio.read(&data, sizeof(data));
    byte dir = data[0];
    byte speed = data[1];
    byte turn = data[2];
    digitalWrite(DIR, dir);
    analogWrite(MOTOR, speed);
    servo.write(turn);
    #ifdef DEBUG
    Serial.println("we got data");
    Serial.print("speed:");
    Serial.println(speed);
    Serial.print("dir:");
    Serial.println(dir);
    Serial.print("turn:");
    Serial.println(turn);
    #endif
}
