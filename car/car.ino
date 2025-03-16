#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE 7
#define CSN 8

#define MOTOR 3
#define DIR 4
#define SERVO 5

Servo servo;
RF24 radio(CE, CSN); // CE, CSN

const byte address[6] = "CRACK";

void setup() {
    Serial.begin(9600);
    pinMode(MOTOR, OUTPUT);
    pinMode(DIR, OUTPUT);
    servo.attach(SERVO, 500, 2500);
    while(!radio.begin()) {
        Serial.println("radio is fucked");
    }
    radio.openReadingPipe(0, address);
    radio.startListening();
}

void loop() {
    if (!radio.available()) {
        Serial.println("Hey we got nothing");
        digitalWrite(DIR, 1);
        analogWrite(MOTOR, 0);
        servo.write(83);
        delay(1000);
        servo.write(65);
        delay(1000);
        servo.write(110);
        delay(1000);
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
    Serial.println("we got data");
}
