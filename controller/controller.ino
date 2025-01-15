#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSN_PIN 8
#define CE_PIN 7
#define X_PIN A1
#define Y_PIN A2
#define R_BTN 3
#define P_BTN 4
#define N_BTN 5
#define T_BTN 6
#define ADDRESS { 'C', 'R', 'A', 'C', 'K' }

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
    radio.begin();
    radio.openWritingPipe(ADDRESS);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
    pinMode(R_BTN, INPUT);
    pinMode(P_BTN, INPUT);
    pinMode(N_BTN, INPUT);
    pinMode(T_BTN, INPUT);
}

void loop() {
    int x = analogRead(X_PIN);
    int y = analogRead(Y_PIN);
    int data[] = {x, y};
    radio.write(&data, sizeof(data));
    delay(10);
}
