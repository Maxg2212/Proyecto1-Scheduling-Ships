#include <Arduino.h>

char incomingByte;

void setup() {
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    incomingByte = Serial.read();  // Leer un solo carácter
    Serial.println(incomingByte);

    if(incomingByte == 'a'){
       digitalWrite(11, HIGH);  // Encender el LED
    }

    if(incomingByte == 'b'){
       digitalWrite(11, LOW);   // Apagar el LED
    }
  }
}