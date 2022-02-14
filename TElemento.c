#include <stdio.h>
#include <stdlib.h>
#include "TElemento.h"

void print(TElemento t){
	printf("[%d]\t%d\t%c\t%s\n", t.id, t.pid, t.status, t.command);
}

int igual(TElemento e1, TElemento e2){
	return (e1.pid == e2.pid);
}

int igualID(TElemento e1, int id){
	return (e1.id == id);
}

void asignar (TElemento *e1, TElemento e2){
	e1->pid = e2.pid;
    e1->status = e2.status;
	strcpy(e1->command, e2.command); 

}
