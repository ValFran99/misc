#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <string.h>

/* Pruebas con una pila vacía */
static void prueba_pila_vacia(void) {
	
	printf("INICIO DE PRUEBAS CON PILA VACIA\n");

    pila_t *pila = pila_crear();

    print_test("se creó la pila", pila != NULL);
    print_test("la pila está vacia", pila_esta_vacia(pila));
    print_test("no devuelve nada al ver el tope", pila_ver_tope(pila) == NULL);
    print_test("no devuelve nada al desapilar", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
    print_test("la pila fue destruida", true);
}

/* Pruebas de apilar y desapilar unos pocos elementos */
static void prueba_apilar_pocos_elementos(void){

	printf("\nINICIO DE PRUEBAS CON ALGUNOS ELEMENTOS\n");

	pila_t *pila = pila_crear();

	int i = 2;
	int j = 7;
	int k = 1;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;

	/* Apilo unos pocos elementos */

	printf("\nAPILO UNOS POCOS ELEMENTOS\n");

	print_test("se creó la pila", pila!= NULL);
	print_test("debo poder apilar", pila_apilar(pila, ele1) == true);
	print_test("la pila no debe estar vacía", !pila_esta_vacia(pila));
	print_test("el tope debe ser el ultimo elemento ingresado", pila_ver_tope(pila) == ele1);
	print_test("apilo el segundo elemento", pila_apilar(pila, ele2));
	print_test("la pila no debe estar vacía", !pila_esta_vacia(pila));
	print_test("el tope debe ser el último elemento ingresado", pila_ver_tope(pila) == ele2);
	print_test("apilo el tercer elemento",pila_apilar(pila, ele3));
	print_test("el tope debe ser el último elemento ingresado", pila_ver_tope(pila) == ele3);

	/* Desapilo uno solo */

	printf("\nDESAPILO UN ELEMENTO \n");

	int* devuelto = pila_desapilar(pila);

	print_test("el elemento devuelto debe ser el mismo que ingresé por última vez", devuelto == ele3);
	print_test("el nuevo tope debe ser el anteultimo elemento que apilé", pila_ver_tope(pila) == ele2);
	print_test("la pila no debe estar vacía", !pila_esta_vacia(pila));

	/* Desapilo todos los elementos */

	printf("\nDESAPILO TODOS LOS ELEMENTOS\n");

	devuelto = pila_desapilar(pila);

	print_test("el elemento devuelto debe ser el segundo que apilé", devuelto == ele2);

	devuelto = pila_desapilar(pila);

	print_test("el elemento devuelto debe ser el primero que apilé", devuelto == ele1);

	print_test("la pila ahora debe estar vacia", pila_esta_vacia(pila));
	print_test("el último elemento devuelto debe ser igual al primero que apile", devuelto == ele1);

	printf("\nLA PILA VACÍA SE DEBE COMPORTAR COMO UNA PILA RECIÉN CREADA \n");

	print_test("la pila debe estar vacía", pila_esta_vacia(pila));
	print_test("no debe devolver nada al ver el tope", pila_ver_tope(pila) == NULL);
	print_test("no debe devolver nada al desapilar", pila_desapilar(pila) == NULL);

	pila_destruir(pila);
	print_test("la pila fue destruida", true);
}

/* Pruebas con un volumen de datos grande para verificar que se modifique el tamaño correctamente */
static void prueba_pila_volumen(void){

	printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

	pila_t *pila = pila_crear();
	int v = 10;
	int* p = &v;
	int* vector[1000];

	for(int i = 0; i < 1000; i++){
		vector[i] = p++;
	}
	printf("\nAPILO 1000 ELEMENTOS\n");

	bool ok = true;

	for(int i = 0; i < 1000; i++){
		pila_apilar(pila, vector[i]);
		ok &= pila_ver_tope(pila) == vector[i];
	}

	print_test("verifico que el tope siempre es el último que apilé", ok);

	printf("\nAHORA LOS DESAPILO\n");

	int* desapilado;
	bool ok_tope = true;

	for(int i = 999; i >= 0; i--){
		desapilado = pila_desapilar(pila);
		ok_tope &= desapilado != pila_ver_tope(pila);
		ok &= desapilado == vector[i];
	}

	print_test("el último desapilado no es el tope", ok_tope);
	print_test("se cumple que el último en entrar es el primero en salir", ok);
	print_test("la pila está vacía", pila_esta_vacia(pila));

	pila_destruir(pila);
	print_test("la pila fue destruida", true);
}

/* Pruebas con NULL para verificar que funcione igualmente */
static void prueba_null(void){

	printf("\nPRUEBAS CON NULL\n");

	pila_t *pila = pila_crear();

	int v = 9;
	int* p = &v;

	printf("\nAPILANDO\n");

	print_test("debo poder apilar NULL", pila_apilar(pila, NULL));
	print_test("el tope debe ser NULL", pila_ver_tope(pila) == NULL);
	print_test("la pila no debe estar vacía", !pila_esta_vacia(pila));
	print_test("apilo el siguiente NULL", pila_apilar(pila, NULL));
	print_test("apilo un puntero cualquiera", pila_apilar(pila, p));
	print_test("el tope es el último puntero ingresado", pila_ver_tope(pila) == p);

	// Desapilo

	printf("\nDESAPILANDO\n");
	
	print_test("el último elemento ingresado es el primer desapilado", pila_desapilar(pila) == p);
	print_test("el tope debe ser NULL", pila_ver_tope(pila) == NULL);
	print_test("la pila no debe estar vacía", !pila_esta_vacia(pila));
	print_test("el siguiente desapilado es NULL", pila_desapilar(pila) == NULL);
	print_test("la pila no debe estar vacía", !pila_esta_vacia(pila));
	print_test("el último es también NULL", pila_desapilar(pila) == NULL);
	print_test("ahora sí la pila debe estar vacía", pila_esta_vacia(pila));
	print_test("ver tope debe devolver NULL igualmente", pila_ver_tope(pila) == NULL);


	pila_destruir(pila);
	print_test("la pila fue destruida", true);
}

/* Pruebas para verificar que la pila funciona con cualquier tipo de punteros */
static void prueba_cadenas(void){

	printf("\nINICIO PRUEBAS CON CADENAS\n");

	char* p1 = "hola";
	char* p2 = "gracias";

	pila_t *pila = pila_crear();

	printf("\nAPILO CADENAS\n");

	bool ok;
	char* des;
	char* tope;
	int cmp;

	print_test("apilo la primer cadena", pila_apilar(pila, p1));
	print_test("apilo la segunda cadena",pila_apilar(pila, p2));

	tope = pila_ver_tope(pila);
	cmp = strcmp(tope, p2);
	ok = (cmp == 0);

	print_test("el tope es la última cadena apilada", ok);

	printf("\nDESAPILO\n");

	des = pila_desapilar(pila);
	cmp = strcmp(des, p2);
	ok = (cmp == 0);

	print_test("la primer cadena desapilada debe ser la segunda que apilé", ok);

	des = pila_desapilar(pila);

	print_test("la pila está vacía", pila_esta_vacia(pila));

	cmp = strcmp(des, p1);
	ok = (cmp == 0);

	print_test("la primer cadena apilada es la última en salir", ok);
	
	pila_destruir(pila);
	print_test("la pila fue destruida", true);

}

void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_apilar_pocos_elementos();
    prueba_pila_volumen();
    prueba_null();
    prueba_cadenas();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif