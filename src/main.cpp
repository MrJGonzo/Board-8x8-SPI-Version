
/* version 4.x del tablero de LEDS 8x8 que incorpora:
* 8 circuitos de desplazamiento 74HC595 para escritura
* 8 registros de desplazamiento 74LS165 para lectura
* Utilizacion del protocolo SPI 
* @autor: Jose Antonio Gonzales jzgonzales@gmail.com
* @date: julio 2020
*/

/*
************************************************** Librerias ************************************************
*/
#include <Arduino.h>
#include <SPI.h>

/*
******************************************** Conexiones hardware ********************************************
*/
//Asignacion de pines del 74LS165 que controla la funcion general de lectura - DATA INPUT
const byte _74HC165_01 = 7;  // LATCH-- Entrada carga en paralelo. Conecta el pin 1 del 74LS165 al pin 7 de Arduino

/* Con la utilizacion del protocolo SPI las conexiones de a los pines 50 y 52 del Ardunio Mega 2560 no se definen ya que estan predefinidas por el hardware. 
* SCK SPI Bus. Conecta el pin 2 del 74LS165 al pin 52 de Arduino Mega 2560 Protocolo SPI
* MISO SPI Bus. Conecta el pin 7 del 74LS165 al pin 50 de Arduino Mega 2560 Protocolo SPI
* La conexion en serie de multiples 74LS165 se hace del pin 10 del primer Chip al pin 9 del siguiente Chip sucesivamente.
*/
 
//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 0
const byte A_74HC595_14 = 2; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte A_74HC595_12 = 3; // LATCHPIN-- Seguro del pulso de salida. Conecta el pin 3 de Arduino con el pin 12 del 74HC595
const byte A_74HC595_11 = 4; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 1
const byte B_74HC595_14 = 8; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte B_74HC595_12 = 9; // LATCHPIN-- Seguro del pulso de salida. Conecta el pin 3 de Arduino con el pin 12 del 74HC595
const byte B_74HC595_11 = 10; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

/*
******************************************** Variables globales ********************************************
*/

const int rows = 8; //Tamano de cada fila.
#define t 1000 

byte input [rows] = { B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000 };

byte output [rows] = {B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000 };

byte state [rows] = { B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000,
                      B00000000 };

byte input_A;
/*
******************************************** Funciones ********************************************
*/


void readState( ){

  digitalWrite (_74HC165_01, LOW); 
  digitalWrite (_74HC165_01, HIGH);

  SPI.setBitOrder(LSBFIRST);

  for(byte i = 0; i < 8; i++){

    input[i] = SPI.transfer(10);
    Serial.println("SPI.transfer : ");
    input_A = input[i];
    Serial.println(input_A);
    Serial.print(" INPUT ARRAY 0:");
    Serial.println(byte(input[0]));
    Serial.print(" INPUT ARRAY 1:");
    Serial.println(input[1]);
    Serial.print(" INPUT ARRAY 2:");
    Serial.println(input[2]);
    Serial.print(" INPUT ARRAY 3:");
    Serial.println(input[3]);
    Serial.print(" INPUT ARRAY 4:");
    Serial.println(input[4]);
    Serial.print(" INPUT ARRAY 5:");
    Serial.println(input[5]);
    Serial.print(" INPUT ARRAY 6:");
    Serial.println(input[6]);                    
    Serial.print(" INPUT ARRAY 7:");
    Serial.println(input[7]);

  }
    
  byte state;
    
  for(byte i = 0; i<rows; i++){

      state = bitRead(input_A,i); 
      Serial.print((String)"Estado pin " +  input[i] + " : " );
      Serial.println(input[i]);

  }
   
   shiftOut(A_74HC595_14, A_74HC595_11, LSBFIRST, input[0]);
   digitalWrite(A_74HC595_12, LOW);
   digitalWrite(A_74HC595_12, HIGH);
   Serial.print("FILA A : " );
   Serial.println(input[0]);

   shiftOut(B_74HC595_14, B_74HC595_11, LSBFIRST, input[1]);
   digitalWrite(B_74HC595_12, LOW);
   digitalWrite(B_74HC595_12, HIGH);
   Serial.print("FILA B : " );
   Serial.println(input[1]);

}

void writeState(){
  
   shiftOut(A_74HC595_14, A_74HC595_11, LSBFIRST, input_A);
   digitalWrite(A_74HC595_12, LOW);
   digitalWrite(A_74HC595_12, HIGH);
   
}



/*
******************************************** Funciones ejecucion de Arduino ********************************************
*/

//funcion de inicializacion general Arduino - Hardware y software

void setup ()
{
  SPI.begin ();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  Serial.begin (115200);
  Serial.println ("Begin switch test.");

  //inicializacion pines chip lectura 
  pinMode (_74HC165_01, OUTPUT);
  digitalWrite (_74HC165_01, HIGH);

  // inicializacion pines chip A escritura
  pinMode(A_74HC595_12, OUTPUT);
  pinMode(A_74HC595_11, OUTPUT);
  pinMode(A_74HC595_14, OUTPUT);
  // inicializacion pines chip B escritura
  pinMode(B_74HC595_12, OUTPUT);
  pinMode(B_74HC595_11, OUTPUT);
  pinMode(B_74HC595_14, OUTPUT);
}
 
//funcion ciclica Arduino 

void loop ()
{
  readState();
  //writeState();

} 