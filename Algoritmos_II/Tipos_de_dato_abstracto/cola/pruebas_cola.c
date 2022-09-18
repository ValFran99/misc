#include "cola.h"
#include "pila.h"
#include "testing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Prueba de que una cola vacía se comporta como debe */
static void prueba_cola_vacia(void){

	printf("\nINICIO DE PRUEBAS CON COLA VACÍA\n");
	cola_t *cola = cola_crear();

	print_test("se creó la cola", cola != NULL);
	print_test("la cola debe de estar vacía", cola_esta_vacia(cola));
	print_test("no debe tener ningún elemento al inicio", cola_ver_primero(cola) == NULL);
	print_test("no debo poder desencolar ningún elemento", cola_desencolar(cola) == NULL);

	cola_destruir(cola, NULL);
	print_test("la cola fue destruida", true);

}

/* Pruebas de encolar y desencolar unos pocos elementos*/
static void prueba_cola_pocos_enteros(void){

	printf("\nINICIO DE PRUEBAS CON POCOS ENTEROS\n");

	cola_t* cola = cola_crear();

	print_test("se creó la cola", cola != NULL);

	int i = 2;
	int j = 7;
	int k = 1;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;

	printf("Encolo tres elementos\n");

	print_test("encolo el primer elemento", cola_encolar(cola, ele1));
	print_test("ahora la cola no está vacía", !cola_esta_vacia(cola));
	print_test("el primer elemento debe ser el que ingrese", cola_ver_primero(cola) == ele1);
	print_test("encolo el segundo elemento", cola_encolar(cola, ele2));
	print_test("la cola no debe estar vacía", !cola_esta_vacia(cola));
	print_test("el primero debe seguir siendo el primero que ingresé", cola_ver_primero(cola) == ele1);
	print_test("ingreso el tercer y último elemento", cola_encolar(cola, ele3));
	print_test("el primero debe seguir siendo el primero que ingresé", cola_ver_primero(cola) == ele1);

	printf("Desencolo esos elementos ingresados\n");

	print_test("el primer elemento que desencolo debe ser el primero que encolé", cola_desencolar(cola) == ele1);
	print_test("la cola no debe estar vacía", !cola_esta_vacia(cola));
	print_test("ahora el primero debe ser el segundo elemento que ingresé", cola_ver_primero(cola) == ele2);
	print_test("el segundo elemento que desencolo debe ser el segundo que encolé", cola_desencolar(cola) == ele2);
	print_test("la cola no debe estar vacía", !cola_esta_vacia(cola));
	print_test("ahora el primer elemento de la cola debe ser el último elemento que ingresé", cola_ver_primero(cola) == ele3);
	print_test("desencolo el último elemento", cola_desencolar(cola) == ele3);
	print_test("ahora si la cola debe estar vacía", cola_esta_vacia(cola));

	printf("La cola vacía se debe comportar como una cola recién creada\n");

	print_test("no debe tener un primero", !cola_ver_primero(cola));
	print_test("no debo poder desencolar nada", cola_desencolar(cola) == NULL);

	cola_destruir(cola, NULL);

	print_test("la cola fue destruida", true);
}

