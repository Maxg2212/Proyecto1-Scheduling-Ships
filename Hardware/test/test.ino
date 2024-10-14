#include <Arduino.h>

char incomingByte;

void setup() {
  // Configuración de los pines 8, 9, 10, 11 y 12 como salida
  pinMode(8, OUTPUT);   
  pinMode(9, OUTPUT);   
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // Inicializa la comunicación serial a 9600 baudios
  Serial.begin(9600);   
}

void loop() {
  // Verifica si hay datos disponibles en el puerto serie
  if (Serial.available()) {
    incomingByte = Serial.read();  // Leer un solo carácter
    Serial.println(incomingByte);   // Imprimir el carácter recibido en el monitor serial

    // Controlar los pines 8 y 9 con las señales 'a' y 'b'
    if (incomingByte == 'a') {
      digitalWrite(8, HIGH);  // Encender el pin 8
      digitalWrite(9, LOW);   // Apagar el pin 9
      digitalWrite(10, LOW);  // Encender el pin 10
      digitalWrite(11, HIGH);  // Encender el pin 11
      digitalWrite(12, HIGH);  // Encender el pin 12
    } 
    else if (incomingByte == 'b') {
      digitalWrite(8, LOW);   // Apagar el pin 8
      digitalWrite(9, HIGH);  // Encender el pin 9
      digitalWrite(10, HIGH);  // Encender el pin 10
      digitalWrite(11, HIGH);  // Encender el pin 11
      digitalWrite(12, LOW);   // Apagar el pin 12 si estaba encendido
    }
  }
}
