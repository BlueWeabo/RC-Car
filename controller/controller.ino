
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PARKING 2
#define SLOW 3
#define NORMAL 4
#define FAST 5

#define PARKING_PRESSED 0b00000001
#define SLOW_PRESSED 0b00000010
#define NORMAL_PRESSED 0b00000100
#define FAST_PRESSED 0b000001000

#define PARKING_MODE 0b00000001
#define SLOW_MODE 0b00000010
#define NORMAL_MODE 0b00000100
#define FAST_MODE 0b0000001000

#define SPEED A1
#define TURN A0

#define CE 7
#define CSN 8

//#define DEBUG

RF24 radio(CE, CSN); // CE, CSN

const byte address[6] = "CRACK";

byte speed = 0;
byte dir = 0;
byte turn = 90;
byte mode = NORMAL_MODE;

byte btnPressed = 0;

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    #endif
    pinMode(PARKING, INPUT);
    pinMode(SLOW, INPUT);
    pinMode(NORMAL, INPUT);
    pinMode(FAST, INPUT);
    pinMode(SPEED, INPUT);
    pinMode(TURN, INPUT);
    while (!radio.begin()) {
    #ifdef DEBUG
        Serial.println("radio is not working");
    #endif
    }
    radio.openWritingPipe(address);
    radio.stopListening();
    radio.setPALevel(RF24_PA_LOW);
}

void loop() {
    byte parking = digitalRead(PARKING);
    if (parking) {
        if ((btnPressed & PARKING_PRESSED) != PARKING_PRESSED) {
            btnPressed = btnPressed | PARKING_PRESSED;
            mode = PARKING_MODE;
        }
    } else if ((btnPressed & PARKING_PRESSED) == PARKING_PRESSED){
        btnPressed = btnPressed ^ PARKING_PRESSED;
    }
    byte slow = digitalRead(SLOW);
    if (slow) {
        if ((btnPressed & SLOW_PRESSED) != SLOW_PRESSED) {
            btnPressed = btnPressed | SLOW_PRESSED;
            mode = SLOW_MODE;
        }
    } else if ((btnPressed & SLOW_PRESSED) == SLOW_PRESSED){
        btnPressed = btnPressed ^ SLOW_PRESSED;
    }
    byte normal = digitalRead(NORMAL);
    if (normal) {
        if ((btnPressed & NORMAL_PRESSED) != NORMAL_PRESSED) {
            btnPressed = btnPressed | NORMAL_PRESSED;
            mode = NORMAL_MODE;
        }
    } else if ((btnPressed & NORMAL_PRESSED) == NORMAL_PRESSED){
        btnPressed = btnPressed ^ NORMAL_PRESSED;
    }
    byte fast = digitalRead(FAST);
    if (fast) {
        if ((btnPressed & FAST_PRESSED) != FAST_PRESSED) {
            btnPressed = btnPressed | FAST_PRESSED;
            mode = FAST_MODE;
        }
    } else if ((btnPressed & FAST_PRESSED) == FAST_PRESSED){
        btnPressed = btnPressed ^ FAST_PRESSED;
    }
    int speedRead = analogRead(SPEED);
    byte speed = 0;
    switch (mode) {
        case PARKING_MODE:
            if (speedRead < 512 - 50) {
                dir = 1;
                speed = map(speedRead, 0, 512 - 50, 51, 0);
            } else if (speedRead > 512 + 50) {
                dir = 0;
                speed = map(speedRead, 512 + 50, 1023, 0, 51);
            }
            break;
        case SLOW_MODE:
            if (speedRead < 512 - 50) {
                dir = 1;
                speed = map(speedRead, 0, 512 - 50, 85, 0);
            } else if (speedRead > 512 + 50) {
                dir = 0;
                speed = map(speedRead, 512 + 50, 1023, 0, 85);
            }
            break;
        case NORMAL_MODE:
            if (speedRead < 512 - 50) {
                dir = 1;
                speed = map(speedRead, 0, 512 - 50, 204, 0);
            } else if (speedRead > 512 + 50) {
                dir = 0;
                speed = map(speedRead, 512 + 50, 1023, 0, 204);
            }
            break;
        case FAST_MODE:
            if (speedRead < 512 - 50) {
                dir = 1;
                speed = map(speedRead, 0, 512 - 50, 255, 0);
            } else if (speedRead > 562) {
                dir = 0;
                speed = map(speedRead, 512 + 50, 1023, 0, 255);
            }
            break;
        default:
            speed = map(speedRead, 0, 1023, 0, 255);
            break;
    }
    int turnRead = analogRead(TURN);
    byte turn = 83;
    if (turnRead < 502 || turnRead > 522) {
        turn = map(turnRead, 0, 1023, 63, 110);
    }
    const byte data[] = {dir, speed, turn};
    radio.write(&data, sizeof(data));
    #ifdef DEBUG
    Serial.println("we sending");
    Serial.println(dir);
    Serial.println(speed);
    Serial.println(turn);
    #endif
}
