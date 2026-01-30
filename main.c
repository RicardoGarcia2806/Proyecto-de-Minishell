//Integrantes: 
//Ricardo Garcia, carnet: 20-10274
//Brian Orta, carnet: 21-10447

//aqui se definen las bibliotecas usadas en el proyecto
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Aqui se define la funcion para tokenizar los elementos de la cadena que pasara el usuario 
//esto ayuda a separar el comando de las flags, es especialmente util si quiero utilizar (o no) flags

//defino el numero maximo de tokens que puede descomponer el minishell

#define MAX_TOKENS 100

//tokenize es una funcion que, a grandes rasgos, va a descomponer el argumento pasado por el usuario en subcadenas
// (los llamados tokens)

char **tokenize(char *line){
  //definicion de las variables
  char **tokens = malloc(MAX_TOKENS * sizeof(char *));
  char *token;
  int i = 0;
  
  //aqui se crea el primer token (el comando, basicamente)
  token = strtok(line, "t\n\r");
  while (token != NULL && i < MAX_TOKENS - 1){
    tokens[i] = token; 
    i++;

    //en esta parte se crean los tokens que faltan 
    token = strtok(NULL, "t\n\r");
  }
  //como token final del arreglo de tokens agrego un elemento null
  tokens[i] = NULL; 
  return tokens;
}



//Aqui se define la funcion principal
int main(){
  
  return 0;
}