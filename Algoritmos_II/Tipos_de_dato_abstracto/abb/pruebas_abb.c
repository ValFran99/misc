/*
 * prueba_abb.c
 * Pruebas para el tipo de dato abstracto Tabla de abb
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */

#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include <stddef.h>
#include <stdbool.h>
#define LEN_STRING 10

/* ******************************************************************
 *                        PRUEBAS UNITARIAS ABB
 * *****************************************************************/


int comparar_len(const char* str1, const char* str2){
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	if(len1 == len2) return 0;
	return len1 > len2 ? 1:-1;
}

static void prueba_crear_abb_vacio(){
	abb_t* abb = abb_crear(strcmp, NULL);

	printf("\nINICIO DE PRUEBA CON ABB VACIO\n");

	print_test("creo un abb vacío", abb);
	print_test("su cantidad de elementos es 0", abb_cantidad(abb) == 0);
	print_test("la clave A no pertenece al abb", !abb_pertenece(abb, "A"));
	print_test("obtener clave A devuelve NULL", abb_obtener(abb, "A") == NULL);
	print_test("borrar la clave A también devuelve NULL", abb_borrar(abb, "A") == NULL);

	abb_destruir(abb);
	print_test("el abb fue destruido", true);

    
}

static void prueba_abb_insertar(){

	printf("\nINICIO DE PRUEBAS INSERTAR\n");
	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "elefante", *valor3 = "??";
	char* clave4 = "bee", *valor4 = "zumzum";

	/* Inserto un valor 1 luego lo borra*/

	print_test("Inserto la clave1", abb_guardar(abb, clave1, valor1));
	print_test("la cantidad es 1", abb_cantidad(abb) == 1);
	print_test("clave1 pertenece al abb", abb_pertenece(abb, clave1));
	print_test("obtener clave1 da valor1", abb_obtener(abb, clave1) == valor1);
	print_test("borro la clave y devuelve valor1", abb_borrar(abb, clave1) == valor1);
	print_test("la cantidad ahora es 0", abb_cantidad(abb) == 0);
	print_test("clave1 ya no pertenece", !abb_pertenece(abb, clave1));
	print_test("no puedo borrar la clave1", abb_borrar(abb, clave1) == NULL);
	print_test("así como tampoco obtener", abb_obtener(abb, clave1) == NULL);

	/* Inserto dos valores que se destruyen con el hash*/

	print_test("inserto clave2", abb_guardar(abb, clave2, valor2));
	print_test("cantidad = 1", abb_cantidad(abb) == 1);
	print_test("obtener clave2 da valor2", abb_obtener(abb, clave2) == valor2);
	print_test("la clave1 pertenece", abb_pertenece(abb, clave2));

	print_test("inserto clave3", abb_guardar(abb, clave3, valor3));
	print_test("cantidad = 2", abb_cantidad(abb) == 2);
	print_test("obtener clave3 da valor3", abb_obtener(abb, clave3) == valor3);
	print_test("la clave3 pertenece", abb_pertenece(abb, clave3));

	print_test("inserto clave4", abb_guardar(abb, clave4, valor4));
	print_test("cantidad = 3", abb_cantidad(abb) == 3);
	print_test("obtener clave4 da valor4", abb_obtener(abb, clave4) == valor4);
	print_test("la clave4 pertenece", abb_pertenece(abb, clave4));

	abb_destruir(abb);
	print_test("el abb fue destruido", true);
}

