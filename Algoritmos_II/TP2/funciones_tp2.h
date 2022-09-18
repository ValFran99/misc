#ifndef _FUNCIONES_TP2_H
#define _FUNCIONES_TP2_H

#include "abb.h"
#include "hash.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

/* ******************************************************************
 *              Funciones de comparación y validación               *
 * *****************************************************************/

int comparar_antiguedad(const void* a, const void* b, const void* extra_data);

// Recibe una cadena con el año actual y verifica si el año recibido es menor al actual
bool validar_anio(char* anio);

typedef struct datos_csv datos_csv_t;

typedef struct registro_doc registro_doc_t;

/* ******************************************************************
 *                    		Carga de datos                          *
 * *****************************************************************/

/* Carga los datos recibidos por los archivos iniciales en las estructuras elegidas
Los pacientes los almacena en un hash, los doctores junto con su especialidad y registro dentro del abb y por último
carga la lista de espera en un hash con formato (especialidad,cola_vip)
Si la carga fue exitosa devuelve true, devuelve false en caso contrario
Pre: Fueron creadas las estructuras
Post: Los datos fueron almacenados en las estructuras correspondientes
*/
bool cargar_datos(abb_t* doctores, hash_t* pacientes, hash_t* lista_de_espera, hash_t* comandos, int argc, char** argv);

void cargar_comandos(hash_t* comandos);

void cargar_abb(lista_t* lista, abb_t* abb);

char* cargar_hash(lista_t* lista, hash_t* hash, bool (*validar)(char*));

void cargar_lista_de_espera(hash_t* lista_de_espera, abb_t* doctores, hash_t* pacientes);

/* Destruye todas las estructuras utilizadas junto con sus datos */
void destruir_estructuras(hash_t* comandos, hash_t* pacientes, abb_t* doctores, hash_t* lista_de_espera);

/* ******************************************************************
 *                    	   Lista de espera                          *
 * *****************************************************************/

/*
Encola el siguiente paciente en la lista de espera en su especialidad correspondiente, y en base a su nivel
de urgencia
Pre: fue creada la lista de espera
Post: el paciente fue encolado en la especialidad indicada
*/
void encolar_lista_de_espera(hash_t* lista_de_espera, char* paciente, char* especialidad, bool urgencia, size_t* cant_espera);

/*
Destruye la lista de espera junto con todos sus datos almacenados
Pre: La lista de espera fue creada
Post: Se destruyeron todos los datos almacenados en la lista de espera junto con ella
*/
void destruir_lista_de_espera(hash_t* lista_de_espera);

/* ******************************************************************
 *                    			Registro                            *
 * *****************************************************************/

registro_doc_t* crear_registro(char *dato);

size_t doc_contador(registro_doc_t* registro_doc, bool suma);

void* registro_datos(registro_doc_t* registro_doc); 

void destruir_registro(void* registro_doc);

#endif  // _FUNCIONES_TP2_H