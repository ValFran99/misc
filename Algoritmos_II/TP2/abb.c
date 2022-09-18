#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500

#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "pila.h"
#include <stdbool.h>

typedef struct nodo{
	struct nodo* izq;
	struct nodo* der;
	char* clave;
	void* valor;
} nodo_t;

struct abb{
	nodo_t* raiz;
	size_t cantidad;
	abb_destruir_dato_t destruir;
	abb_comparar_clave_t comparar;

};

struct abb_iter{
    struct nodo* raiz;
    struct pila* pila;
    char* min;
    char* max;
    abb_comparar_clave_t cmp;
};


/* ******************************************************************
 *                    	FUNCIONES NODO
 * *****************************************************************/

nodo_t* nodo_crear(char* clave, void* dato){

	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL) return NULL;

	nodo->clave = clave;
	nodo->valor = dato;
	nodo->izq = NULL;
	nodo->der = NULL;

	return nodo;
}

void* nodo_borrar(nodo_t* nodo){

	void* valor = nodo->valor;
	free(nodo->clave);
	free(nodo);
	return valor;
}

void nodo_destruir(abb_t* arbol, nodo_t* nodo){

	free(nodo->clave);
	if(arbol->destruir) arbol->destruir(nodo->valor);
	
	free(nodo);
}

/* ******************************************************************
 *                    	FUNCIONES AUXILIARES
 * *****************************************************************/

/* Busca en el arbol el nodo con la clave buscada y su padre
Devuelve el hijo "normalmente" y al padre por parámetro, padre es NULL en caso de no existir.
Pre: el abb fue creado
*/
nodo_t* buscar_padre_hijo(const abb_t* arbol, nodo_t* nodo, nodo_t** padre, const char* clave){
	
	if(!nodo) return NULL;
	
	if(arbol->comparar(clave, nodo->clave) == 0) return nodo;

	if(arbol->comparar(clave, nodo->clave) > 0){
		*padre = nodo;
		return buscar_padre_hijo(arbol, nodo->der, padre, clave);
	} else{
		*padre = nodo;
		return buscar_padre_hijo(arbol, nodo->izq, padre, clave);
	}
}

/* Busca el nodo con la clave más chica entre las más grandes y lo devuelve */
nodo_t* buscar_reemplazante(nodo_t* nodo){

	if(nodo->izq == NULL) return nodo;
	return buscar_reemplazante(nodo->izq);
}



