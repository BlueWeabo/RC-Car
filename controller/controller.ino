
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BACK 2
#define SLOW 3
#define NORMAL 4
#define FAST 5

#define BACK_PRESSED 0b00000001
#define SLOW_PRESSED 0b00000010
#define NORMAL_PRESSED 0b00000100
#define FAST_PRESSED 0b000001000

#define SLOW_MODE 0b00000001
#define NORMAL_MODE 0b00000010
#define FAST_MODE 0b0000000100

#define SPEED A1
#define TURN A0

#define CE 7
#define CSN 8

RF24 radio(CE, CSN); // CE, CSN

const byte address[6] = "CRACK";

byte speed = 0;
byte dir = 1;
byte turn = 90;
byte mode = NORMAL_MODE;

byte btnPressed = 0;

void setup() {
    Serial.begin(9600);
    pinMode(BACK, INPUT);
    pinMode(SLOW, INPUT);
    pinMode(NORMAL, INPUT);
    pinMode(FAST, INPUT);
    pinMode(SPEED, INPUT);
    pinMode(TURN, INPUT);
    while (!radio.begin()) {
        Serial.println("FUUUCK");
    }
    radio.openWritingPipe(address);
    radio.stopListening();
}

void loop() {
    byte back = digitalRead(BACK);
    if (back) {
        if ((btnPressed & BACK_PRESSED) != BACK_PRESSED) {
            btnPressed = btnPressed | BACK_PRESSED;
            dir = 0;
            mode = NORMAL_MODE;
        }
    } else {
        btnPressed = btnPressed ^ BACK_PRESSED;
    }
    byte slow = digitalRead(SLOW);
    if (slow) {
        if ((btnPressed & SLOW_PRESSED) != SLOW_PRESSED) {
            btnPressed = btnPressed | SLOW_PRESSED;
            dir = 0;
            mode = NORMAL_MODE;
        }
    } else {
        btnPressed = btnPressed ^ SLOW_PRESSED;
    }
    byte normal = digitalRead(NORMAL);
    if (normal) {
        if ((btnPressed & NORMAL_PRESSED) != NORMAL_PRESSED) {
            btnPressed = btnPressed | NORMAL_PRESSED;
            dir = 1;
            mode = NORMAL_MODE;
        }
    } else {
        btnPressed = btnPressed ^ NORMAL_PRESSED;
    }
    byte fast = digitalRead(FAST);
    if (fast) {
        if ((btnPressed & FAST_PRESSED) != FAST_PRESSED) {
            btnPressed = btnPressed | FAST_PRESSED;
            dir = 1;
            mode = FAST_MODE;
        }
    } else {
        btnPressed = btnPressed ^ FAST_PRESSED;
    }
    int speedRead = analogRead(SPEED);
    byte speed = 0;
    switch (mode) {
        case SLOW_MODE:
            speed = map(speedRead, 0, 1023, 0, 51);
            break;
        case NORMAL_MODE:
            speed = map(speedRead, 0, 1023, 0, 255);
            break;
        case FAST_MODE:
            speed = speedRead > 255 ? 255 : speedRead;
            break;
        default:
            speed = map(speedRead, 0, 1023, 0, 255);
            break;
    }
    int turnRead = analogRead(TURN);
    byte turn = 83;
    if (turnRead < 450 || turnRead > 572) {
        turn = map(turnRead, 0, 1023, 63, 110);
    }
    const byte data[] = {dir, speed, turn};
    radio.write(&data, sizeof(data));
    Serial.println("we sending");
}
