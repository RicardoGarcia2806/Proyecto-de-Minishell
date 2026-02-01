#include "tokenize.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

/*
Este modulo simplemente contiene la declaracion de la funcion tokenize
*/

/*
Esta funcion, como su nombre lo indica, "tokeniza" la entrada del usuario
usando delimitadores como el espacio, la tabulacion, etc. En pocas palabras, 
separa la cadena original en subcadenas y precisamente lo que retorna es un arreglo 
de apuntadores a los trozos de dichas subcadenas
*/

//este es el numero maximo de tokens permitidos por el minishell
#define MAX_TOKENS 64

char **tokenize(char *line){
  //declaracion de las variables usadas en esta funcion
  char **tokens = malloc(MAX_TOKENS * sizeof(char *));
  char *token;
  char *saveptr;
  int i = 0;

  //retorna null si la linea de tokens esta vacia
  if (!tokens) {
    return NULL;
  }

  //en caso contrario, construye los tokens, en un arreglo con el mismo nombre
  token = strtok_r(line, " \t\n\r", &saveptr); 
  while(token != NULL && i < MAX_TOKENS - 1){
    tokens[i++] = token;
    token = strtok_r(NULL, " \t\n\r", &saveptr);
  }

  //se le asigna null al ultimo elemento de este arreglo de tokens
  tokens[i] = NULL;  
  return tokens;
}

//esta funcion libera la memoria ocupada por el arreglo de tokens

void free_tokens(char **tokens){
  free(tokens);
}