static void prueba_abb_borrar(){

	printf("\nINICIO DE PRUEBAS DE BORRADO\n");

	abb_t* abb = abb_crear(comparar_len, NULL);

	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "elefante", *valor3 = "??";
	char* clave4 = "hey", *valor4 = "moona";
	char* clave5 = "e", *valor5 = "a";
	char* clave6 = "cadena_larga", *valor6 = "hey";
	char* clave7 = "pertenece", *valor7 = "no";
	char* clave8 = "realize", *valor8 = "yes";
	char* clave9 = "HALL-A", *valor9 = "VA-11";

	print_test("inserto clave9", abb_guardar(abb, clave9, valor9));
	print_test("inserto clave1", abb_guardar(abb, clave1, valor1));
	print_test("inserto clave4", abb_guardar(abb, clave4, valor4));
	print_test("inserto clave5", abb_guardar(abb, clave5, valor5));
	print_test("inserto clave2", abb_guardar(abb, clave2, valor2));
	print_test("inserto clave3", abb_guardar(abb, clave3, valor3));
	print_test("inserto clave8", abb_guardar(abb, clave8, valor8));
	print_test("inserto clave7", abb_guardar(abb, clave7, valor7));
	print_test("inserto clave6", abb_guardar(abb, clave6, valor6));
	print_test("la cantidad es 9", abb_cantidad(abb) == 9);

	/* Veo distintos casos bordes del abb_borrar */

	// Borrar raíz
	print_test("borro la raíz (clave9), devuelve valor9", abb_borrar(abb, clave9) == valor9);
	print_test("la cantidad disminuyó a 8", abb_cantidad(abb) == 8);
	print_test("clave9 ya no pertenece al arbol", !abb_pertenece(abb, clave9));
	print_test("obtener de clave9 devuelve NULL", abb_obtener(abb, clave9) == NULL);
	print_test("así como borrar", abb_borrar(abb, clave9) == NULL);

	// Caso borrar con 2 hijos
	/*

	                          O <- borrar
	                      O       O

	*/
	print_test("borro la clave4, devuelve valor4", abb_borrar(abb, clave4) == valor4);
	print_test("la cantidad disminuyó a 7", abb_cantidad(abb) == 7); 
	print_test("clave4 ya no pertenece al arbol", !abb_pertenece(abb, clave4));
	print_test("obtener de clave4 devuelve NULL", abb_obtener(abb, clave4) == NULL);
	print_test("así como borrar", abb_borrar(abb, clave4) == NULL);

	print_test("re-inserto la clave4", abb_guardar(abb, clave4, valor4));
	print_test("la cantidad ahora es 8", abb_cantidad(abb) == 8);

	/*
	Caso izquierda 1 solo hijo derecho

	                          O
	                      O <- borrar
	                          O

	*/

	print_test("borro la clave5, devuelve valor5", abb_borrar(abb, clave5) == valor5);
	print_test("la cantidad ahora es 7", abb_cantidad(abb) == 7);
	
	/*
	Caso izquierda 1 solo hijo izquierdo

	                          O
	                      O <- borrar
	                  O
	*/

	print_test("borro la clave2, devuelve valor2", abb_borrar(abb, clave2) == valor2);
	print_test("la cantidad ahora es 6", abb_cantidad(abb) == 6);


	/*
	Caso derecha 1 solo hijo derecho

	                          O
	                              O <- borrar
	                                  O
	*/

	print_test("borro la clave3, devuelve el valor3", abb_borrar(abb, clave3) == valor3);
	print_test("la cantidad ahora es 5", abb_cantidad(abb) == 5);

	/*
	Caso derecha 1 solo hijo izquierdo

	                          O
	                              O <- borrar
	                          O

	 */

	char* clave10 = "monogatari", *valor10 = "arararagi";

	print_test("inserto la clave10", abb_guardar(abb, clave10, valor10));
	print_test("la cantidad ahora es 6", abb_cantidad(abb) == 6);

	print_test("borro la clave6, devuelve valor6", abb_borrar(abb, clave6) == valor6);
	print_test("ahora la cantidad es 5", abb_cantidad(abb) == 5);

	// Caso hoja
	print_test("borro la clave4, devuelve valor4", abb_borrar(abb, clave4) == valor4);
	print_test("la cantidad ahora es 4", abb_cantidad(abb) == 4);

	// Borro los siguientes elementos

	print_test("borro la clave1, devuelve valor1", abb_borrar(abb, clave1) == valor1);
	print_test("borro la clave7, devuelve valor7", abb_borrar(abb, clave7) == valor7);
	print_test("borro la clave8, devuelve valor8", abb_borrar(abb, clave8) == valor8);
	print_test("ahora la cantidad es 1", abb_cantidad(abb) == 1);

	// Caso borro la última clave

	print_test("borro la última clave, devuelve el valor correspondiente", abb_borrar(abb, clave10) == valor10);
	print_test("la cantidad actual es 0", abb_cantidad(abb) == 0);

	abb_destruir(abb);
	print_test("el abb fue destruido", true);

}

