#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CSN_PIN 8
#define CE_PIN 7
#define MOTOR_PIN 3
#define SERVO_PIN 4
#define ADDRESS { 'C', 'R', 'A', 'C', 'K' }

RF24 radio(CE_PIN, CSN_PIN);
Servo motor;
Servo servo;

void setup() {
    motor.attach(MOTOR_PIN, 1000, 2000);
    servo.attach(SERVO_PIN);
    radio.begin();
    radio.openReadingPipe(0, ADDRESS);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        int data[4] = {};
        radio.read(&data, sizeof(data));
        int x = data[0];
        int y = data[1];
        if (x > 550) {
            x = map(x, 512, 1023, 0, 180);
            motor.write(x);
        }
        if (y > 552) {
            y = map(y, 0, 1023, 0, 180);
            servo.write(y);
        }
        if (y < 472) {
            y = map(y, 0, 1023, 0, 180);
            servo.write(y);
        }
        if (y < 552 && y > 472) {
            servo.write(90);
        }
    }
}
