
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "heap.h"
#include "cola.h"
#include "cola_vip.h"

struct cola_vip{
	heap_t* normal;
	cola_t* vip;
	size_t cant;
};

cola_vip_t* cola_vip_crear(cmp_func_t cmp, void* extra_data){

	cola_vip_t* cola_vip = malloc(sizeof(cola_vip_t));
	if(cola_vip == NULL) return NULL;

	cola_vip->vip = cola_crear();
	if(cola_vip->vip == NULL){
		free(cola_vip);
		return NULL;
	}

	cola_vip->normal = heap_crear(cmp, extra_data);
	if(cola_vip->normal == NULL){
		cola_destruir(cola_vip->vip, NULL);
		free(cola_vip);
		return NULL;
	}

	cola_vip->cant = 0;

	return cola_vip;
}

void cola_vip_destruir(cola_vip_t* cola_vip, void (*destruir_dato)(void *)){

	if(destruir_dato){
		heap_destruir(cola_vip->normal, destruir_dato);
		cola_destruir(cola_vip->vip, destruir_dato);
	} else{
		heap_destruir(cola_vip->normal, NULL);
		cola_destruir(cola_vip->vip, NULL);
	}
	free(cola_vip);
}

bool cola_vip_encolar(cola_vip_t* cola_vip, void* valor, bool vip){

	if(vip){
		if(cola_encolar(cola_vip->vip, valor) == false){
			return false;
		}
	} else{
		if(heap_encolar(cola_vip->normal, (char*) valor) == false){
			return false;
		}
	}

	cola_vip->cant++;
	return true;
}

void* cola_vip_desencolar(cola_vip_t* cola_vip){

	void* valor;

	if(cola_esta_vacia(cola_vip->vip)){
		valor = heap_desencolar(cola_vip->normal);
	} else{
		valor = cola_desencolar(cola_vip->vip);
	}
	cola_vip->cant--;
	return valor;
}

bool cola_vip_esta_vacia(const cola_vip_t* cola_vip){
	return cola_vip->cant == 0;
}

void* cola_vip_ver_primero(const cola_vip_t* cola_vip){

	if(cola_vip_esta_vacia(cola_vip)) return NULL;

	if(cola_esta_vacia(cola_vip->vip)){
		return heap_ver_max(cola_vip->normal);
	} else{
		return cola_ver_primero(cola_vip->vip);
	}
}

size_t cola_vip_cantidad(const cola_vip_t* cola_vip){
	return cola_vip->cant;
}