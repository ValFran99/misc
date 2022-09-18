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
#include "strutil.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define URGENTE "URGENTE"
#define REGULAR "REGULAR"

#define CAMPO_COMANDO 0
#define CAMPO_PARAMETROS 1
#define ERROR_MAIN 1

#define CANT_PARAM_PEDIR_TURNO 3
#define PEDIR_TURNO_PACIENTE 0
#define PEDIR_TURNO_ESPECIALIDAD 1
#define PEDIR_TURNO_URGENCIA 2

#define CANT_PARAM_ATENDER_SIGUIENTE 1
#define ATENDER_SIGUIENTE_DOCTOR 0

#define CANT_PARAM_INFORME 2
#define INFORME_DOCTOR_INI 0
#define INFORME_DOCTOR_FIN 1

size_t cant_parametros(char** parametros){

	int cant_parametros;
	for(cant_parametros = 0; parametros[cant_parametros] != NULL; cant_parametros++){
	}
	return cant_parametros;
}

bool chequear_parametros_pedir_turno(hash_t* pacientes, hash_t* lista_de_espera, char** parametros){

	bool todo_ok = true;     

	if(cant_parametros(parametros) != CANT_PARAM_PEDIR_TURNO){
		printf(ENOENT_PARAMS, COMANDO_PEDIR_TURNO);
		return false;
	}
	char* paciente = parametros[PEDIR_TURNO_PACIENTE];
	char* especialidad = parametros[PEDIR_TURNO_ESPECIALIDAD];
	char* urgencia = parametros[PEDIR_TURNO_URGENCIA];

	if(hash_pertenece(pacientes, paciente) == false){
		printf(ENOENT_PACIENTE, paciente);
		todo_ok = false;
	}

	if(hash_pertenece(lista_de_espera, especialidad) == false){
		printf(ENOENT_ESPECIALIDAD, especialidad);
		todo_ok = false;
	}

	if(strcmp(urgencia, URGENTE) != 0 && strcmp(urgencia, REGULAR) != 0){
		printf(ENOENT_URGENCIA, urgencia);
		todo_ok = false;
	}
	return todo_ok;
}

bool chequear_parametros_atender_siguiente(abb_t* doctores, char** parametros){

	if(cant_parametros(parametros) != CANT_PARAM_ATENDER_SIGUIENTE){
		printf(ENOENT_PARAMS, COMANDO_PEDIR_TURNO);
		return false;
	}

	if(abb_pertenece(doctores, parametros[ATENDER_SIGUIENTE_DOCTOR]) == false){
		printf(ENOENT_DOCTOR, parametros[ATENDER_SIGUIENTE_DOCTOR]);
		return false;
	}
	return true;
}	

bool chequear_parametros_informe(char** parametros){

	if(cant_parametros(parametros) != CANT_PARAM_INFORME){
		printf(ENOENT_PARAMS, COMANDO_INFORME);
		return false;
	}
	return true;
}

void lista_doctores(abb_t* doctores, char* min, char* max){

	size_t cont = 1;
	abb_iter_t* iter = abb_iter_in_crear(doctores, min, max);
	bool suma = false;

	while(!abb_iter_in_al_final(iter)){
		const char* clave = abb_iter_in_ver_actual(iter);
		registro_doc_t* registro = abb_obtener(doctores, clave);
		char* esp = registro_datos(registro);
		size_t cant_curados = doc_contador(registro, suma);
		printf(INFORME_DOCTOR, cont, clave, esp, cant_curados);
		abb_iter_in_avanzar(iter);
		cont++;
	}
	abb_iter_in_destruir(iter);
}

size_t cant_informe(abb_t* doctores, char* min, char* max){

	size_t cant = 0;
	abb_iter_t* iter = abb_iter_in_crear(doctores, min, max);
	
	while(abb_iter_in_al_final(iter) == false){
		abb_iter_in_avanzar(iter);
		cant++;
	}
	abb_iter_in_destruir(iter);
	return cant;
}

