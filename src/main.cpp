
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
 
//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT
const byte _74HC595_14 = 2; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte _74HC595_12 = 3; // LATCHPIN-- Seguro del pulso de salida. Conecta el pin 3 de Arduino con el pin 12 del 74HC595
const byte _74HC595_11 = 4; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

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

byte input_A = B00000000;
/*
******************************************** Funciones ********************************************
*/


void readState( ){

  digitalWrite (_74HC165_01, LOW); 
  digitalWrite (_74HC165_01, HIGH);

  SPI.setBitOrder(LSBFIRST);

  /*for(int i = 0; i<rows; i++){
    
    input[i] = SPI.transfer(0);

  }
  */
    input_A = SPI.transfer(0);
    byte state;
    for(byte i = 0; i<rows; i++){
      state = bitRead(input_A,i); 
      Serial.print((String)"Estado pin " +  i + " : " );
      Serial.println(state);
    }
   Serial.println("Arreglo: 1");
  //return state;

}

void writeState(){
  
   shiftOut(_74HC595_14, _74HC595_11, LSBFIRST, input_A);
   digitalWrite(_74HC595_12, LOW);
   digitalWrite(_74HC595_12, HIGH);
   
}



/*
******************************************** Funciones ejecucion de Arduino ********************************************
*/

//funcion de inicializacion general Arduino - Hardware y software

void setup ()
{
  SPI.begin ();
  Serial.begin (115200);
  Serial.println ("Begin switch test.");

  //inicializacion pines chip lectura 
  pinMode (_74HC165_01, OUTPUT);
  digitalWrite (_74HC165_01, HIGH);

  // inicializacion pines chip escritura
  pinMode(_74HC595_12, OUTPUT);
  pinMode(_74HC595_11, OUTPUT);
  pinMode(_74HC595_14, OUTPUT);
}
 
//funcion ciclica Arduino 

void loop ()
{
  readState();
  writeState();

} 
