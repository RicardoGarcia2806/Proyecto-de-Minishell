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

//declaramos los prototipos de las funciones utilizadas
//esto es necesario para que el compilador entienda que existen estas funciones y las ejecute

char **tokenize(char *line);
void free_tokens(char **tokens); 
static void help(void);
static void cd_dir(char **tokens);
static void pwd(void);
static void print_mtime(const struct stat *st);
static void ls(char **tokens);
static void stats(void);
static void exit_(void);
int built_handle(char **tokens);

//numero maximo de tokens (es para la funcion **tokenize(char *line))
#define MAX_TOKENS 64 

//Funcion principal:

int main(){
  //Aqui se declaran las variables que usara la funcion principal:
  char *line = NULL;
  size_t len = 0;
  ssize_t nread; 

  /*
  A grandes rasgos, el bucle principal del programa lo que va a hacer es preguntarle constantemente al usuario comandos
  hasta que este decida salir del minishell (que es donde se tendria que detener el bucle)
  */

 while (1){
  
  printf("%s", "minishell> ");
  //fflush se usa para forzar la escritura inmediata de los datos en memoria
  fflush(stdout);

  //tomamos el comando del usuario:

  //nota: getline solo funciona en sistemas UNIX/Linux, por lo que este codigo puede no correr
  //si se ejecuta en un ambiente de tipo windows

  nread = getline(&line, &len, stdin);

  //si se realiza una lectura incorrecta, se detiene el bucle
  if (nread == -1){
    printf("\n");
    break;
  }
  

  /*
  Aplicaremos tokenize sobre line, donde tokenize va a modificar line y devolver sus punteros
  */
 char **tokens = tokenize(line);
  if (!tokens) continue;
  if (!tokens[0]){

    //libera la memoria ocupada por el arreglo tokens
    free_tokens(tokens);
    continue;
  }
  
  //Aqui se toman los comandos que puedo realizar:

  //esta variable funciona como una especie de codigo de estado 
  int builtin = 0;

  //defino el arreglo que contiene todas las intrucciones que puedo hacer: 
  const char *comandos[] = {"cd", "help", "pwd", "ls", "stats", "exit", NULL};

  for (int i = 0; comandos[i]; i++){
    if (strcmp(tokens[0], comandos[i]) == 0){
      builtin = 1;
    }
  }

  if (builtin){
    //llamo a la funcion que maneja los comandos y ejecuto el comando en cuestion
    built_handle(tokens);

    //libero la memoria
    free_tokens(tokens);
    continue;
  }

  if (tokens[0] && strcmp(tokens[0], "exit") == 0){
    free_tokens(tokens);
    break;
  }

  free_tokens(tokens);
 }
  free(line);
  return 0;
}

//en esta parte del codigo estan todas las funciones usadas en main:


/*
Esta funcion, como su nombre lo indica, "tokeniza" la entrada del usuario
usando delimitadores como el espacio, la tabulacion, etc. En pocas palabras, 
separa la cadena original en subcadenas y precisamente lo que retorna es un arreglo 
de apuntadores a los trozos de dichas subcadenas
*/
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

  //en caso contrario, construye los tokens, en un arreglo con el mismo nomnre
  token = strtok_r(line, " \t\n\r", &saveptr); 
  while(token != NULL && i < MAX_TOKENS - 1){
    tokens[i++] = token;
    token = strtok_r(NULL, " \t\n\r", &saveptr);
  }
  tokens[i] = NULL;  
  return tokens;
}

//esta funcion libera la memoria ocupada por el arreglo de tokens
void free_tokens(char **tokens){
  free(tokens);
}

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

int built_handle(char **tokens){
  
  if (!tokens || !tokens[0]){ 
    return 0;
  }

  //aqui se decide que comando usar en funcion del token seleccionado

  //nota: strcmp compara dos cadenas de caracteres
  if(strcmp(tokens[0], "exit") == 0){
    exit_();
  } else if (strcmp(tokens[0], "help") == 0){
    help();
  } else if (strcmp(tokens[0],"cd")==0){
    cd_dir(tokens);
  } else if (strcmp(tokens[0], "pwd")==0){
    pwd();
  } else if (strcmp(tokens[0], "ls")==0){
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