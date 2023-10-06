//PLACA 1
#define LED VERDE PB7
#define LED AMARILLO PB8
#define LED ROJO PB9
#define BUZZER PA0
#define BTN PA1
#define BTN ALARMA PA2
//PLACA 2
LED VERDE PA10
#define LED AMARILLO PB5
#define LED ROJO PB6
#define BUZZER PA3
#define BTN PA4
#define BTN ALARMA PA5
//PLACA 3
#define LED VERDE PB11
#define LED AMARILLO PB1
#define LED ROJO PB0
#define BUZZER PB9
#define BTN PB7
#define BTN ALARMA PB8
//PLACA 4
#define LED VERDE PB12
#define LED AMARILLO PB13
#define LED ROJO PB14
#define BUZZER PA9
#define BTN PA0
#define BTN ALARMA PA1

// Definición de estados
enum Estado {
  VERDE,
  ROJO_AMARILLO,
  ROJO,
  ESPERA_CRUCE,
  EMERGENCIA
};

Estado estadoActual = VERDE;
unsigned long tiempoInicioEstado;
const unsigned long tiempoVerde = 2000;  // 2 segundos en milisegundos
const unsigned long tiempoRojoAmarillo = 1000;  // 1 segundo en milisegundos
const unsigned long tiempoEsperaCruce = 5000;  // 5 segundos en milisegundos

void setup() {
  // Configurar pines como entradas o salidas según corresponda
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_PEATON, INPUT_PULLUP);
  pinMode(BTN_ALARMA, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned long tiempoActual = millis();

  switch (estadoActual) {
    case VERDE:
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_AMARILLO, LOW);
      digitalWrite(LED_ROJO, LOW);
      // Verificar si se presiona el botón de cruce
      if (digitalRead(BTN_PEATON) == LOW) {
        estadoActual = ESPERA_CRUCE;
        tiempoInicioEstado = tiempoActual;
      }
      break;

    case ROJO_AMARILLO:
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARILLO, HIGH);
      digitalWrite(LED_ROJO, HIGH);
      if (tiempoActual - tiempoInicioEstado >= tiempoRojoAmarillo) {
        estadoActual = VERDE;
        tiempoInicioEstado = tiempoActual;
      }
      break;

    case ROJO:
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARILLO, LOW);
      digitalWrite(LED_ROJO, HIGH);
      // Verificar si se presiona el botón de cruce
      if (digitalRead(BTN_PEATON) == LOW) {
        estadoActual = ESPERA_CRUCE;
        tiempoInicioEstado = tiempoActual;
      }
      // Verificar si se activa la emergencia
      if (digitalRead(BTN_ALARMA) == LOW) {
        estadoActual = EMERGENCIA;
        tiempoInicioEstado = tiempoActual;
        // Activar alarma y enviar mensaje por puerto serie (a implementar)
        digitalWrite(BUZZER, HIGH);
        Serial.println("Emergencia activada.");
      }
      break;

    case ESPERA_CRUCE:
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARILLO, LOW);
      digitalWrite(LED_ROJO, LOW);
      // Permitir que el peatón cruce durante el tiempo especificado
      if (tiempoActual - tiempoInicioEstado >= tiempoEsperaCruce) {
        estadoActual = VERDE;
        tiempoInicioEstado = tiempoActual;
      }
      break;

    case EMERGENCIA:
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARILLO, LOW);
      digitalWrite(LED_ROJO, HIGH);
      // Verificar si se desactiva la emergencia
      if (digitalRead(BTN_ALARMA) == LOW) {
        estadoActual = ROJO;
        tiempoInicioEstado = tiempoActual;
        // Detener alarma y enviar mensaje por puerto serie (a implementar)
        digitalWrite(BUZZER, LOW);
        Serial.println("Emergencia desactivada.");
      }
      break;
  }
}
