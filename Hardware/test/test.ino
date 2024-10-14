#include <Arduino.h>

char incomingByte;

void setup() {
  pinMode(8, OUTPUT);   // Pin 8 como salida
  pinMode(9, OUTPUT);   // Pin 9 como salida
  Serial.begin(9600);   // Inicializa la comunicación serial a 9600 baudios
}

void loop() {
  if (Serial.available()) {
    incomingByte = Serial.read();  // Leer un solo carácter
    Serial.println(incomingByte);   // Imprimir el carácter recibido en el monitor serial

    // Controlar los pines 8 y 9
    if (incomingByte == 'a') {
      digitalWrite(8, HIGH);  // Encender el pin 8
      digitalWrite(9, LOW);   // Apagar el pin 9
    } 
    else if (incomingByte == 'b') {
      digitalWrite(8, LOW);   // Apagar el pin 8
      digitalWrite(9, HIGH);  // Encender el pin 9
    }
  }
}
