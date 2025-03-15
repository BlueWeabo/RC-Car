#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "01010";

void setup() {
  Serial.begin(9600);
  while(!radio.begin()) {
        Serial.println("radio is fucked");
    }
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
    if (!radio.available()) {
        Serial.println("Hey we got nothing");
    }
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
}
