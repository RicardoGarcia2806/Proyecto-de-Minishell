//este define con el codigo 700, es para poder ejecutar funciones propias de los estandares POSIX
#define _XOPEN_SOURCE 700

//Integrantes: 
//Ricardo Garcia, carnet: 20-10274
//Brian Orta, carnet: 21-10447

//aqui se describen las bibliotecas usadas por el programa:
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include "builtins.h"
#include "tokenize.h"

//Funcion principal:

int main(){
  //Aqui se declaran las variables que usara la funcion principal:
  char *line = NULL;
  size_t len = 0;
  ssize_t nread; 

  /*
  A grandes rasgos, el bucle principal del programa lo que va a hacer es preguntarle constantemente al usuario comandos
  hasta que este decida salir del minishell (que es donde se tendria que detener el bucle, usando la funcion exit__, manejada por 
  built_handle
  */

 while (1){
  
  printf("%s", "minishell> ");

  //fflush se usa para forzar la escritura inmediata de los datos en memoria
  fflush(stdout);

  //tomamos el comando del usuario:

  //nota: getline solo funciona en sistemas UNIX/Linux, por lo que este codigo puede no correr
  //si se ejecuta en un ambiente de tipo windows. Hay que tener esto en cuenta. Sin embargo, el 
  //programa se corrio desde la WSL, y funciono perfectamente. 

  nread = getline(&line, &len, stdin);

  //si se realiza una lectura incorrecta, se hace un salto de linea
  if (nread == -1){
    printf("\n");
    break;
  }
  

  /*
  Aplicaremos tokenize sobre line, donde tokenize va a modificar line y devolver el puntero al arreglo de 
  punteros donde guardo los tokens. Por ejemplo: Si escribo "Hola Nestor" o "Hola Guillermo", tokenize devolvera un
  puntero al arreglo {"Hola", "Nestor"} u {"Hola","Guillermo"}
  */
 char **tokens = tokenize(line);

  //si el arreglo de tokens esta vacio (o sea, todos sus elementos son NULL), continuas el programa
  if (!tokens) continue;

  //si el arreglo de tokens esta lleno, u ocupado por otros elementos, entonces se libera ese espacio
  //y luego continuo con el programa. Para ello uso free_tokens, disponible en tokenize.h
  if (!tokens[0]){
    free_tokens(tokens);
    continue;
  }
  
  //Aqui se toman los comandos que puedo realizar:
  //esta variable funciona como una especie de codigo de estado 
  int builtin = 0;

  //defino el arreglo que contiene todas las intrucciones que puedo hacer: 
  const char *comandos[] = {"cd", "help", "pwd", "ls", "stats", "exit", NULL};

  //si en en la primera posicion del arreglo de tokens (o sea, el comando) esta alguno de los 
  //elementos del arreglo comandos, verficando todas las posiciones de este ultimo
  for (int i = 0; comandos[i]; i++){

    //si esta y es uno de los comandos definidos para el minishell, entonces builtin se vuelve 1
    if (strcmp(tokens[0], comandos[i]) == 0){
      builtin = 1;
    }
  }

  if (builtin){
    //llamo a la funcion que maneja los comandos y ejecuto el comando en cuestion
    built_handle(tokens);

    //libero el espacio ocupado por el arreglo tokens durante la instruccion, 
    // o sea, termino el comando, y libero el arreglo tokens
    free_tokens(tokens);
    continue;
  }
  
  //libero el espacio del arreglo tokens, totalmente. 
  free_tokens(tokens);
 }

  //libero el espacio ocupado por el comando introducido por el usuario 
  free(line);
  return 0;
}
