#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

//aqui se declaran las funciones usadas en este modulo, que son basicamente
//todas las operaciones que puede hacer el cmd

/*
La funcion help() imprime una lista con todos los posibles comandos que puede 
hacer el shell
*/ 

static void help(void){
  printf("%s","Comandos disponibles:\n");
  printf("%s","cd <dir>    Cambia el directorio actual\n");
  printf("%s","exit        Finaliza la ejecución del shell\n");
  printf("%s","help        Muestra las funciones que puede hacer el minishell\n");
  printf("%s","ls [-l]     Lista el contenido del directorio actual\n");
  printf("%s","pwd         Imprime el directorio actual\n");
  printf("%s","stats       Muestra estadísticas del directorio actual\n");
}

/*
cd_dir permite desplazarte entre directorios, con el comando cd <dir>, donde 
<dir> es el directorio al que me quiero mover. Ademas, si no se coloca argumento
arroja un error
*/

static void cd_dir(char **tokens){

  //cd tiene dos argumentos, si falta uno arroja un error
  if (!tokens[1]){
    fprintf(stderr, "error cd: especifique el directorio al que quiere moverse\n");
    return;
  }
  if (chdir(tokens[1]) != 0){
    perror("cd");
  }
}

/*
pwd simplemente retorna la ruta del directorio actual, para ello se hace
una llamada al sistema usando una funcion getcwd de la libreria <unistd.h>. getcwd lo que hace
es, basicamente retorna la ruta del directorio de trabajo actual.
*/

static void pwd(void){
  //creo  un buffer, que es como una memoria temporal, donde planeo guardar
  // la ruta que retorne getcwd
  char buffer[4096];

  //llamo a getcwd

  if (getcwd(buffer, sizeof(buffer)) != NULL) {
    printf("%s\n", buffer);
  } else {
    perror("pwd"); 
  }
}

/*
Aqui se define  la funcion ls y ls -l, donde ls solo retorna todo lo que
haya en el directorio en el que me encuentro. Por otro lado, ls -l retorna 
directorios pero con su fecha de creacion, hora, etc. Para ello usamos una 
biblioteca llamada time.h
*/

//hacemos un breve inciso paara decir que, windows no admite la funcion
//localtime_r(), en su defecto usa localtime_s().
static void print_mtime(const struct stat *st){
  struct tm tm;
  localtime_r(&st->st_mtime, &tm);
  char buf[32];
  strftime(buf, sizeof(buf), "%Y%m%d %H:%M", &tm);
  printf("%s", buf);
}

static void ls(char **tokens){

  //declaracion de variables:
  int formato =0;
  if (tokens[1] && strcmp(tokens[1], "-l") == 0){
    formato = 1;
  }

  //definimos el directorio como DIR, con un apuntador:
  DIR *d = opendir(".");
  if (!d){
    perror("ls");
    return;
  }
  
  //solo para aclarar, strict es un tipo de dato compuesto que es definido 
  //por el usuario.
  struct dirent *entrada;
  while((entrada = readdir(d)) != NULL){
    if(entrada->d_name[0] == '.'){ 
      continue;
    }
    if (!formato){
      printf("%s  ", entrada->d_name); 
    } else {
      struct stat st;
      if (stat(entrada -> d_name, &st) == 0){
        if (S_ISDIR(st.st_mode)){
          printf("%s", "directorio");
        } else {
          printf("%s", "archivo ");
        }
          printf("%lld ", (long long)st.st_size);
          print_mtime(&st);
          printf(" %s\n", entrada->d_name);
      } else {
        printf("??? 0 00000000 00:00 %s\n", entrada->d_name);
      }
    }
  }

  if(!formato){
    printf("\n");
  }

  closedir(d); 
}

/*
la funcion stats() solo cuenta los archivos y directorios en el directorio actual
*/

static void stats(void){
  DIR *d = opendir(".");

  if (!d){
    perror("stats");
    return;
  }

  //estas variables seran las encargadas de almacenar el numero de archivos
  // y directorios existentes en el directorio actual
  int archivos = 0;
  int directorios = 0;

  struct dirent *entrada;

  //el bucle se ejecuta siempre que d no sea NULL, es decir, siempre
  //que "encuentre" una carpeta para contar

  while ((entrada = readdir(d)) !=  NULL){
    if (entrada -> d_name[0] == '.'){
      continue;
    }

    //con S_ISDIR se verifica si el un objeto st es un diretorio, 
    //en cualquier otro caso es un archivo (esto pues, en una computadora 
    // existen solo directorios o archivos)
    struct stat st;
    if (stat(entrada -> d_name, &st) == 0){
      if (S_ISDIR(st.st_mode)){
        directorios++;
      } else {
        archivos++;
      }
    }
  }
  closedir(d);
  printf("%d directorios \n%d archivos%s\n", directorios, archivos, archivos==1?"":""); 
}


/*
exit__ solo sirve para salir del minishell, para ello usamos la funcion
exit() de la libreria stdlib.h, que simplemente finaliza el proceso actual
liberando buffers y devolviendo un codigo de estado al S.O.
*/

static void exit_(void){
  exit(0);
}

//ahora se crea una funcion que maneje todas las operaciones del minishell
//se llamara built_handle, recibiendo un token y ejecutando la funcion adecuada
//se prefirio hacerlo en una funcion aparte y no en el main debido a que se quiere 
//mantener cierta modularidad, asi podemos escalar el codigo mas facilmente, y mantenemos un main
//menos monstruoso y complicado de modificar

int built_handle(char **tokens){
  
  if (!tokens || !tokens[0]){ 
    return 0;
  }

  //aqui se decide que comando usar en funcion del token seleccionado

  //nota: strcmp compara dos cadenas de caracteres y devuelve 0 si las cadenas son iguales
  // y devuelve -1 si las cadenas no son iguales.
  //strcmp es de la biblioteca string
  if(strcmp(tokens[0], "exit") == 0){
    exit_();
  } else if (strcmp(tokens[0], "help") == 0){
    help();
  } else if (strcmp(tokens[0],"cd") == 0){
    cd_dir(tokens);
  } else if (strcmp(tokens[0], "pwd") == 0){
    pwd();
  } else if (strcmp(tokens[0], "ls") == 0){
    ls(tokens);
  } else if (strcmp(tokens[0], "stats") == 0){
    stats();
  } else {

    //es decir, ejecutas un comando que no esta en los comandos
    //disponibles en el minishell
    return 0;
  }
  return 0;

}