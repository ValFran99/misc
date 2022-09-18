#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "lista.h"
#include "hash.h"

#define TAM_INICIAL 7
#define FACT_CARGA 2
#define FACT_ACHICAR 0.3
#define REDIMENSIONAR 2

/* ******************************************************************
 *                    	FUNCION DE HASHING
 * *****************************************************************/

uint32_t one_at_a_time(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

/* Wrapper de la función de hashing */
size_t hashing(const void* clave, size_t cap){

    size_t len = strlen(clave);
    size_t hash = one_at_a_time(clave, len);
    return hash % cap;
}

/* ******************************************************************
 *                    			CAMPO
 * *****************************************************************/

typedef struct hash_campo{

	char* clave;
	void* valor;

} campo_t;

campo_t* crear_campo(char* clave, void* valor){

	campo_t* campo = malloc(sizeof(campo_t));
	if(campo == NULL) return NULL;

	campo->clave = clave;
	campo->valor = valor;

	return campo;
}

/* ******************************************************************
 *                    			HASH
 * *****************************************************************/

struct hash{

	void** tabla;
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t destruir;

};

/* ******************************************************************
 *                    		ITERADOR
 * *****************************************************************/

/* ******************************************************************
 *                    	FUNCIONES AUXILIARES
 * *****************************************************************/

void** crear_tabla(size_t tam){

	void** tabla = malloc(sizeof(void*) * tam);

	if(tabla == NULL){
		return NULL;
	}

	for(int i = 0; i < tam; i++){
		tabla[i] = lista_crear();

		if(tabla[i] == NULL){

			for(int j = 0; j < i; j++){
				lista_destruir(tabla[j], NULL);
			}
			free(tabla);
			return NULL;
		}
	}
	return tabla;
}

void hash_redimensionar(hash_t* hash, size_t tam_nuevo){

	void** tabla_nueva = crear_tabla(tam_nuevo);

	for(int i = 0; i < hash->capacidad; i++){

		while(!lista_esta_vacia(hash->tabla[i])){
			campo_t* campo = lista_borrar_primero(hash->tabla[i]);	
			size_t hashed = hashing(campo->clave, tam_nuevo);
			lista_insertar_primero(tabla_nueva[hashed], campo);
			}

		lista_destruir(hash->tabla[i], NULL);
	}

	free(hash->tabla);
	hash->tabla = tabla_nueva;
	hash->capacidad = tam_nuevo;
}

/* Recibe un hash, una clave y un puntero a un iterador de lista y devuelve el campo correspondiente a esa
clave y también en iterador que se utilizó para llegar a él*/
campo_t* obtener_campo(const hash_t *hash, const char *clave, lista_iter_t** iter_salida){

	size_t hashed = hashing(clave, hash->capacidad);

	if(!lista_esta_vacia(hash->tabla[hashed])){

		lista_iter_t* iter = lista_iter_crear(hash->tabla[hashed]);

		while(!lista_iter_al_final(iter)){
			campo_t* campo = lista_iter_ver_actual(iter);
			
			if(strcmp(clave, campo->clave) == 0){
				*iter_salida = iter;
				return campo;
			}
			lista_iter_avanzar(iter);
		}
		lista_iter_destruir(iter);
	}
	return NULL;
}

/* ******************************************************************
 *                    	PRIMITIVAS DEL HASH
 * *****************************************************************/

/* Crea el hash
 */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato){

	hash_t* hash = malloc(sizeof(hash_t));
	if(hash == NULL) return NULL;

	void** tabla = crear_tabla(TAM_INICIAL);
	if(tabla == NULL){
		free(hash);
		return NULL;
	}

	hash->tabla = tabla;
	hash->destruir = destruir_dato;
	hash->capacidad = TAM_INICIAL;
	hash->cantidad = 0;

	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

	lista_iter_t* iter = NULL;
	campo_t* campo = obtener_campo(hash, clave, &iter);
	lista_iter_destruir(iter);
	if(campo){
		if(hash->destruir) hash->destruir(campo->valor);
		campo->valor = dato;
		return true;
	}

	// Creo un nuevo campo con una copia de la clave y el valor dados
	char* clave_copia = strdup(clave);
	if(clave_copia == NULL) return false;

	float carga = (float) (hash->cantidad + 1) / (float) hash->capacidad;

	// Si llego a necesitar más espacio redimensiono
	if(carga >= FACT_CARGA){
		hash_redimensionar(hash, hash->capacidad*REDIMENSIONAR);
	}

	campo_t* campo_guardar = crear_campo(clave_copia, dato);

	size_t hashed = hashing(clave, hash->capacidad);
	lista_insertar_primero(hash->tabla[hashed], campo_guardar);
	hash->cantidad++;

	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){

	lista_iter_t* iter_borrar = NULL;
	campo_t* campo_borrar = obtener_campo(hash, clave, &iter_borrar);
	if(campo_borrar == NULL){
		lista_iter_destruir(iter_borrar);
		return NULL;
	}

	campo_borrar = lista_iter_borrar(iter_borrar);
	lista_iter_destruir(iter_borrar);

	free(campo_borrar->clave);
	void* aux = campo_borrar->valor;
	free(campo_borrar);
	hash->cantidad--;

	// Chequeo si hace achicar
	float carga = (float) hash->cantidad / (float) hash->capacidad;
	if(carga < FACT_ACHICAR && carga != 0){
		hash_redimensionar(hash, hash->capacidad / REDIMENSIONAR);
	}

	return aux;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	
	lista_iter_t* iter = NULL;
	campo_t* campo = obtener_campo(hash, clave, &iter);
	lista_iter_destruir(iter);
	if(campo){
		return campo->valor;
	}
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){

	lista_iter_t* iter = NULL;
	campo_t* campo = obtener_campo(hash, clave, &iter);
	lista_iter_destruir(iter);
	return campo != NULL;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){

	for(int i = 0; i < hash->capacidad; i++){
		while(!lista_esta_vacia(hash->tabla[i])){
			campo_t* campo = lista_borrar_primero(hash->tabla[i]);
			if(hash->destruir){
				hash->destruir(campo->valor);
			}
			free(campo->clave);
			free(campo);
		}
		lista_destruir(hash->tabla[i], NULL);
	}
	free(hash->tabla);
	free(hash);
}

