#include <Arduino.h>


const int x_axis = 10; // Cantidad de pixels por fila
const int y_axis = 10; // Cantidad de filas de la matriz

#define t 1000 

// Almacenamiento de los estados de los chips SPI de sensores-entradas

int baseState [y_axis] = {  0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000 };

int inputState [y_axis] = { 0b1100000111,
                            0b0000001111,
                            0b0000011111,
                            0b0000111111,
                            0b0001111111,
                            0b0011111111,
                            0b0111111111,
                            0b1111111111,
                            0b0111111111,
                            0b1010111111 };

int outputState [y_axis] = {0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000,
                            0b0000000000 };

char *salida[] {".","","","","","","","","","..."};

char *myStringA[] = { "111100000111", 
                      "0b0000001111", 
                      "0b0000011111", 
                      "0b0000111111", 
                      "0b0011111111", 
                      "0b0111111111", 
                      "0b0111111111", 
                      "0b1010111111", 
                      "0b0000000111",
                      "0b0000001111" };



char *newStrings[y_axis] = {"1000000000","1100000000","1110000000","1111000000","1111100000","1111110000","1111111000","1111111100","1111111110","1111111111"};

char *myStrings[10] = {"This is string 1", "This is string 2", "This is string 3",
                     "This is string 4", "This is string 5", "This is string 6"
                    };

char myString[] = "1111111110000011000010111100101110101011001010110110001100001";



void readStrings()
{
  for (int i = 0; i < y_axis; i++) {
    Serial.println(newStrings[i]);
  }
  Serial.println("");
}




// Lee bit por bit el estado de cada pixel entredado al por filas y lo almacena en el arreglo output. Probado OK
void bitStateReader( int in[], int out[], int x, int y ){

    Serial.println("Lectura bit x bit :");
    
    for(int i = 0; i < y_axis ; i++){

        for(int j = x_axis; j > 0; j--){ // Se invierte para formatear los bits por orden del bit menos significativo - LSBFIRST

            out[i] = bitRead(in[i],j);
            Serial.print(String(out[i]));

        }
    Serial.println(" ");
    }
    Serial.println(" ");
}

// Cuenta la cantidad de repeticiones de cada uno de los elementos de la cadena que recibe. Funciona sobre Strings
void printing(String s) 
{ 
    
    for (int i = 0; s[i] != '\0'; i++) { 

      s.charAt(i);

      int count = 1;
      int key = 1;
      
     // bool val = 0;
      
      while (s.charAt(i) == s.charAt(i+1) ) { 
        i++; 
        count++; 
      }

        Serial.print(s.charAt(i)); 
        Serial.print(" : ");
        Serial.print(count);
        Serial.println("");
      } 
         
  
    Serial.println("");
} 

// Funcion principal para sumar. Se verifica que la operacion inicie en la coordenada de origen (0,0) y que los pixels activados sean consecutivos.
void sum(String s, char key, char zero)
{

  char a = s.charAt(0);
  // Se realiza el inicio de la lectura de actividad en el tablero
  Serial.println("Inicio de sesion");
  
  // Se revisa posible error en la entrada de datos
  if (s.length() == '\0')
  {
    Serial.println("La secuencia de entrada es nula");
  }

  /*
   * Implementar otros escenarios de errores de entrega en la secuencia de datos de entrada.
   * Solamente se deben admitir cadenas binarias de longitud n (10 bits inicialmente).
   */

  // valida si la posicion de la ficha esta esta bien 
  else if (a == zero)
  {
    Serial.println("Coloca tu ficha en la primera posicion");
  }

  // Si la cadena basica de entrada es admisible se inicia el analisis del patron
  else
  {

    Serial.println("");
    for(int i = 0; i < s.length(); i++)
    {
      
      int state_a = 1;  // Se determina como el estado inicial del tablero para la operacion de suma.
      int state_b = 0;  // Se estable el segundo estado, de acuerdo al cambio se deb dar la retroalimentacion

      char a = s.charAt(0);
      // Se valida que la cadena inicie en 1, esto representa que la ficha este en la posicion correcta
      if( a == key )
      {
        while( ( s.charAt(i) == key) && (s.charAt(i+1) == key) )
        {
          state_a++;
          i++;
          
        }
        
        Serial.print(" Tu numero es el: ");
        Serial.print(state_a);
        
        break;

      }

    }
  
  }

}




void counter(String s, char key)
{
  if(s.length() == 0)
  {
    Serial.println("0 mensaje vacio, revisar entradas.");
  }
  else
  {
    for(int i = 0; i < s.length(); i++)
    {
      int ones = 1;
      int count[] = {0};

      while ( (s[i] == s[i+1])  )
      {
        i++;
        ones++;
      }

      count[i] = ones;
        
      Serial.print(s[i]);
      Serial.print(" : ");
      Serial.print(count[i]);
      Serial.println("");
      //return count[i];
    }

    Serial.println(" ");
  }
  
}


/*
// funcion basica suma todos los unos del byte
void sum(int in){

  int b = 0;
  int a = 0;

  for(int i = 0; i < 8; i++){
    a = bitRead(in,i);>
    if(a == 1 && bitRead(in,i+1) == 1 ){
      b = b + 1;
    }
   
  }
  Serial.print("Unos encontrados: ");
  Serial.println(b);
}
*/

void sumRow(int in[], int x, int y){

      for(int i = 0; i < y_axis ; i++){

      int a = 0;
      int b = 0;
      int c = 0;
      int count [y] = {0,0,0,0,0,0,0,0};

        for(int j = x_axis; j > 0; j--){ // Se invierte para formatear los bits por orden del bit menos significativo - LSBFIRST

          switch (bitRead(in[i],j))
          {
          case 0:
            if((in[i],j) == 1 && (in[i],j-1) != 0){
              c++;
              count[i] = c;
            }
            break;

          }


        }
    Serial.print(String(count[i]));
    Serial.println(" ");

    }

    Serial.println(" ");

}


void recibe(int h[])
{
  //int a = 0;
  //a = h.length();
  Serial.println("mensaje recibido es: ");
  for(int i = 0; i < 24; i++)
  {

    Serial.println(h[i]);
  
  }

}

void printBitState(int x_axis, int y_axis){

    String state [x_axis] = { "0","0","0","0","0","0","0","0" };

    for(int i = 0; i < 8 ; i++){

      Serial.print((String)"Bit fila " + i + " : ");
      
        for(int j = 0; j < y_axis; j /**
 // String s = String(n);
  Serial.println("inicio");
  for(int i = 0; i <s.length(); i++ )
  { 
    int count = 1;
    char a = s.charAt(0);

    if( a == c )
    {
      while( ( s.charAt(i) == c) && (s.charAt(i+1) == c) )
      {
        count++;
        i++;
    
      }
      Serial.print(s.charAt(i)); 
      Serial.print(" : ");
      Serial.print(count);
      Serial.println(" ");
    }
    else
    {
      Serial.println("La secuencia no es valida");
    }
  
  }
  **/

  


++){

            state[i] = bitRead(inputState[i],j);
            Serial.print((String)state[i]);
        }

    Serial.println(" ");

    }
    
    Serial.println(" ** ");

}

void setup() {

  Serial.begin(9600);

}

void loop() {

  //bitStateReader(inputState, outputState, x_axis, y_axis);
  //sum(inputState[7]);
  //sumRow(inputState, x_axis, y_axis);
  //sum(*myStringA, '1', '0');
  //printing(*myStrings);
  readStrings();
  
}