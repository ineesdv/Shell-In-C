#include <stdio.h>
#include <stdlib.h>
#include "simplelinkedlist.h"
#include "TElemento.h"




void insertar  (TElemento e, TLinkedList* l){
    //Tnodo* aux = (Tnodo*) malloc(1*sizeof(Tnodo));
    TLinkedList aux = (TLinkedList) malloc(1*sizeof(Tnodo));
    aux->sig=*l;
    asignar(&(aux->info),e);
    *l=aux;
}
             
void mostrar (TLinkedList* l){
    TLinkedList aux = *l;
    while (aux!=NULL){
        print(aux->info);
        aux=aux->sig;
    }
    printf("\n");
}
void crearVacia (TLinkedList* l){
    *l=NULL;
}

void insertarFinal (TElemento e, TLinkedList* a){
    if(esVacia(a)){
        insertar(e,a);
    }else{
        TLinkedList aux = *a;
        while ((aux->sig)!=NULL){
            aux=aux->sig;
        }

        TLinkedList auxN = (TLinkedList) malloc(1*sizeof(Tnodo));
        auxN->sig=NULL;
        asignar(&(auxN->info),e);
        aux->sig=auxN;

    }

}
int eliminar (TElemento e, TLinkedList* a){
    TLinkedList act = *a;
    TLinkedList ant= NULL;
    int encontrado =0;
    while ((act!=NULL)&&(!encontrado)){
        encontrado=igual(act->info,e);
        if (!encontrado) {
            ant=act;
            act=act->sig;
        }
    }

    if (encontrado){
        if (ant==NULL){
            (*a)=(*a)->sig;//por si es el primero
        }else{
            ant->sig=act->sig;
        }
        free(act);
    }
    return encontrado;
}

void primero (TLinkedList* a, TElemento* e){
    if(!esVacia(a)){
        asignar(e,(*a)->info);
    }

}
void resto (TLinkedList* a){
    if(!esVacia(a)){
        TLinkedList aux = *a;
        *a=(*a)->sig;
        free(aux);
    }
}
void ultimo (TLinkedList* a, TElemento* e){
    if(!esVacia(a)){
        TLinkedList aux = *a;
        while ((aux->sig)!=NULL){
            aux=aux->sig;
        }
       asignar(e,aux->info);

    }
}
void concatenar(TLinkedList* a1, TLinkedList* a2){
    TLinkedList aux = *a2;
    while ((aux)!=NULL){
        insertarFinal(aux->info,a1);
        aux=aux->sig;
    }
 }
int contiene (TElemento e, TLinkedList* a){
     TLinkedList act = *a;

        int encontrado =0;
        while ((act!=NULL)&&(!encontrado)){
            encontrado=igual(act->info,e);
            if (!encontrado){
                act=act->sig;
            }
        }
        return encontrado;
}

int contienePorID (int id, TLinkedList* a){
     TLinkedList act = *a;

        int encontrado =0;
        while ((act!=NULL)&&(!encontrado)){
            encontrado=igualID(act->info, id);
            if (!encontrado){
                act=act->sig;
            }
        }
        return encontrado;
}

void mostrarPorID (int id, TLinkedList* a){
     TLinkedList act = *a;

        int encontrado =0;
        while ((act!=NULL)&&(!encontrado)){
            encontrado=igualID(act->info,id);
            if (!encontrado){
                act=act->sig;
            }else{
                print(act->info);
            }
        }

        
}

int devolverPID (int id, TLinkedList* a){
     TLinkedList act = *a;

        int encontrado =0;
        while ((act!=NULL)&&(!encontrado)){
            encontrado=igualID(act->info,id);
            if (!encontrado){
                act=act->sig;
            }else{
                return(act->info.pid);
            }
        }

        
}

void cambiarStatus(int id, TLinkedList* a, char status){
    TLinkedList act = *a;

        int encontrado =0;
        while ((act!=NULL)&&(!encontrado)){
            encontrado=igualID(act->info, id);
            if (!encontrado){
                act=act->sig;
            }else{
                act->info.status = status;
            }
        }
        
}




int esVacia (TLinkedList* a){
    return (*a)==NULL;
}
int longitud (TLinkedList* a){
    TLinkedList aux = *a;
    int longitud =0;
        while ((aux)!=NULL){
            longitud ++;
            aux=aux->sig;
        }
    return longitud;
}




void copiar(TLinkedList* original, TLinkedList* copia){
    TLinkedList pUltimo = NULL;
    crearVacia(copia);
    TLinkedList paux = *original;
    while (paux!=NULL) {
        TLinkedList nuevo = (TLinkedList) malloc(1*sizeof(Tnodo));
        asignar(&(nuevo->info),paux->info);
        nuevo->sig=NULL;

        if (pUltimo==NULL) {
            *copia = nuevo;
        }else {
            pUltimo->sig=nuevo;
        }
        pUltimo=nuevo;
        paux = paux->sig;
    }
}



/*
void copiar(TLinkedList* original, TLinkedList* copia){
    crearVacia(copia);
    TNodo* paux = *original;
    while (paux!=NULL) {
        insertarFinal (paux->info, copia);
        paux = paux->sig;
    }
}
*/


void destruir(TLinkedList* a){
    TLinkedList ant= NULL;
    TLinkedList act= *a;
    while (act!=NULL){
        ant=act;
        act=act->sig;
        free (ant);
    }
    free (ant);
    *a=NULL;

}
