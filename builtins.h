#ifndef BUILTINS_H
#define BUILTINS_H

//declaro los prototipos de las funciones que voy a usar 
//estas funciones son basicamente todo lo que puede hacer el shell

static void help(void);
static void cd_dir(char **tokens);
static void pwd(void);
static void ls(char **tokens);
static void stats(void);
static void exit_(void);
int built_handle(char **tokens);

#endif