static void prueba_abb_reemplazar(){

	printf("\nINICIO PRUEBAS REEMPLAZAR VALORES\n");

	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    print_test("inserto la clave1", abb_guardar(abb, clave1, valor1a));
    print_test("obtener clave1 devuelve valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("inserto la clave2", abb_guardar(abb, clave2, valor2a));
    print_test("obtener clave2 devuelve valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Inserto clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("obtener clave1 ahora devuelve valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("inserto clave2 con valor2b", abb_guardar(abb, clave2, valor2b));
    print_test("obtener clave2 ahora devuelve valor2b", abb_obtener(abb, clave2) == valor2b);

    abb_destruir(abb);

    print_test("el abb fue destruido", true);

}

static void prueba_abb_reemplazar_con_destruir(){

	printf("\nINICIO PRUEBAS REEMPLAZAR CON DESTRUIR\n");

	abb_t* abb = abb_crear(strcmp, free);

	char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    print_test("inserto la clave1", abb_guardar(abb, clave1, valor1a));
    print_test("obtener clave1 devuelve valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("inserto la clave2", abb_guardar(abb, clave2, valor2a));
    print_test("obtener clave2 devuelve valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Inserto clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("obtener clave1 ahora devuelve valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("inserto clave2 con valor2b", abb_guardar(abb, clave2, valor2b));
    print_test("obtener clave2 ahora devuelve valor2b", abb_obtener(abb, clave2) == valor2b);

    abb_destruir(abb);

    print_test("el abb fue destruido", true);
}

static void prueba_abb_clave_vacia(){

	printf("\nINICIO PRUEBA CLAVE VACIA\n");
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave = "", *valor = "";

	print_test("inserto clave vacia", abb_guardar(abb, clave, valor));
	print_test("la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("obtener clave vacía es valor", abb_obtener(abb, clave) == valor);
	print_test("pertenece clave vacía es true", abb_pertenece(abb, clave));
	print_test("borrar clave vacía devuelve valor", abb_borrar(abb, clave) == valor);
	print_test("la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
    print_test("el abb fue destruido", true);
}

static void prueba_abb_valor_null(){

	printf("\nINICIO PRUEBA VALOR NULL\n");

	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave = "", *valor = NULL;

	print_test("insertar clave vacía con valor NULL", abb_guardar(abb, clave, valor));
	print_test("la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("obtener clave vacía es NULL", abb_obtener(abb, clave) == NULL);
	print_test("pero la clave vacía pertenece", abb_pertenece(abb, clave));
	print_test("borrar clave vacía devuelve NULL", abb_borrar(abb, clave) == NULL);
	print_test("pero aún así se borró la clave, su cantidad es 0", abb_cantidad(abb) == 0);

	abb_destruir(abb);
	print_test("se destruyó el abb", true);
}

char* generar_cadena_random(size_t tam, char* pool, size_t len_pool){

	char* dest = malloc(tam);
	if(dest == NULL) return NULL;

	size_t fin = 0;

	for(int i = 0; i < tam - 1; i++){
		size_t i_letra = rand() % len_pool;
		dest[fin] = pool[i_letra];
		fin++;
	}
	dest[fin] = '\0';

	return dest;
}

static void prueba_abb_volumen(){
	
	printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

	abb_t* abb_1 = abb_crear(strcmp, NULL);

	char* pool = "abcdefghijklmnopqrstuvwxyz";
	size_t len_pool = strlen(pool);

	char *claves[1000];
	char *valores[1000];
	bool ok_guardar = true;

	for(int i = 0; i < 1000; i++){
		claves[i] = generar_cadena_random(LEN_STRING, pool, len_pool);
		valores[i] = generar_cadena_random(LEN_STRING, pool, len_pool);

		ok_guardar &= abb_guardar(abb_1, claves[i], valores[i]);
		if(!ok_guardar) break;
	}

	print_test("se guardaron todos los valores y sus claves", ok_guardar);
	print_test("la cantidad es la correcta", abb_cantidad(abb_1) == 1000);

	bool ok_pertenece = true;
	bool ok_obtener = true;

	for(int i = 0; i < 1000; i++){
		ok_pertenece &= abb_pertenece(abb_1, claves[i]);
		ok_obtener &= abb_obtener(abb_1, claves[i]) == valores[i];
		if(!ok_pertenece || !ok_obtener) break;
	}

	print_test("todas las claves pertenecen", ok_pertenece);
	print_test("obtener devuelve los valores que debería", ok_obtener);

	bool ok_borrar = true;

	for(int i = 0; i < 1000; i++){
		char* valor = abb_borrar(abb_1, claves[i]);
		ok_borrar &= valor == valores[i];
		free(valor);
		if(!ok_borrar) break;
	}

	print_test("se borraron todas las claves y sus valores", ok_borrar);

	abb_destruir(abb_1);
	print_test("se destruyó el abb_1", true);

	abb_t* abb_2 = abb_crear(strcmp, free);

	for(int i = 0; i < 1000; i++){
		char* clave = generar_cadena_random(LEN_STRING, pool, len_pool);
		char* valor = generar_cadena_random(LEN_STRING, pool, len_pool);
		ok_guardar &= abb_guardar(abb_2, clave, valor);
		if(!ok_guardar) break;
		free(clave);
	}

	print_test("se guardaron todos los pares clave|valor en abb_2", ok_guardar);

	abb_destruir(abb_2);
	print_test("se destruyó el abb_2", true);

	/* Como el abb guarda una copia de las claves, tengo que liberar yo las */
	for(int i = 0; i < 1000; i++){
		free(claves[i]);
	}
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS ITER
 * *****************************************************************/

bool visitar_1(const char *clave, void *valor, void *extra){

    if(!clave || !valor) return false;

    printf("%s\n", (char*) clave); 
    printf("%s\n", (char*) valor); 

    return true; 

}


bool visitar_2(const char *clave, void *valor, void *extra){

    
    if (*(size_t*) extra >= 2){
        return false;
    } 
    *(size_t*) extra += 1; 
    printf("%s\n", (char*) clave); 
    printf("%s\n", (char*) valor); 
    return true;
    

}

static void prueba_iterar_abb_vacio()
{
    printf("\nINICIO DE PRUEBAS DE ITERADOR VACIO\n");
    abb_t* abb = abb_crear(NULL, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter)); 
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_abb_iterar()
{
    printf("\nINICIO DE PRUEBAS DE ITERADOR\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    /* Inserta 3 valores */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Primer valor */
    
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    printf("\nINICIO DE PRUEBAS DE ITER INT\n");

    printf("\nImprimo todos los elemntos\n");

    abb_in_order(abb, visitar_1, NULL);

    printf("\nImprimo todos los 2 primeros elemntos\n");

    size_t extra = 0;

    abb_in_order(abb, visitar_2, &extra);

    abb_iter_in_destruir(iter);
    print_test("el abb_iter fue destruido", true);
    abb_destruir(abb);
    print_test("el abb fue destruido", true);
}

static void prueba_abb_iterar_volumen(size_t largo){
    printf("\nINICIO DE PRUEBAS DE VOLUMEN ITER\n");

	abb_t* abb = abb_crear(strcmp, NULL);

	char* pool = "abcdefghijklmnopqrstuvwxyz";
	size_t len_pool = strlen(pool);

	char *claves[largo];
	char *valores[largo];
	bool ok_guardar = true;

	for(int i = 0; i < largo; i++){
		claves[i] = generar_cadena_random(LEN_STRING, pool, len_pool);
		valores[i] = generar_cadena_random(LEN_STRING, pool, len_pool);
		ok_guardar &= abb_guardar(abb, claves[i], valores[i]);
		if(!ok_guardar) break;
	} 

    abb_iter_t* iter = abb_iter_in_crear(abb);
    
    print_test("Prueba abb iter esta al final", !abb_iter_in_al_final(iter)); 


    bool ok = true;
    int i = 0;
    const char *clave;
    char *valor;
	

	while(i < 5000){

        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        
        abb_iter_in_avanzar(iter);   
        i++;
	}

    print_test("Prueba abb iteración en volumen", ok);
    printf("\n%d\n", i);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    print_test("el abb_iter fue destruido", true);

	

	for(int j = 0; j < 5000; j++){
		free(claves[j]);
		free(valores[j]);
	}

	
    
    abb_destruir(abb);
    print_test("el abb fue destruido", true);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_estudiante()
{
    printf("\nINICIO DE PRUEBAS ESTUDIANTE\n");

	prueba_crear_abb_vacio();
	prueba_abb_insertar();
	prueba_abb_borrar();
	prueba_abb_reemplazar();
	prueba_abb_reemplazar_con_destruir();
	prueba_abb_clave_vacia();
	prueba_abb_valor_null();
	prueba_abb_volumen();
    
    prueba_iterar_abb_vacio();
    prueba_abb_iterar();
    prueba_abb_iterar_volumen(5000);
    
    printf("\nFIN DE PRUEBAS ESTUDIANTE\n");
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif