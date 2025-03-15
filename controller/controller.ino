
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "01010";

void setup() {
    Serial.begin(9600);
  while (!radio.begin()) {
        Serial.println("FUUUCK");
    }
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  const char text[] = "Testing out radio";
  radio.write(&text, sizeof(text));
}