/* Prueba de encolar y desencolar NULLs */
static void prueba_cola_null(void){

	printf("\nINICIO DE PRUEBAS CON NULL\n");

	cola_t* cola = cola_crear();

	print_test("la cola fue creada", cola != NULL);

	printf("Apilo NULLs\n");

	print_test("puedo encolar NULL", cola_encolar(cola, NULL));
	print_test("el primer elemento de la cola es NULL", cola_ver_primero(cola) == NULL);
	print_test("pero la cola no está vacía", !cola_esta_vacia(cola));
	print_test("encolo otro NULL", cola_encolar(cola, NULL));
	print_test("encolo otro NULL", cola_encolar(cola, NULL));
	print_test("el primero sigue siendo NULL", cola_ver_primero(cola) == NULL);
	print_test("y la cola sigue sin estar vacía", !cola_esta_vacia(cola));

	printf("Ahora desapilo los NULLs\n");

	print_test("desapilo el primer NULL", cola_desencolar(cola) == NULL);
	print_test("el primero sigue siendo NULL", cola_ver_primero(cola) == NULL);
	print_test("pero la cola sigue sin estar vacía", !cola_esta_vacia(cola));
	print_test("desencolo el segundo NULL", cola_desencolar(cola) == NULL);
	print_test("desencolo el tercer NULL", cola_desencolar(cola) == NULL);
	print_test("ahora sí ver_primero devuelve NULL", cola_ver_primero(cola) == NULL);
	print_test("y la cola sí está vacía", cola_esta_vacia(cola));
	print_test("no puedo desencolar más", cola_desencolar(cola) == NULL);

	cola_destruir(cola, NULL);
	print_test("la cola fue destruida", true);
}

/* Pruebas de encolar/desencolar un volumen grande de elementos */
static void prueba_cola_volumen(void){

	printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

	cola_t* cola = cola_crear();

	print_test("la cola fue creada", cola != NULL);

	int v = 10;
	int* p = &v;
	int* vector[1000];

	for(int i = 0; i < 1000; i++){
		vector[i] = p++;
	}

	printf("Encolo 1000 elementos\n");

	bool ok = true;
	bool ok_encolar = true;

	for(int i = 0; i < 1000; i++){
		ok_encolar &= cola_encolar(cola, vector[i]);
		ok &= cola_ver_primero(cola) == vector[0];
	}

	print_test("pude encolar todos los elementos", ok_encolar);
	print_test("verifico que el primero siempre es el primer elemento que encolé", ok);

	printf("Ahora los desencolo\n");

	int* desencolado;
	bool ok_primero = true;

	for(int i = 0; i < 1000; i++){
		desencolado = cola_desencolar(cola);
		ok_primero &= desencolado != cola_ver_primero(cola);
		ok &= desencolado == vector[i];
	}

	print_test("el último desencolado no es el primero", ok_primero);
	print_test("se cumple que el primero en entrar es el primero en salir", ok);
	print_test("la cola está vacía", cola_esta_vacia(cola));

	cola_destruir(cola, NULL);
	print_test("la cola fue destruida", true);
}

/* Prueba de destruir una cola que aún tiene elementos, y los elementos son punteros estáticos */
static void prueba_cola_destruir_no_vacia(void){

	printf("\nINICIO PRUEBA DE DESTRUCCIÓN DE COLA NO VACÍA\n");

	cola_t* cola = cola_crear();

	print_test("la cola fue creada", cola != NULL);

	int i = 2;
	int j = 7;
	int k = 1;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;

	printf("Encolo algunos elementos\n");

	print_test("encolo el primer elemento", cola_encolar(cola, ele1));
	print_test("encolo el segundo elemento", cola_encolar(cola, ele2));
	print_test("encolo el tercer elemento", cola_encolar(cola, ele3));

	printf("Ahora destruyo la cola no vacía\n");

	cola_destruir(cola, NULL);
	print_test("la cola fue destruida", cola);

}

/* Prueba en la que destruyo una cola con punteros dinámicos sin destruir los punteros en sí mismos */
static void prueba_cola_sin_destruir_datos(void){

	printf("\nINICIO DE PRUEBAS DESTRUYENDO COLA SIN DESTRUIR LOS DATOS\n");

	cola_t* cola = cola_crear();

	print_test("se creó la cola", cola != NULL);

	int* ele1 = malloc(sizeof(int));
	int* ele2 = malloc(sizeof(int));
	int* ele3 = malloc(sizeof(int));

	*ele1 = 7;
	*ele2 = 2;
	*ele3 = 1;

	printf("Encolo los punteros dinámicos\n");

	print_test("encolo el primer elemento", cola_encolar(cola, ele1));
	print_test("encolo el segundo elemento", cola_encolar(cola, ele2));
	print_test("encolo el tercer elemento", cola_encolar(cola, ele3));
	print_test("el primero de la cola debe ser el primer elemento encolado", cola_ver_primero(cola) == ele1);

	printf("Destruyo la cola\n");

	cola_destruir(cola, NULL);

	print_test("la cola fue destruida sin destruir los datos", true);

	free(ele1);
	free(ele2);
	free(ele3);

	print_test("y ahora destruyo los datos individualmente", true);
}