struct hash_iter{
	const hash_t* hash;
	size_t index;
	campo_t* actual;
	lista_iter_t* lista_iter;
};

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
    if(hash_iter == NULL) return NULL;

    hash_iter->hash = hash; //Mi Hash

	hash_iter->index = 0; // Para saber en que lista estoy, y evitar la funcion de hashing, en caso de que mi campo sea NULL

	while(lista_esta_vacia(hash->tabla[hash_iter->index])){
		if(hash_iter->index + 1 == hash->capacidad){
			break;
		}
		hash_iter->index +=1;
	}

	hash_iter->actual = lista_ver_primero(hash->tabla[hash_iter->index]); //Encontre la primera lista con campo no nulo

	lista_iter_t* iterador = lista_iter_crear(hash->tabla[hash_iter->index]);
	if(iterador == NULL){
		free(hash_iter);
		return NULL;
	}

	hash_iter->lista_iter = iterador; //Mi iterador de lista

    return hash_iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter){ 

    if(hash_iter_al_final(iter)){
        return false;
    }

	if(!lista_iter_al_final(iter->lista_iter)){
		lista_iter_avanzar(iter->lista_iter);

		iter->actual = lista_iter_ver_actual(iter->lista_iter);
		printf("\nDEBUG: Intento leer donde estoy parado\n");
		char* clave = iter->actual->clave;
		printf("\nLa clave guardada es: %s\n", clave);
    	return true;
	}


	//Se que habara mas valores adelante, pero ignoro las listas vacias

	iter->index += 1;

	lista_iter_destruir(iter->lista_iter); //Destruyo mi iterador de lista, ya que necesito uno nuevo para la sig
	 

	while(lista_esta_vacia(iter->hash->tabla[iter->index])){ //Busco la sig lista que no este vacia, si esta vacia, creo igual un iterador en mi ultima lista y mi actual sera NULL
		if(iter->index + 1 == iter->hash->capacidad){
			break;
		}
		iter->index +=1;
	}

	lista_iter_t* iterador = lista_iter_crear(iter->hash->tabla[iter->index]);
	if(iterador == NULL) return NULL;
	iter->lista_iter = iterador;
		
	iter->actual = lista_ver_primero(iter->hash->tabla[iter->index]);

	return true;
	


}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter){
    
    if(hash_iter_al_final(iter)){
		printf("\nDEBUG: Estoy en iter\n");
        return NULL;
    } 
    printf("\nDEBUG: creo que explota al leer la clave\n");
    return iter->actual->clave;
}


bool hash_iter_al_final(const hash_iter_t *iter){

	if(!lista_iter_al_final(iter->lista_iter)){
		return false;
	}

	if(iter->index+1 >= iter->hash->capacidad && lista_iter_al_final(iter->lista_iter)){ //Si mi iterador esta al final de la lista, y mi hashed(siendo la lista en la que estoy) +1 es igual a la capacidad, estoy en la ultima lista de mi hash
		return true;
	}

    return false;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter){
	lista_iter_destruir(iter->lista_iter);
    free(iter);
}