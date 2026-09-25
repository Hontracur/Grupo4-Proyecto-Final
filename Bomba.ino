#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>   


Adafruit_LiquidCrystal lcd1(0);




const byte FILAS = 4;
const byte COLUMNAS = 4;

char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas[FILAS] = {9, 8, 7, 6};
byte pinesColumnas[COLUMNAS] = {5, 4, 3, 2};

Keypad teclado = Keypad(
  makeKeymap(teclas),
  pinesFilas,
  pinesColumnas,
  FILAS,
  COLUMNAS
);



// LED 1
const int LED1_VERDE = A0;
const int LED1_ROJO = A1;

// LED 2
const int LED2_VERDE = A2;
const int LED2_ROJO = A3;

// LED 3






const int PIEZO = 12;


String preguntas[6] = {
  "RAIZ DE 25?",
  "MITAD DE 10M?",
  "12 X 8?",
  "RAIZ DE 144?",
  "25% DE 200?",
  "2 ELEVADO 5?"
};

long respuestas[6] = {
  5,
  5000000,
  96,
  12,
  50,
  32
};



int preguntaActual = 0;

String respuesta = "";

unsigned long tiempoInicio;

const unsigned long tiempoTotal = 30000;

bool juegoTerminado = false;

int segundoAnterior = -1;



void setup() {

  // LEDs
  pinMode(LED1_VERDE, OUTPUT);
  pinMode(LED1_ROJO, OUTPUT);

  pinMode(LED2_VERDE, OUTPUT);
  pinMode(LED2_ROJO, OUTPUT);




  // Piezo

  pinMode(PIEZO, OUTPUT);


  // Al comenzar:
  // todos los LEDs en BLANCO

  digitalWrite(LED1_ROJO, LOW);
  digitalWrite(LED2_ROJO, LOW);

  digitalWrite(LED1_VERDE, LOW);
  digitalWrite(LED2_VERDE, LOW);

  // Pantalla

  lcd1.begin(16, 2);

  lcd1.setCursor(0, 0);
  lcd1.print("BOMBA");

  lcd1.setCursor(0, 1);
  lcd1.print("MATEMATICA" );  

  delay(2000);


  // Comenzar juego

  tiempoInicio = millis();

  mostrarPregunta();
}




void loop() {

  if (juegoTerminado) {
    return;
  }


 
  unsigned long tiempoPasado = millis() - tiempoInicio;

  int segundos = 30 - (tiempoPasado / 1000);


  if (segundos <= 0) {

    explosion();

    return;
  }


  // Actualizar segundos

  if (segundos != segundoAnterior) {

    segundoAnterior = segundos;

    lcd1.setCursor(13, 0);

    lcd1.print(" ");

    lcd1.setCursor(13, 0);

    lcd1.print(segundos);
  }


 
  char tecla = teclado.getKey();


  if (tecla) {

    // BORRAR

    if (tecla == '*') {

      respuesta = "";

      lcd1.setCursor(0, 1);
      lcd1.print(" ");

      lcd1.setCursor(0, 1);
      lcd1.print("R:");
    }


    // CONFIRMAR

    else if (tecla == '#') {

      comprobarRespuesta();
    }


    // NUMEROS

    else if (tecla >= '0' && tecla <= '9') {

      if (respuesta.length() < 10) {

        respuesta += tecla;

        lcd1.setCursor(0, 1);

        lcd1.print("R:");
        lcd1.print(respuesta);
      }
    }
  }
}



void mostrarPregunta() {

  lcd1.clear();

  lcd1.setCursor(0, 0);

  lcd1.print("P");
  lcd1.print(preguntaActual + 1);
  lcd1.print("/6 ");

  int segundos = 30 - ((millis() - tiempoInicio) / 1000);

  lcd1.print("T:");
  lcd1.print(segundos);


  lcd1.setCursor(0, 1);

  lcd1.print(preguntas[preguntaActual]);
}




void comprobarRespuesta() {

  if (respuesta == "") {
    return;
  }


  long numero = respuesta.toInt();


 
  if (numero == respuestas[preguntaActual]) {


    // Preguntas 1 a 3
    // LED 1 pasa a verde

    if (preguntaActual == 2) {

      digitalWrite(LED1_ROJO, LOW);
      digitalWrite(LED1_VERDE, HIGH);
    }


    // Preguntas 3 a 6
    // LED 2 pasa a verde

    if (preguntaActual == 5) {

      digitalWrite(LED2_ROJO, LOW);
      digitalWrite(LED2_VERDE, HIGH);
    }


    // Preguntas 5 y 6
    // LED 3 pasa a verde

    if (preguntaActual == 5) {

     

      victoria();

      return;
    }


    // Siguiente pregunta

    preguntaActual++;

    respuesta = "";

    delay(500);

    mostrarPregunta();
  }


 
  else {

    explosion();
  }
}




void victoria() {

  juegoTerminado = true;

  lcd1.clear();

  lcd1.setCursor(0, 0);
  lcd1.print("BOMBA");

  lcd1.setCursor(0, 1);
  lcd1.print("DESACTIVADA!");


  // Melodia

  tone(PIEZO, 523);
  delay(200);

  tone(PIEZO, 659);
  delay(200);

  tone(PIEZO, 784);
  delay(200);

  tone(PIEZO, 1047);
  delay(400);

  tone(PIEZO, 784);
  delay(200);

  tone(PIEZO, 1047);
  delay(600);

  noTone(PIEZO);
}



void explosion() {

  juegoTerminado = true;

  lcd1.clear();

  lcd1.setCursor(0, 0);
  lcd1.print("!!! BOOM !!!");

  lcd1.setCursor(0, 1);
  lcd1.print("BOMBA EXPLOTO");


  // Todos los LEDs rojos

  digitalWrite(LED1_VERDE, LOW);
  digitalWrite(LED2_VERDE, LOW);

  digitalWrite(LED1_ROJO, HIGH);
  digitalWrite(LED2_ROJO, HIGH);


  // Sonido de explosion

  for (int frecuencia = 1200;
       frecuencia >= 100;
       frecuencia -= 50) {

    tone(PIEZO, frecuencia);

    delay(15);
  }

  noTone(PIEZO);


  // Parpadeo rojo

  for (int i = 0; i < 6; i++) {

    digitalWrite(LED1_ROJO, LOW);
    digitalWrite(LED2_ROJO, LOW);

    delay(100);

    digitalWrite(LED1_ROJO, HIGH);
    digitalWrite(LED2_ROJO, HIGH);

    delay(100);
  }
}