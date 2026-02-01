//esto es el nombre del modulo, en este caso le 
//coloque tokenize.h, como a funcion de ese mismo nombre


#ifndef TOKENIZE_H
#define TOKENIZE_H

//aqui declaro el prototipo de las funciones, que en este caso solo es tokenize
//solo para aclarar, un prototipo es una declaracion del encabezado de la funcion
// esto permite que el compilador de c reconozca esos bloques de codigo como funciones

char **tokenize(char *line);
void free_tokens(char **tokens);

#endif