/* ******************************************************************
 *                    	PRIMITIVAS ARBOL
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

	abb_t* abb = malloc(sizeof(abb_t));
	if(abb == NULL) return NULL;

	abb->raiz = NULL;
	abb->cantidad = 0;
	abb->destruir = destruir_dato;
	abb->comparar = cmp;

	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){

	nodo_t* padre = NULL;
	nodo_t* hijo = buscar_padre_hijo(arbol, arbol->raiz, &padre, clave);


	// Si hijo no es NULL quiere decir que ya existe ese nodo, y solo tengo que destruir el valor
	// viejo y pisarle el nuevo
	if(hijo != NULL){
		if(arbol->destruir) arbol->destruir(hijo->valor);
		hijo->valor = dato;
		return true;
	}
	char* clave_copia = strdup(clave);
	if(clave_copia == NULL) return false;

	nodo_t* nuevo_nodo = nodo_crear(clave_copia, dato);
	if(nuevo_nodo == NULL){
		free(clave_copia);
		return false;
	}
	arbol->cantidad++;
	// Si el padre sigue siendo NULL es porque estoy en el caso de abb vacío, solo cambio la referencia a la raíz
	if(padre == NULL){
		arbol->raiz = nuevo_nodo;
		return true;
	}

	if(arbol->comparar(clave, padre->clave) > 0){
		padre->der = nuevo_nodo;
	} else{
		padre->izq = nuevo_nodo;
	}
	return true;
}

void* abb_caso_hoja(abb_t* arbol, nodo_t* hijo, nodo_t* padre){

	// Caso último valor, raíz sin hijos
	if(padre == NULL){
		arbol->raiz = NULL;
		arbol->cantidad--;
		return nodo_borrar(hijo);

	} else if(arbol->comparar(hijo->clave, padre->clave) > 0){
		padre->der = NULL;

	} else if(arbol->comparar(hijo->clave, padre->clave) < 0){
		padre->izq = NULL;
	}
	arbol->cantidad--;
	return nodo_borrar(hijo);
}

void* abb_caso_hijo_unico(abb_t* arbol, nodo_t* hijo, nodo_t* padre){

	nodo_t* nieto_unico;
	if(!hijo->izq){
		nieto_unico = hijo->der;
	} else{
		nieto_unico = hijo->izq;
	}
	// Caso raíz con un solo hijo
	if(padre == NULL){
		arbol->raiz = nieto_unico;
		arbol->cantidad--;
		return nodo_borrar(hijo);
	}
	if(arbol->comparar(hijo->clave, padre->clave) > 0){
		padre->der = nieto_unico;

	} else if(arbol->comparar(hijo->clave, padre->clave) < 0){
		padre->izq = nieto_unico;
	}
	arbol->cantidad--;
	return nodo_borrar(hijo);
}

void* abb_caso_dos_hijos(abb_t* arbol, nodo_t* hijo){
	
	nodo_t* reemplazante = buscar_reemplazante(hijo->der);
	char* clave_reemplazo = strdup(reemplazante->clave);
	void* valor_reemplazo = abb_borrar(arbol, clave_reemplazo);

	free(hijo->clave);
	hijo->clave = clave_reemplazo;
	void* valor = hijo->valor;
	hijo->valor = valor_reemplazo;

	return valor;
}

void *abb_borrar(abb_t *arbol, const char *clave){

	nodo_t* padre = NULL;
	nodo_t* hijo = buscar_padre_hijo(arbol, arbol->raiz, &padre, clave);
	if(hijo == NULL) return NULL;

	// Caso hoja
	if(hijo->izq == NULL && hijo->der == NULL){
		return abb_caso_hoja(arbol, hijo, padre);
	}

	// Caso tengo solo un hijo
	else if((hijo->izq != NULL && hijo->der == NULL) || (hijo->izq == NULL && hijo->der != NULL)){
		return abb_caso_hijo_unico(arbol, hijo, padre);
	}

	// Caso tengo dos hijos
	return abb_caso_dos_hijos(arbol, hijo);
}

void *abb_obtener(const abb_t *arbol, const char *clave){

	nodo_t* padre = NULL;
	nodo_t* buscado = buscar_padre_hijo(arbol, arbol->raiz, &padre, clave);
	if(buscado == NULL) return NULL;

	return buscado->valor;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){

	nodo_t* padre = NULL;
	nodo_t* buscado = buscar_padre_hijo(arbol, arbol->raiz, &padre, clave);

	return buscado != NULL;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

void _abb_destruir(abb_t *arbol, nodo_t* nodo){

	if(nodo == NULL) return;

	_abb_destruir(arbol, nodo->izq);
	_abb_destruir(arbol, nodo->der);
	nodo_destruir(arbol, nodo);
}

void abb_destruir(abb_t *arbol){

	_abb_destruir(arbol, arbol->raiz);
	free(arbol);
}


/* ******************************************************************
 *                    		ITERADOR INTERNO
 * *****************************************************************/

bool wrapper_abb_in_order(nodo_t *nodo, bool visitar(const char *, void*, void*), void *extra){

    if(nodo == NULL) return true; 

    if(wrapper_abb_in_order(nodo->izq, visitar, extra)){
		
		if(!visitar(nodo->clave, nodo->valor, extra)){
			return false;
		}
		if(!wrapper_abb_in_order(nodo->der, visitar, extra)){
			return false;
		}
		return true;
	}
	return false;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    wrapper_abb_in_order(arbol->raiz, visitar, extra);
}

/* ******************************************************************
 *                    		ITERADOR EXTERNO
 * *****************************************************************/

nodo_t* buscar_siguiente(nodo_t* actual, pila_t* pila, char* min, char* max, int (cmp)(const char*, const char*)){

	if(actual == NULL || (actual->izq == NULL && actual->der == NULL)){
		return actual;
	}

	if(cmp(actual->clave, max) <= 0){
		pila_apilar(pila, actual);
		return buscar_siguiente(actual->izq, pila, min, max, cmp);
	}

	return buscar_siguiente(actual->izq, pila, min, max, cmp);

}

