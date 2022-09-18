#include "pila.h"

#include <stdlib.h>
#include <stdio.h>

#define TAM_INICIAL 10
#define REDIMENSION 2
#define MULTIPLICADOR 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

bool redimensionar_pila(pila_t *pila, size_t tam){
	void **datos_nuevo = realloc(pila->datos, tam * sizeof(void*));
	if(datos_nuevo == NULL){
		return false;
	}
	pila->capacidad = tam;
	pila->datos = datos_nuevo;
	return true;
}

pila_t *pila_crear(void){

	pila_t *pila = malloc(sizeof(pila_t));

	if(pila == NULL){
		return NULL;
	}
	pila->cantidad = 0;
	pila->capacidad = TAM_INICIAL;
	pila->datos = malloc(TAM_INICIAL * sizeof(void*));
	
	if(pila->datos == NULL){
		free(pila);
		return NULL;
	}
	return pila;
}

void pila_destruir(pila_t *pila){

	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){

	if(pila->cantidad == pila->capacidad){
		size_t nuevo_tam = (pila->capacidad * REDIMENSION);
		if(!(redimensionar_pila(pila, nuevo_tam))){
			return false;
		}
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;

	return true;
}

void *pila_ver_tope(const pila_t *pila){
	if(pila->cantidad == 0){
		return NULL;
	}
	return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila){
	if(pila->cantidad == 0){
		return NULL;
	}
	void* valor = pila->datos[pila->cantidad - 1];
	pila->cantidad--;

	if(pila->capacidad == TAM_INICIAL){
		return valor;
	}

	if(pila->cantidad * MULTIPLICADOR <=  pila->capacidad){
		size_t nuevo_tam = ((pila->capacidad / MULTIPLICADOR) < TAM_INICIAL) ? TAM_INICIAL:pila->capacidad / REDIMENSION;

		redimensionar_pila(pila, nuevo_tam);
	}
	return valor;
}