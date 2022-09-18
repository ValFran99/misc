#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                    			NODO
 * *****************************************************************/

typedef struct nodo{

	void* dato;
	struct nodo* prox;

} nodo_t;

nodo_t* crear_nodo(void* valor){

	nodo_t* nodo = malloc(sizeof(nodo_t));

	if(!nodo){
		return NULL;
	}

	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

/* ******************************************************************
 *                    			LISTA
 * *****************************************************************/

struct lista{

	struct nodo* prim;
	struct nodo* ult;
	size_t largo;

};

/* ******************************************************************
 *                    	ITERADOR EXTERNO
 * *****************************************************************/

struct lista_iter{

	struct lista* lista;
	struct nodo* actual;
	struct nodo* anterior;

};

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	
	if(lista == NULL){
		return NULL;
	}

	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return lista->prim == NULL && lista->ult == NULL && lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){

	nodo_t* nuevo_nodo = crear_nodo(dato);
	if(nuevo_nodo == NULL){
		return false;
	}

	if(lista_esta_vacia(lista)){
		lista->ult = nuevo_nodo;
	} else{
		nuevo_nodo->prox = lista->prim;
	}

	lista->prim = nuevo_nodo;
	lista->largo++;

	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){

	nodo_t* nuevo_nodo = crear_nodo(dato);

	if(nuevo_nodo == NULL){
		return false;
	}

	if(lista_esta_vacia(lista)){
		lista->prim = nuevo_nodo;
	} else{
		lista->ult->prox = nuevo_nodo;
	}

	lista->ult = nuevo_nodo;
	lista->largo++;

	return true;
}

void *lista_borrar_primero(lista_t *lista){
	
	if(lista_esta_vacia(lista)){
		return NULL;
	}

	nodo_t* prim_viejo = lista->prim;
	void* valor = prim_viejo->dato;

	if(lista->prim == lista->ult){
		lista->ult = NULL;
	}

	lista->prim = lista->prim->prox;
	free(prim_viejo);
	lista->largo--;

	return valor;
}

void *lista_ver_primero(const lista_t *lista){

	if(lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){

	while(!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if(destruir_dato){
			destruir_dato(dato);
		}
	}
	free(lista);
}

void lista_destruir_csv(lista_t* lista, void (*destruir_dato)(char**)){

	while(!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if(destruir_dato){
			destruir_dato(dato);
		}
	}
	free(lista);
}

/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){

	bool ok = true;
	nodo_t* actual = lista->prim;

	while(actual && ok){
		ok &= visitar(actual->dato, extra);
		actual = actual->prox;
	}
}

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){

	lista_iter_t* iter = malloc(sizeof(lista_iter_t));

	if(iter == NULL){
		return NULL;
	}

	iter->lista = lista;
	iter->actual = lista->prim;
	iter->anterior = NULL;

	return iter;

}

bool lista_iter_avanzar(lista_iter_t *iter){

	if(lista_iter_al_final(iter)){
		return false;
	}

	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	return iter->actual ? iter->actual->dato : NULL;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){

	nodo_t* nuevo_nodo = crear_nodo(dato);

	if(nuevo_nodo == NULL){
		return false;
	}

	nuevo_nodo->prox = iter->actual;

	// Caso lista vacía
	if(lista_esta_vacia(iter->lista)){
		iter->lista->prim = nuevo_nodo;
		iter->lista->ult = nuevo_nodo;
	}

	// Caso iterador al principio
	else if(iter->lista->prim == iter->actual){
		iter->lista->prim = nuevo_nodo;
	}

	// Caso general
	else{
		iter->anterior->prox = nuevo_nodo;

		if(lista_iter_al_final(iter)){
			iter->lista->ult = nuevo_nodo;
		}
	}

	iter->actual = nuevo_nodo;
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){

	if(lista_iter_al_final(iter)){
		return NULL;
	}

	nodo_t* actual_viejo = iter->actual;
	void* valor = actual_viejo->dato;

	iter->actual = iter->actual->prox;
	
	// Caso un solo elemento
	if(iter->lista->prim == iter->lista->ult){
		iter->lista->prim = NULL;
		iter->lista->ult = NULL;
	}

	// Caso primer elemento
	else if(actual_viejo == iter->lista->prim){
		iter->lista->prim = iter->actual;
	}

	// Caso general
	else{
		iter->anterior->prox = iter->actual;
		if(lista_iter_al_final(iter)){
			iter->lista->ult = iter->anterior;
		}
	}

	free(actual_viejo);
	iter->lista->largo--;
	return valor;
}