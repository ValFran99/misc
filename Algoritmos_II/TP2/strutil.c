
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include "strutil.h"
#include <stdlib.h>
#include <string.h>

char *substr(const char *str, size_t n){
	return strndup(str, n);
}

char** crear_strv(const char* str, size_t len, char sep){

	size_t cant_cadenas = 1;
	
	for(int i = 0; i < len; i++){
		if(str[i] == sep){
			cant_cadenas++;
		}
	}
	cant_cadenas++;
	char **strv = malloc(sizeof(char*) * cant_cadenas);
	return strv;
}

char **split(const char *str, char sep){

	size_t len = strlen(str);

	char** strv = crear_strv(str, len, sep);
	if(strv == NULL) return NULL;

	int i_inicial = 0;
	int pos_strv = 0;
	int i;
	for(i = 0; i < len; i++){
		if(str[i] == sep){
			strv[pos_strv] = substr(str + i_inicial, i - i_inicial);
			pos_strv++;
			i_inicial = i + 1;
		}
	}
	strv[pos_strv] = substr(str + i_inicial, i - i_inicial);
	pos_strv++;
	strv[pos_strv] = NULL;
	
	return strv;
}

/* Concatena la segunda cadena recibida al final de la primer cadena, además recibe un parámetro extra
que indica la posición del caracter nulo de la primer cadena.
Actualiza ese valor al finalizar la función y devuelve la primer cadena. */
char* concatenar_str(char* destino, char* origen, size_t* final){

	size_t i;
	size_t j;
	for(i = *final, j = 0; origen[j] != '\0'; i++, j++){
		destino[i] = origen[j];
	}
	destino[i] = '\0';
	*final = i;
	return destino;
}

/* Concatena un caracter al final de la cadena recibida, además recibe un parámetro extra que indica
la posición del caracter nulo de la cadena.
Actualiza ese valor al finalizar la función y devuelve la primer cadena */
char* concatenar_char(char* destino, char caracter, size_t* final){

	destino[*final] = caracter;
	if(caracter == '\0') return destino;
	destino[++*final] = '\0';
	
	return destino;
}

char* crear_cadena_vacia(){
	char* vacia = malloc(sizeof(char));
	if(vacia == NULL) return NULL;
	vacia[0] = '\0';
	return vacia;
}

char* crear_cadena_destino(char** strv){

	char* cadena;
	size_t cant_sep = 0;
	size_t cant_char = 0;
	size_t tam_cad;

	for(int i = 0; strv[i] != NULL; i++){
		cant_sep++;
		cadena = strv[i];
		for(int j = 0; cadena[j] != '\0'; j++){
			cant_char++;
		}
	}

	tam_cad = cant_sep + cant_char + 2;

	char* unida = malloc(tam_cad);

	return unida;
}

char *join(char **strv, char sep){

	if(strv == NULL || strv[0] == NULL){
		return crear_cadena_vacia();
	}

	char* cadena;
	char* unida = crear_cadena_destino(strv);
	if(unida == NULL) return NULL;

	size_t final = 0;
	for(int i = 0; strv[i] != NULL; i++){

		cadena = strv[i];
		concatenar_str(unida, cadena, &final);
		if(strv[i + 1] != NULL) concatenar_char(unida, sep, &final);
	}
	return unida;
}

void free_strv(char *strv[]){

	for(int i = 0; strv[i] != NULL; i++){
		free(strv[i]);
	}
	free(strv);

}