void procesar_comando(const char* comando, char** parametros, abb_t* doctores, hash_t* pacientes, hash_t* lista_de_espera, hash_t* comandos) {

	if(hash_pertenece(comandos, comando) == false){
		printf(ENOENT_CMD, comando);
		return;
	} else if(strcmp(comando, COMANDO_PEDIR_TURNO) == 0 && chequear_parametros_pedir_turno(pacientes, lista_de_espera, parametros) == true) {
		
		size_t cant;

		char* paciente = parametros[PEDIR_TURNO_PACIENTE];
		char* especialidad = parametros[PEDIR_TURNO_ESPECIALIDAD];
		char* urgencia = parametros[PEDIR_TURNO_URGENCIA];

		encolar_lista_de_espera(lista_de_espera, strdup(paciente), especialidad, strcmp(urgencia, URGENTE) == 0, &cant);
		printf(PACIENTE_ENCOLADO, paciente);
		printf(CANT_PACIENTES_ENCOLADOS, cant, especialidad);

	} else if(strcmp(comando, COMANDO_ATENDER) == 0 && chequear_parametros_atender_siguiente(doctores, parametros) == true) {
			
		registro_doc_t* registro = abb_obtener(doctores, parametros[ATENDER_SIGUIENTE_DOCTOR]); 
		char* especialidad = registro_datos(registro);
		cola_vip_t* cola_vip = hash_obtener(lista_de_espera, especialidad);

		if(cola_vip_esta_vacia(cola_vip)){
			printf(SIN_PACIENTES);
			return;
		}
		bool suma = true;
		doc_contador(registro, suma);
		char* atendido = cola_vip_desencolar(cola_vip);
		printf(PACIENTE_ATENDIDO, atendido);
		printf(CANT_PACIENTES_ENCOLADOS, cola_vip_cantidad(cola_vip), especialidad);
		free(atendido);
		
	} else if(strcmp(comando, COMANDO_INFORME) == 0 && chequear_parametros_informe(parametros) == true) {

		char* inicio = (strcmp(parametros[INFORME_DOCTOR_INI], "") == 0)? NULL:parametros[INFORME_DOCTOR_INI];
		char* fin = (strcmp(parametros[INFORME_DOCTOR_FIN], "") == 0)? NULL:parametros[INFORME_DOCTOR_FIN];
		printf(DOCTORES_SISTEMA, cant_informe(doctores, inicio, fin));
		lista_doctores(doctores, inicio, fin);
	}
}

void procesar_entrada(abb_t* doctores, hash_t* pacientes, hash_t* lista_de_espera, hash_t* comandos) {
	
	char* linea = NULL;
	size_t c = 0;

	while(getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if(campos[CAMPO_PARAMETROS] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[CAMPO_PARAMETROS], ',');
		procesar_comando(campos[CAMPO_COMANDO], parametros, doctores, pacientes, lista_de_espera, comandos);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int main(int argc, char** argv) {

	hash_t* hash_comandos = hash_crear(NULL);
	if(hash_comandos == NULL) return ERROR_MAIN;

	abb_t* abb_doctores = abb_crear(strcmp, destruir_registro);
	if(abb_doctores == NULL){
		hash_destruir(hash_comandos);
		return ERROR_MAIN;
	}

	hash_t* hash_pacientes = hash_crear(free);
	if(hash_pacientes == NULL){
		hash_destruir(hash_comandos);
		abb_destruir(abb_doctores);
		return ERROR_MAIN;
	}

	hash_t* lista_de_espera = hash_crear(NULL);
	if(lista_de_espera == NULL){
		hash_destruir(hash_comandos);
		abb_destruir(abb_doctores);
		hash_destruir(hash_pacientes);
		return ERROR_MAIN;
	}

	if(cargar_datos(abb_doctores, hash_pacientes, lista_de_espera, hash_comandos, argc, argv) == false){
		destruir_estructuras(hash_comandos, hash_pacientes, abb_doctores, lista_de_espera);
		return ERROR_MAIN;
	}

	procesar_entrada(abb_doctores, hash_pacientes, lista_de_espera, hash_comandos);

	destruir_estructuras(hash_comandos, hash_pacientes, abb_doctores, lista_de_espera);
	
	return 0;
}
