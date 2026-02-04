#include <Arduino.h>
#include "BluetoothSerial.h"

// Pin definitions for motors and LEDs
#define MotorDerechoAdelante 32
#define MotorDerechoAtras 33
#define MotorIzquierdoAdelante 25
#define MotorIzquierdoAtras 26
#define ledRojo 19
#define ledVerde 18
#define ledAzul 5

// Configuration for motor control
const int freq = 5000;   // Hz
const int motorChannel = 0;
const int resolution = 8;

// Define a constant for velocity
const int vel = 27;

// Enable Bluetooth configuration check
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error "Bluetooth is not enabled! Please run `make menuconfig` to enable it"
#endif

// Create an instance of the BluetoothSerial class
BluetoothSerial SerialBT;

/**
 * Function to control motor directions manually.
 *
 * @param valorMotorDerAde  Value for MotorDerechoAdelante pin (0-255)
 * @param valorMotorDerAtr  Value for MotorDerechoAtras pin (0-255)
 * @param valorMotorIzqrAde Value for MotorIzquierdoAdelante pin (0-255)
 * @param valorMotorIzqAtr  Value for MotorIzquierdoAtras pin (0-255)
 */
void direccionesManual(int valorMotorDerAde, int valorMotorDerAtr, int valorMotorIzqrAde, int valorMotorIzqAtr) {
  digitalWrite(MotorDerechoAdelante, valorMotorDerAde);
  digitalWrite(MotorDerechoAtras, valorMotorDerAtr);
  digitalWrite(MotorIzquierdoAdelante, valorMotorIzqrAde);
  digitalWrite(MotorIzquierdoAtras, valorMotorIzqAtr);
}

/**
 * Function to control LED colors manually.
 *
 * @param ledRojo1    Value for ledRojo pin (0-255)
 * @param ledAzul1    Value for ledAzul pin (0-255)
 * @param ledVerde1   Value for ledVerde pin (0-255)
 */
void encendidoLedManual(int ledRojo1, int ledAzul1, int ledVerde1) {
  digitalWrite(ledRojo, ledRojo1);
  digitalWrite(ledVerde, ledVerde1);
  digitalWrite(ledAzul, ledAzul1);
}

/**
 * Setup function to configure pins and start the motor control.
 */
void setup() {
  // Start Bluetooth serial
  SerialBT.begin("MonitosESP32");
  Serial.begin(9600);

  // Set pin modes for motors and LEDs
  pinMode(MotorDerechoAdelante, OUTPUT);
  pinMode(MotorDerechoAtras, OUTPUT);
  pinMode(MotorIzquierdoAdelante, OUTPUT);
  pinMode(MotorIzquierdoAtras, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // Configure motor control
  ledcSetup(motorChannel, freq, resolution);
  const int dutyCycle = 110;
  ledcWrite(motorChannel, dutyCycle);
  analogWrite(vel, dutyCycle);

  // Initialize motor directions to low (stop)
  digitalWrite(MotorDerechoAdelante, LOW);
  digitalWrite(MotorDerechoAtras, LOW);
  digitalWrite(MotorIzquierdoAdelante, LOW);
  digitalWrite(MotorIzquierdoAtras, LOW);
}

/**
 * Loop function to continuously check for incoming Bluetooth data.
 */
void loop() {
  // Check if there is incoming data from the Bluetooth connection
  if (SerialBT.available() > 0) {
    char caracter = SerialBT.read();

    switch (caracter) {
      case 'A':  // Adelante: COLOR IDENTIFICATIVO 'VERDE'
        direccionesManual(255, 0, 255, 0);
        encendidoLedManual(0, 0, 255);
        Serial.println("AdelanteM");
        break;
      case 'D':  // Derecha: COLOR IDENTIFICATIVO 'MAGENTA'
        direccionesManual(255, 0, 0, 0);
        encendidoLedManual(255, 255, 0);
        Serial.println("DerechaM");
        break;
      case 'I':  // Izquierda: COLOR IDENTIFICATIVO 'AMARILLO'
        direccionesManual(0, 0, 255, 0);
        encendidoLedManual(255, 0, 255);
        Serial.println("IzquierdaM");
        break;
      case 'R':  // Atras: COLOR IDENTIFICATIVO 'AZUL'
        direccionesManual(0, 255, 0, 255);
        encendidoLedManual(0, 255, 0);
        Serial.println("AtrasM");
        break;
      case 'P':  // Parar: COLOR IDENTIFICATIVO 'ROJO'
        direccionesManual(0, 0, 0, 0);
        encendidoLedManual(255, 0, 0);
        Serial.println("PararM");
        break;
      default:
        printf("oK");
    }
  }
}
