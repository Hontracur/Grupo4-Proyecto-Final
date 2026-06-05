#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment displayMeta;
Adafruit_7segment displayCrono;

const int BOTON = A2;

const int LED_ROJO = 6;
const int LED_VERDE = 3;
const int LED_AZUL = 5;

bool corriendo = false;
unsigned long inicioTiempo = 0;

void setup() {

  displayMeta.begin(0x71);
  displayCrono.begin(0x70);

  pinMode(BOTON, INPUT_PULLUP);

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);

  displayCrono.print(0);
  displayCrono.drawColon(true);
  displayCrono.writeDisplay();
}

void loop() {

  // Meta fija 10:00
  displayMeta.print(1000);
  displayMeta.drawColon(true);
  displayMeta.writeDisplay();

  static bool ultimoEstado = HIGH;
  bool estado = digitalRead(BOTON);

  // Botón presionado
  if (ultimoEstado == HIGH && estado == LOW) {

    if (!corriendo) {

      corriendo = true;
      inicioTiempo = millis();

      digitalWrite(LED_ROJO, LOW);
      digitalWrite(LED_VERDE, LOW);
    }
    else {

      corriendo = false;

      unsigned long tiempo = millis() - inicioTiempo;

      int segundos = tiempo / 1000;
      int centesimas = (tiempo % 1000) / 10;

      // Gana si está entre 9.95 y 10.05 segundos
      if (tiempo >= 9950 && tiempo <= 10050) {
        digitalWrite(LED_VERDE, HIGH);
      } else {
        digitalWrite(LED_ROJO, HIGH);
      }

      delay(3000);

      digitalWrite(LED_ROJO, LOW);
      digitalWrite(LED_VERDE, LOW);

      displayCrono.print(0);
      displayCrono.drawColon(true);
      displayCrono.writeDisplay();
    }

    delay(200);
  }

  ultimoEstado = estado;

  if (corriendo) {

    unsigned long tiempo = millis() - inicioTiempo;

    int segundos = tiempo / 1000;
    int centesimas = (tiempo % 1000) / 10;

    int valor = segundos * 100 + centesimas;

    displayCrono.print(valor);
    displayCrono.drawColon(true);
    displayCrono.writeDisplay();
  }
}
