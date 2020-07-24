
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
 
/*  
* Asignacion de pines del 74HC595 que controla la funcion general de escritura
*/

const byte LATCH_74HC595_12 = 3; // LATCHPIN-- Seguro del pulso de salida. Conecta el pin 3 de Arduino con el pin 12 del 74HC595. Se comparte en paralelo para todos los 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 0
const byte A_74HC595_14 = 2; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte A_74HC595_11 = 4; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 1
const byte B_74HC595_14 = 8; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte B_74HC595_11 = 10; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 2
const byte C_74HC595_14 = 2; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte C_74HC595_11 = 4; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 3
const byte D_74HC595_14 = 8; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte D_74HC595_11 = 10; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 4
const byte E_74HC595_14 = 2; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte E_74HC595_11 = 4; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 5
const byte F_74HC595_14 = 8; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte F_74HC595_11 = 10; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 6
const byte G_74HC595_14 = 2; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte G_74HC595_11 = 4; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

//Asignacion de pines del 74HC595 que controla la funcion general de escritura - DATA OUTPUT FILA 7
const byte H_74HC595_14 = 8; // DATAPIN-- Salida serial. Conecta el pin 2 de Arduino con el pin 14 del 74HC595
const byte H_74HC595_11 = 10; // CLOCKPIN-- Pulso del reloj. Conecta el pin 4 del Arduino con el pin 11 del 74HC595

/*
******************************************** Variables globales ********************************************
*/

const int pixels = 8; // Cantidad de pixels por fila

#define t 1000 

// Almacenamiento de los estados de los chips SPI de sensores-entradas
byte inputState [pixels] = {  B00000000,
                              B00000000,
                              B00000000,
                              B00000000,
                              B00000000,
                              B00000000,
                              B00000000,
                              B00000000 };

byte output [pixels] = {B00000000,
                        B00000000,
                        B00000000,
                        B00000000,
                        B00000000,
                        B00000000,
                        B00000000,
                        B00000000 };

bool state [pixels] = { 0,0,0,0,0,0,0,0 };

// Arreglo de para los pines del pulso de Reloj de cada chip 74HC595
byte clockPin [pixels] = {  A_74HC595_11, 
                            B_74HC595_11, 
                            C_74HC595_11, 
                            D_74HC595_11, 
                            E_74HC595_11,
                            F_74HC595_11,
                            G_74HC595_11  };

// Arreglo de para los pines del pulso de datos de cada chip 74HC595
byte dataPin [pixels] = {   A_74HC595_14, 
                            B_74HC595_14, 
                            C_74HC595_14, 
                            D_74HC595_14, 
                            E_74HC595_14,
                            F_74HC595_14,
                            G_74HC595_14  };
byte input_A;
String bitValue;

/*
******************************************** Funciones de prueba ********************************************
*/



// Funcion general de escritura de estado - Solo para pruebas - 
void writeState(){
  
   shiftOut(A_74HC595_14, A_74HC595_11, LSBFIRST, input_A);
   digitalWrite(LATCH_74HC595_12, LOW);
   digitalWrite(LATCH_74HC595_12, HIGH);
   
}

/*
******************************************** Funciones revisadas ********************************************
*/

// Lee y almacena el estado de cada valor entregado por los chips SPI a cada una las filas en el arreglo inputState
void readStateSPI(){

  for(int i = 0; i < pixels; i++){
    
    // Se lee el estado del bus SPI y se almacena en cada posicion del arreglo
    inputState[i] = SPI.transfer(8);
    // Imprime el valor decimal de la fila - Se puede usar para la etapa analitica de patrones
    Serial.println((String)"Bus SPI buffer " + i + " valor decimal :" + inputState[i] );  // Se monitorea el estado de asignacion de cada fila
    Serial.println((String)"Bus SPI buffer " + i + " valor hexadecimal :" + (inputState[i], HEX) );  // Se monitorea el estado de asignacion de cada fila
  }   
    Serial.println(" ");
}

// Lee bit por bit el estado de cada pixel por filas y lo almacena en el arreglo state
void readBitState(){

    for(int i = 0; i < pixels; i++){

      Serial.print((String)"Estado bit x bit fila " + i + " : ");
      
        for(int j = 0; j < pixels; j++){

            state[i] = bitRead(inputState[i],j);
            Serial.print((String)state[i]);
        }
      
      Serial.println(" ");

    }
    
}

// Escribe el estado a la fila que se le asigne en la funcion ShiftOut
void ledWrite(){

   digitalWrite(LATCH_74HC595_12, LOW);
   digitalWrite(LATCH_74HC595_12, HIGH);
}


// Funcion de lectura del estado de los sensores-interreptures y escritura del estado a cada fila de LEDs
void readAndWrite( ){

  // Inicializacion del estado de acuerdo con los pulsos requeridos por el chip
  digitalWrite (_74HC165_01, LOW); 
  digitalWrite (_74HC165_01, HIGH);

  SPI.setBitOrder(LSBFIRST);

// Lectura y asignacion del estado del lector-sensor por fila
  readStateSPI();

  // Se asignan los parametros de la funcion ShiftOut a cada puerto de cada chip 74HC595 para la escritura de estado.
  for(int i = 0; i < 2; i++){
    
    shiftOut(dataPin[i], clockPin[i], MSBFIRST, inputState[i]);
    Serial.println((String)"FILA " + i + " Estado :" + inputState[i]);

    ledWrite(); // Se escribe el estado de la fila de LEDs
  
  }


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

  //Pulso Latch en paralelo para el arreglo de chips de escritura
  pinMode(LATCH_74HC595_12, OUTPUT);
  // inicializacion pines chip A escritura
  pinMode(A_74HC595_11, OUTPUT);
  pinMode(A_74HC595_14, OUTPUT);
  // inicializacion pines chip B escritura
  pinMode(B_74HC595_11, OUTPUT);
  pinMode(B_74HC595_14, OUTPUT);
  // Inicializar los pines de los chips C,D,E,F,G,H 74HC595
}
 
//funcion ciclica Arduino 
void loop ()
{
  // Llamado a la funcion de lecto-escritura
  readAndWrite();
  readBitState();

} 