/* Prueba con punteros dinámicos en la que destruyo la cola junto con dichos punteros */
static void prueba_cola_destruir_free(void){

	printf("\nINICIO PRUEBAS DESTRUYENDO COLA Y SUS DATOS\n");

	cola_t* cola = cola_crear();

	print_test("se creó la cola", cola != NULL);

	int* ele1 = malloc(sizeof(int));
	int* ele2 = malloc(sizeof(int));
	int* ele3 = malloc(sizeof(int));

	*ele1 = 7;
	*ele2 = 2;
	*ele3 = 1;

	printf("Encolo los punteros dinámicos\n");

	print_test("encolo el primer elemento", cola_encolar(cola, ele1));
	print_test("encolo el segundo elemento", cola_encolar(cola, ele2));
	print_test("encolo el tercer elemento", cola_encolar(cola, ele3));

	printf("Destruyo la cola\n");

	cola_destruir(cola, free);

	print_test("se destruyó la cola junto con los datos", true);

}

void destruir_pila_wrapper(void* pila){
	pila_destruir(pila);
}

/* Prueba de encolar/desencolar pilas y también destruir la cola junto con todas las pilas */
static void prueba_cola_destruir_pila(void){

	printf("\nINICIO PRUEBAS DE ENCOLAR Y DESTRUIR PILAS\n");

	printf("Creo la cola\n");

	cola_t* cola = cola_crear();

	print_test("la cola fue creada", cola != NULL);

	printf("Creo varias pilas\n");
	pila_t* pila_1 = pila_crear();
	print_test("se creó la pila 1", pila_1 != NULL);
	pila_t* pila_2 = pila_crear();
	print_test("se creó la pila 2", pila_2 != NULL);
	pila_t* pila_3 = pila_crear();
	print_test("se creó la pila 3", pila_3 != NULL);

	printf("Apilo algunos elementos en cada una\n");

	int i = 2;
	int j = 7;
	int k = 1;
	int l = 10;
	int m = 12;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;
	int* ele5 = &m;

	print_test("apilo el elemento 1 en la pila 1", pila_apilar(pila_1, ele1));
	print_test("apilo el elemento 2 en la pila 1", pila_apilar(pila_1, ele2));
	print_test("apilo el elemento 3 en la pila 2", pila_apilar(pila_2, ele3));
	print_test("apilo el elemento 4 en la pila 3", pila_apilar(pila_3, ele4));
	print_test("apilo el elemento 5 en la pila 3", pila_apilar(pila_3, ele5));

	print_test("encolo la primer pila", cola_encolar(cola, pila_1));
	print_test("encolo la segunda pila", cola_encolar(cola, pila_2));
	print_test("encolo la tercer pila", cola_encolar(cola, pila_3));
	print_test("ver_primero debe devolver la primer pila", cola_ver_primero(cola) == pila_1);
	print_test("la cola no debe estar vacía", !cola_esta_vacia(cola));

	cola_destruir(cola, destruir_pila_wrapper);

	print_test("la cola fue destruida junto con las 3 pilas", true);
}

void pruebas_cola_estudiante(){

	printf("\nINICIO DE PRUEBAS ESTUDIANTE\n");

	prueba_cola_vacia();
	prueba_cola_pocos_enteros();
	prueba_cola_null();
	prueba_cola_volumen();
	prueba_cola_destruir_no_vacia();
	prueba_cola_sin_destruir_datos();
	prueba_cola_destruir_free();
	prueba_cola_destruir_pila();

	printf("\nFIN DE PRUEBAS ESTUDIANTE\n");
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif