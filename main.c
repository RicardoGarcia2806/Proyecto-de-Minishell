#include <stdio.h>
//declaracion del prototipo
int max(int num1, int num2);

//declaracion de la funcion
int max(int num1, int num2){
  if (num1 >= num2){
    return num1;
  } else {
    return num2;
  }
};

int main(){
  // declaracion de variables
  int number;
  int pair_counting;
  int even_counting;
  int pasos;
  int max_value;
  int new_number;

  // Para empezar, se solicita al usuario un entero positivo
  printf("%s\n", "Por favor, ingrese un entero positivo:");
  scanf("%d", &number);

  //Se requiere verificar que el numero sea mayor que cero, en caso contrario devolveremos un error
  if (number % 2 == 0){
    printf("%s", "Error: El numero no es un entero positivo"); 
  };

  //estos contadores sirven para contar cuantas veces el numero fue par o impar. y para contar el numero de pasos
  pair_counting = 0;
  even_counting = 0;
  pasos = 0;

  // Este bucle ejecuta la conjetura de Collatz y se detiene cuando el numero llega a 1
  while (number != 1){

    //Si el numero es par, se divide entre dos
    if(number % 2 == 0){
      number = number / 2;
      pair_counting++;
      printf("%d\n", number);

      //aqui calculamos el maximo
      //max_value = max(number, new_number);

    } else {
      //si el numero es impar, se multiplica por tres y se le suma 1
      number = 3*number + 1;
      even_counting++;
      printf("%d\n", number);
      //max_value = max(number, new_number);
    }
  }

  //En este bloque de codigo, se imprimen el numero de veces que el numero fue par o impar
  printf("%s", "Cantidad de veces que el numero fue par: ");
  printf("%d\n", pair_counting);

  printf("%s", "Cantidad de veces que el numero fue impar: ");
  printf("%d\n", even_counting);

  //calculo el numero de pasos, que no es mas que la suma de veces en que el numero fue par mas 
  // el numero de veces que el numero fue impar

  pasos = even_counting + pair_counting;
  printf("%s", "Numero de pasos: ");
  printf("%d\n", pasos);

  //valor maximo
  printf("%s", "Valor maximo: ");
  printf("%d\n", max);
  return 0;
};