#include "cola.h"
#include <stdlib.h>

typedef struct nodo{

	void* dato;
	struct nodo* prox;

} nodo_t;

nodo_t* crear_nodo_cola(void* valor){

	nodo_t* nodo = malloc(sizeof(nodo_t));

	if(!nodo){
		return NULL;
	}

	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

struct cola{

	nodo_t* prim;
	nodo_t* ult;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void){

	cola_t* cola = malloc(sizeof(cola_t));

	if(!cola){
		return NULL;
	}
	cola->prim = NULL;
	cola->ult = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

	while(!cola_esta_vacia(cola)){
		void* dato = cola_desencolar(cola);
		if(destruir_dato){
			destruir_dato(dato);
		}
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->prim == NULL && cola->ult == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){

	nodo_t *nuevo_nodo = crear_nodo_cola(valor);
	if(!nuevo_nodo){
		return false;
	}

	if(!cola->prim){
		cola->prim = nuevo_nodo;
	} else{
		cola->ult->prox = nuevo_nodo;
	}
	cola->ult = nuevo_nodo;
	return true;
}

void *cola_ver_primero(const cola_t *cola){
	if(cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->prim->dato;
}

void *cola_desencolar(cola_t *cola){
	if(cola_esta_vacia(cola)){
		return NULL;
	}
	nodo_t* aux = cola->prim;
	cola->prim = cola->prim->prox;
	if(!cola->prim){
		cola->ult = NULL;
	}
	void* valor = aux->dato;
	free(aux);
	return valor;
}