nodo_t* buscar_minimo(nodo_t* actual, pila_t* pila, char* min, char* max, int (cmp) (const char*, const char*)){

	if((actual->izq == NULL && actual->der == NULL) || cmp(actual->clave,min) == 0){
		return actual;
	}

	bool en_rango;
	if(max == NULL){
		en_rango = cmp(actual->clave, min) >= 0;
	} else{
		en_rango = cmp(actual->clave, min) >= 0 && cmp(actual->clave, max) <= 0;
	}

	if(en_rango){
		if(actual->izq == NULL)
			return actual;
		pila_apilar(pila, actual);
		return buscar_minimo(actual->izq, pila, min, max, cmp);
	}
	// Caso fuera de rango por abajo
	else if(cmp(actual->clave, min) < 0){
		return buscar_minimo(actual->der, pila, min, max, cmp);
	}
	// Caso fuera de rango por arriba
	if(actual->izq == NULL)
		return actual;
	return buscar_minimo(actual->izq, pila, min, max, cmp);

}

nodo_t *recorrer_hijos_izq(nodo_t *actual, pila_t* pila){

	if(actual->izq == NULL){
		return actual;
	}
	pila_apilar(pila, actual->izq);
	return recorrer_hijos_izq(actual->izq, pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol, char* min, char* max){
	
    abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));
    if(abb_iter == NULL) return NULL;

    pila_t* pila = pila_crear();
    if(pila == NULL){
        free(abb_iter);
        return NULL;
    }
    abb_iter->raiz = arbol->raiz; 

    if(arbol->raiz != NULL){
    	if(min == NULL){
    		if(max == NULL || arbol->comparar(arbol->raiz->clave, max) <= 0){
			    pila_apilar(pila, arbol->raiz);
				abb_iter->raiz = recorrer_hijos_izq(abb_iter->raiz, pila);
    		} else{
    			// Acá debería buscar los siguientes válidos
    			abb_iter->raiz = buscar_siguiente(abb_iter->raiz, pila, min, max, arbol->comparar);
    			if(arbol->comparar(abb_iter->raiz->clave, max) <= 0)
    				pila_apilar(pila, abb_iter->raiz);
    		}
    	} else{
    		abb_iter->raiz = buscar_minimo(abb_iter->raiz, pila, min, max, arbol->comparar);
    		if(max == NULL){
    			if(arbol->comparar(abb_iter->raiz->clave, min) >= 0)
    				pila_apilar(pila, abb_iter->raiz);
    			
    		} else{
    			if(arbol->comparar(abb_iter->raiz->clave, min) >= 0 && arbol->comparar(abb_iter->raiz->clave, max) <= 0)
    				pila_apilar(pila, abb_iter->raiz);
    		}
    	}
    }
    
    abb_iter->pila = pila;
    abb_iter->cmp = arbol->comparar;
    abb_iter->min = min;
    abb_iter->max = max;

    return abb_iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

    if(abb_iter_in_al_final(iter)){
        return false; 
    }
    pila_desapilar(iter->pila); 

    if(iter->raiz->der){
    	if(iter->max == NULL){
			pila_apilar(iter->pila, iter->raiz->der);
	        iter->raiz = iter->raiz->der;
			iter->raiz = recorrer_hijos_izq(iter->raiz, iter->pila);

    	} else if(iter->cmp(iter->raiz->der->clave, iter->max) <= 0){
    		pila_apilar(iter->pila, iter->raiz->der);
    		iter->raiz = iter->raiz->der;
    		iter->raiz = recorrer_hijos_izq(iter->raiz, iter->pila);

    	} else{
    		// Acá debería buscar los siguientes que estén dentro del rango y apilarlos
    		iter->raiz = iter->raiz->der;
    		iter->raiz = buscar_siguiente(iter->raiz, iter->pila, iter->min, iter->max, iter->cmp);
    		if(iter->raiz == NULL){
    			return true;
    		}
    		if(iter->cmp(iter->raiz->clave, iter->max) <= 0){
    			pila_apilar(iter->pila, iter->raiz);
    		}

    	}
    	return true;
    }	
    iter->raiz = pila_ver_tope(iter->pila);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){

    if(abb_iter_in_al_final(iter)){
        return NULL; 
    }
	nodo_t* nodo = pila_ver_tope(iter->pila);
    return nodo->clave;
}


bool abb_iter_in_al_final(const abb_iter_t *iter){

    if(pila_esta_vacia(iter->pila)){
        return true;
    }
    return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}
