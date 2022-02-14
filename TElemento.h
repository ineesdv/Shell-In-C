#ifndef elemento_h
#define elemento_h
#include <stdio.h>
#include <stdlib.h>

typedef struct TElemento{
	pid_t pid;
	int id;
	char status; // Background or Foreground (B/F)
	char command[1024]; // Comando
} TElemento;

void print(TElemento t);
int igual(TElemento e1, TElemento e2);
void asignar(TElemento *e1, TElemento e2);
int igualID(TElemento e1, int id);

#endif
