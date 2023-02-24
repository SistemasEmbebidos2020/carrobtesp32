//Librerias
#include <Arduino.h>
#include "BluetoothSerial.h"

//Definicion de variables
#define MotorDerechoAdelante 32
#define MotorDerechoAtras 33
#define MotorIzquierdoAdelante 25
#define MotorIzquierdoAtras 26
#define ledRojo 19
#define ledVerde 18
#define ledAzul 5
char caracter;
const int freq = 5000;
const int motorChannel = 0;
const int resolution = 8;
const int vel = 27;

//Configuracion Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//Funcion que determina el manejo manual del carro, segun la acciones de los botones en la appp
void direccionesManual(int valorMotorDerAde, int valorMotorDerAtr, int valorMotorIzqrAde, int valorMotorIzqAtr){
  digitalWrite(MotorDerechoAdelante, valorMotorDerAde);
  digitalWrite(MotorDerechoAtras, valorMotorDerAtr);
  digitalWrite(MotorIzquierdoAdelante, valorMotorIzqrAde);
  digitalWrite(MotorIzquierdoAtras, valorMotorIzqAtr);
}

//Mostrara el color asignado por cada opcion de manera manual
void encendidoLedManual(int ledRojo1, int ledAzul1, int ledVerde1){
  digitalWrite(ledRojo,ledRojo1);
  digitalWrite(ledVerde,ledVerde1);
  digitalWrite(ledAzul,ledAzul1);
}

void setup() {
  SerialBT.begin("MonitosESP32");
  Serial.begin(9600);
  //Seteando variables de salida
  pinMode(MotorDerechoAdelante, OUTPUT);
  pinMode(MotorDerechoAtras, OUTPUT);
  pinMode(MotorIzquierdoAdelante, OUTPUT);
  pinMode(MotorIzquierdoAtras, OUTPUT);
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);

  ledcSetup(motorChannel, freq, resolution);
  const int dutyCycle = 110;
  ledcWrite(motorChannel, dutyCycle);
  analogWrite(vel,dutyCycle);  

  digitalWrite(MotorDerechoAdelante, LOW);
  digitalWrite(MotorDerechoAtras, LOW);
  digitalWrite(MotorIzquierdoAdelante, LOW);
  digitalWrite(MotorIzquierdoAtras, LOW);

}

void loop() {

  if (SerialBT.available() > 0) { //Si recibe algo por bt ingresa
    caracter = SerialBT.read();   //lee un caracter que recibe
     switch(caracter) {  //compara el caracter recibido con cada letra en cada case
      case 'A':
      //Adelante: COLOR IDENTIFICATIVO 'VERDE'
        direccionesManual(255,0,255,0);
        encendidoLedManual(0,0,255);
        Serial.println("AdelanteM");
        break;
      case 'D' :
      //Derecha: COLOR IDENTIFICATIVO 'MAGENTA'
        direccionesManual(255,0,0,0);
        encendidoLedManual(255,255,0);
        Serial.println("DerechaM");
        break;
      case 'I' :
      //Izquierda: COLOR IDENTIFICATIVO 'AMARILLO'
         direccionesManual(0,0,255,0);
         encendidoLedManual(255,0,255);
         Serial.println("IzquierdaM");
         break;
      case 'R' :
      //Atras: COLOR IDENTIFICATIVO 'AZUL'
         direccionesManual(0,255,0,255);
         encendidoLedManual(0,255,0);
         Serial.println("AtrasM");
         break;
      case 'P' :
      //Parar: COLOR IDENTIFICATIVO 'ROJO'
         direccionesManual(0,0,0,0);
         encendidoLedManual(255,0,0);
         Serial.println("PararM");
         break;
      default :
         printf("oK");
   }
  }

}
