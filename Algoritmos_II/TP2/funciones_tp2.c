#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

#include "abb.h"
#include "cola_vip.h"
#include "csv.h"
#include "funciones_tp2.h"
#include "hash.h"
#include "mensajes.h"

#define ANIO_ACTUAL 2021

#define CAMPO_COMANDO 0
#define CAMPO_PARAMETROS 1

#define CANT_PARAM_CARGA_INICIAL 3

/* ******************************************************************
 *                    			Datos csv                           *
 * *****************************************************************/

struct datos_csv{
    void* nombre;
    void* dato;
};

void* crear_datos_csv(char** campos, void* extra){

    datos_csv_t* datos_csv = malloc(sizeof(datos_csv_t));
    if (!datos_csv) return NULL;

    datos_csv->nombre = strdup(campos[CAMPO_COMANDO]);
    datos_csv->dato = strdup(campos[CAMPO_PARAMETROS]);
    return datos_csv;
}

void csv_dato_destruir(void* dato){

	datos_csv_t* boom = (datos_csv_t*) dato;

	free(boom->nombre);
	free(boom->dato);
	free(boom);
}

/* ******************************************************************
 *                    			Registro                            *
 * *****************************************************************/

struct registro_doc{
	void* dato; 
	size_t contador;
};

registro_doc_t* crear_registro(char *dato){

	registro_doc_t* registro_doc = malloc(sizeof(registro_doc_t));
	if (!registro_doc) return NULL;
	registro_doc->dato = dato; 
	registro_doc->contador = 0; 
	return registro_doc;
}

void destruir_registro(void* registro_doc){

	registro_doc_t* reg = (registro_doc_t*) registro_doc;

	free(reg->dato);
	free(reg);
}

size_t doc_contador(registro_doc_t* registro_doc, bool suma){

	if(suma) registro_doc->contador += 1; 
	return registro_doc->contador; 
} 

void* registro_datos(registro_doc_t* registro){
	return registro->dato;
} 

/* ******************************************************************
 *              Funciones de comparación y validación               *
 * *****************************************************************/

int comparar_antiguedad(const void* a, const void* b, const void* extra_data){

    char* a_str = (char*) a;
    char* b_str = (char*) b;

    hash_t* pacientes = (hash_t*) extra_data;

    int v1 = atoi(hash_obtener(pacientes, a_str));
    int v2 = atoi(hash_obtener(pacientes, b_str));

    if(v1 == v2) return 0;

    return v1>v2? -1:1;
}

bool es_numerica(char* cad){

	for(int i = 0; cad[i] != '\0'; i++){
		if(isdigit(cad[i]) == 0) return false;
	}
	return true;
}

bool validar_anio(char* anio){

	if(es_numerica(anio) == false) return false;

	size_t nro = strtol(anio, '\0', 10);
	if(nro > ANIO_ACTUAL || nro < 0) return false;
	return true;
}

/* ******************************************************************
 *                    	   Lista de espera                          *
 * *****************************************************************/

void encolar_lista_de_espera(hash_t* lista_de_espera, char* paciente, char* especialidad, bool urgencia, size_t* cant_espera){

	cola_vip_t* cola_esp = (cola_vip_t*) hash_obtener(lista_de_espera, especialidad);
	cola_vip_encolar(cola_esp, paciente, urgencia);
	*cant_espera = cola_vip_cantidad(cola_esp);
}

void destruir_lista_de_espera(hash_t* lista_de_espera){

	hash_iter_t* iter = hash_iter_crear(lista_de_espera);
	if(iter == NULL) return;

	while(!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		cola_vip_destruir(hash_obtener(lista_de_espera, clave), free);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	hash_destruir(lista_de_espera);
}

/* ******************************************************************
 *                    		Carga de datos                          *
 * *****************************************************************/

// Carga el hash que contiene los comandos
void cargar_comandos(hash_t* comandos){
	
	bool relleno = true;

	hash_guardar(comandos, COMANDO_PEDIR_TURNO, &relleno);
	hash_guardar(comandos, COMANDO_ATENDER, &relleno);
	hash_guardar(comandos, COMANDO_INFORME, &relleno);
}

// Carga el hash de los pacientes y antiguedades
char* cargar_hash(lista_t* lista, hash_t* hash, bool (*validar)(char*)){

	lista_iter_t* iter = lista_iter_crear(lista);

	while(!lista_iter_al_final(iter)){
		datos_csv_t* campo = lista_iter_ver_actual(iter);
		if(validar){
			if(!validar(campo->dato)){
				lista_iter_destruir(iter);
				return campo->dato;
			}
		}
		hash_guardar(hash, campo->nombre, strdup(campo->dato));
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;
}

// Carga el abb de doctores
void cargar_abb(lista_t* lista, abb_t* abb){

	lista_iter_t* iter = lista_iter_crear(lista);

	while(!lista_iter_al_final(iter)){
		datos_csv_t* campo = lista_iter_ver_actual(iter);
		registro_doc_t* registro_doc = crear_registro(strdup(campo->dato));
		abb_guardar(abb, campo->nombre, registro_doc); //Arbol de doctores, especialidades
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
}

void cargar_lista_de_espera(hash_t* lista_de_espera, abb_t* doctores, hash_t* pacientes){

	abb_iter_t* iter = abb_iter_in_crear(doctores, NULL, NULL);
	
	if(iter == NULL) return;

	while(!abb_iter_in_al_final(iter)){
		const char* doctor = abb_iter_in_ver_actual(iter);
		registro_doc_t* registro = abb_obtener(doctores, doctor); 
		const char* especialidad = registro_datos(registro);

		if(hash_pertenece(lista_de_espera, especialidad) == false){
			hash_guardar(lista_de_espera, especialidad, cola_vip_crear(comparar_antiguedad, pacientes));
		}
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
}

// Carga los datos
bool cargar_datos(abb_t* doctores, hash_t* pacientes, hash_t* lista_de_espera, hash_t* comandos, int argc, char** argv){

	if(argc != CANT_PARAM_CARGA_INICIAL){
		printf(ENOENT_CANT_PARAMS);
		return false;
	}

	for(int i = 1; i < argc; i++){
		char* file_name = argv[i];
		lista_t* csv_lista = csv_crear_estructura(file_name, crear_datos_csv, NULL);

		if(csv_lista == NULL){
			printf(ENOENT_ARCHIVO, file_name);
			return false;
		}
		char* check = NULL;

		if(i == 1){
			cargar_abb(csv_lista, doctores);

		} else{
			check = cargar_hash(csv_lista, pacientes, validar_anio);
		}

		if(check != NULL){
			printf(ENOENT_ANIO, check);
			return false;
		}
		lista_destruir(csv_lista, csv_dato_destruir);
	}
	cargar_lista_de_espera(lista_de_espera, doctores, pacientes);
	cargar_comandos(comandos);
	return true;
} 

void destruir_estructuras(hash_t* comandos, hash_t* pacientes, abb_t* doctores, hash_t* lista_de_espera){

	hash_destruir(comandos);
	abb_destruir(doctores);
	hash_destruir(pacientes);
	destruir_lista_de_espera(lista_de_espera);
}