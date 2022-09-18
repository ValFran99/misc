#include "testing.h"
#include "lista.h"
#include "pila.h"

#include <stdio.h>
#include <stdlib.h>

static void pruebas_lista_vacia(void){

	printf("\nINICIO DE PRUEBAS CON LISTA VACÍA\n");

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);

	print_test("la lista está vacía", lista_esta_vacia(lista));
	print_test("no debe tener primero", lista_ver_primero(lista) == NULL);
	print_test("tampoco debe tener último", lista_ver_ultimo(lista) == NULL);
	print_test("no debería poder borrar nada, ya que está vacía", lista_borrar_primero(lista) == NULL);
	print_test("su largo debe ser cero", lista_largo(lista) == 0);

	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);
}

static void pruebas_pocos_punteros(void){

	printf("\nINICIO DE PRUEBAS CON POCOS PUNTEROS\n");
	lista_t* lista = lista_crear();
	print_test("se creó la lista", lista != NULL);

	int i = 5;
	int j = 10;
	int k = 12;
	int l = 30;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;

	print_test("la lista se creó vacía", lista_esta_vacia(lista));
	print_test("el largo de la lista es cero", lista_largo(lista) == 0);
	
	printf("\nInserto los elementos\n");

	print_test("inserto el primer elemento al principio", lista_insertar_primero(lista, ele1));
	print_test("ahora al lista no está vacía", !lista_esta_vacia(lista));
	print_test("su largo debe ser 1", lista_largo(lista) == 1);
	print_test("al tener un elemento, su primero ahora debe ser ese", lista_ver_primero(lista) == ele1);
	print_test("y como solo tiene un elemento, su último también debe ser el elemento insertado", lista_ver_ultimo(lista) == ele1);

	print_test("inserto otro elemento al principio", lista_insertar_primero(lista, ele2));
	print_test("la lista sigue sin estar vacía", !lista_esta_vacia(lista));
	print_test("su largo ahora debe ser 2", lista_largo(lista) == 2);
	print_test("el primer elemento de la lista debe ser el último insertado", lista_ver_primero(lista) == ele2);
	print_test("pero el último elemento no debe haber cambiado", lista_ver_ultimo(lista) == ele1);

	print_test("inserto otro elemento al final", lista_insertar_ultimo(lista, ele3));
	print_test("la lista sigue sin estar vacía", !lista_esta_vacia(lista));
	print_test("su largo ahora debe ser 3", lista_largo(lista) == 3);
	print_test("el primer elemento de la lista no debe haber cambiado", lista_ver_primero(lista) == ele2);
	print_test("pero el último si se debe actualizar", lista_ver_ultimo(lista) == ele3);

	printf("\nElimino los elementos\n");

	print_test("borro el primer elemento de la lista, y es el último primero que inserté", lista_borrar_primero(lista) == ele2);
	print_test("el largo de la lista disminuyó a 2", lista_largo(lista) == 2);
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("se actualizó el primero al siguiente elemento", lista_ver_primero(lista) == ele1);
	print_test("el último de la lista no se modificó", lista_ver_ultimo(lista) == ele3);

	print_test("borro el siguiente primero de la lista", lista_borrar_primero(lista) == ele1);
	print_test("el largo de la lista disminuyó a 1", lista_largo(lista) == 1);
	print_test("la lista sigue sin estar vacía", !lista_esta_vacia(lista));
	print_test("se actualizó el primero al siguiente elemento", lista_ver_primero(lista) == ele3);
	print_test("al tener un solo elemento, el primer elemento también es el último", lista_ver_ultimo(lista) == ele3);

	print_test("borro el último elemento de la lista", lista_borrar_primero(lista) == ele3);
	print_test("el largo de la lista ahora es 0", lista_largo(lista) == 0);
	print_test("por ende la lista está vacía", lista_esta_vacia(lista));
	print_test("al estar vacía, no debo tener un primero", lista_ver_primero(lista) == NULL);
	print_test("como tampoco un último", lista_ver_ultimo(lista) == NULL);
	print_test("no debería poder seguir borrando", lista_borrar_primero(lista) == NULL);

	printf("\nInsertar al final en una lísta vacía\n\n");

	print_test("insertar un elemento al final debería ser lo mismo que insertar un elemento al principio", lista_insertar_ultimo(lista, ele4));
	print_test("el largo de la lista debe ser 1", lista_largo(lista) == 1);
	print_test("la lista no debe estar vacía", !lista_esta_vacia(lista));
	print_test("el primer elemento debe ser el que inserté", lista_ver_primero(lista) == ele4);
	print_test("el último elemento también debe ser el mismo", lista_ver_ultimo(lista) == ele4);

	print_test("el elemento borrado debe ser el mismo que ingresé", lista_borrar_primero(lista) == ele4);
	print_test("la lista vuelve a estar vacía", lista_esta_vacia(lista));
	print_test("su largo debe ser 0", lista_largo(lista) == 0);

	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);

}

static void pruebas_null(void){

	printf("\nINICIO DE PRUEBAS CON NULLs\n");

	lista_t* lista = lista_crear();

	print_test("se creó la lista", lista != NULL);

	printf("\nInicia la inserción de NULLs\n");

	print_test("inserto el primer NULL", lista_insertar_primero(lista, NULL));
	print_test("su primer elemento es NULL", lista_ver_primero(lista) == NULL);
	print_test("su último elemento también", lista_ver_ultimo(lista) == NULL);
	print_test("pero la lista no está vacía", !lista_esta_vacia(lista));
	print_test("y su largo es 1", lista_largo(lista) == 1);

	print_test("inserto otro NULL al principio", lista_insertar_primero(lista, NULL));
	print_test("el primer elemento sigue siendo NULL", lista_ver_primero(lista) == NULL);
	print_test("el último también es NULL", lista_ver_ultimo(lista) == NULL);
	print_test("la lista sigue sin estar vacía", !lista_esta_vacia(lista));
	print_test("y su largo es de 2", lista_largo(lista) == 2);

	print_test("inserto otro NULL al final", lista_insertar_ultimo(lista, NULL));
	print_test("el último elemento sigue siendo NULL", lista_ver_ultimo(lista) == NULL);
	print_test("la lista sigue sin estar vacía", !lista_esta_vacia(lista));
	print_test("y su largo es de 3", lista_largo(lista) == 3);

	printf("\nInicia la eliminación de los NULLs\n");

	print_test("borro el primer NULL", lista_borrar_primero(lista) == NULL);
	print_test("el primero sigue siendo NULL", lista_ver_primero(lista) == NULL);
	print_test("así como el último", lista_ver_ultimo(lista) == NULL);
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("su largo disminuyó a 2", lista_largo(lista) == 2);

	print_test("borro el siguiente NULL", lista_borrar_primero(lista) == NULL);
	print_test("la lista sigue sin estar vacía", !lista_esta_vacia(lista));
	print_test("su largo disminuyó a 1", lista_largo(lista) == 1);

	print_test("borro el último NULL", lista_borrar_primero(lista) == NULL);
	print_test("el primero es NULL", lista_ver_primero(lista) == NULL);
	print_test("el último también", lista_ver_ultimo(lista) == NULL);
	print_test("pero ahora sí la lista está vacía", lista_esta_vacia(lista));
	print_test("y su largo es 0", lista_largo(lista) == 0);

	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);

}

static void prueba_volumen(void){

	printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

	lista_t* lista = lista_crear();

	print_test("la lista fue creada", lista != NULL);

	int v = 12;
	int* p = &v;
	int* vector[1000];

	for(int i = 0; i < 1000; i++){
		vector[i] = p++;
	}
	
	printf("\nInserto al final 1000 elementos\n");

	print_test("inserto el primer elemento", lista_insertar_ultimo(lista, vector[0]));
	print_test("se actualizó el primer elemento", lista_ver_primero(lista) == vector[0]);
	print_test("se actualizó el último elemento", lista_ver_ultimo(lista) == vector[0]);

	bool ok_ult = true;
	bool ok_prim = true;

	for(int i = 1; i < 1000; i++){
		lista_insertar_ultimo(lista, vector[i]);
		ok_ult &= lista_ver_ultimo(lista) == vector[i];
		ok_prim &= lista_ver_primero(lista) == vector[0];
	}

	print_test("verifico que siempre se actualizó el último elemento", ok_ult);
	print_test("y que el primero solo se actualizó al primer elemento", ok_prim);
	print_test("su largo debe ser 1000", lista_largo(lista) == 1000);

	printf("\nAhora los borro\n");

	int* borrado;
	bool ok_borr = true;

	for(int i = 0; i < 999; i++){
		borrado = lista_borrar_primero(lista);
		ok_borr &= borrado == vector[i];
		ok_prim &= borrado != lista_ver_primero(lista);
		ok_ult &= lista_ver_ultimo(lista) == vector[999];
	}

	print_test("verifico que el primer elemento se fue actualizando", ok_prim);
	print_test("y que el último elemento no cambió hasta que quede un solo elemento", ok_ult);
	print_test("y que los elementos mantuvieron el orden con el cual fueron insertados", ok_borr);
	print_test("el largo de la lista debe ser 1", lista_largo(lista) == 1);

	print_test("borro el último elemento", lista_borrar_primero(lista) == vector[999]);
	print_test("el primer elemento ahora debe ser NULL", lista_ver_primero(lista) == NULL);
	print_test("así como el último", lista_ver_ultimo(lista) == NULL);
	print_test("y su largo debe ser cero", lista_largo(lista) == 0);

	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);

}

static void prueba_destruir_lista_vacia_free(void){

	printf("\nINICIO LA PRUEBA DESTRUIR LISTA VACÍA CON FREE\n");
	lista_t* lista = lista_crear();

	print_test("la lista fue creada", lista != NULL);

	lista_destruir(lista, free);
	print_test("la lista fue destruida", true);
}

static void prueba_destruir_lista_no_vacia(void){

	printf("\nINICIO DE LA PRUEBA DESTRUIR LISTA NO VACÍA\n");

	int i = 5;
	int j = 10;
	int k = 12;
	int l = 30;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;

	lista_t* lista = lista_crear();

	print_test("la lista fue creada", lista != NULL);

	print_test("inserto el primer elemento", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el segundo elemento", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el tercer elemento", lista_insertar_ultimo(lista, ele3));
	print_test("inserto el cuarto elemento", lista_insertar_ultimo(lista, ele4));
	print_test("su largo es 4", lista_largo(lista) == 4);

	printf("\nAhora destruyo la lista\n");
	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);

}

static void prueba_destruir_lista_sin_destruir_datos(void){

	printf("\nINICIO DE LA PRUEBA DESTRUIR LISTA SIN DESTRUIR SUS DATOS\n");

	int* ele1 = malloc(sizeof(int));
	int* ele2 = malloc(sizeof(int));
	int* ele3 = malloc(sizeof(int));

	*ele1 = 2;
	*ele2 = 3;
	*ele3 = 10;

	lista_t* lista = lista_crear();

	print_test("se creó la lista", lista != NULL);

	print_test("inserto el primer elemento", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el segundo elemento", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el tercer elemento", lista_insertar_ultimo(lista, ele3));

	lista_destruir(lista, NULL);

	print_test("la lista fue destruida sin destruir los datos", true);

	free(ele3);
	free(ele2);
	free(ele1);

	print_test("y pude liberar los datos individualmente", true);

}

static void prueba_destruir_lista_free(void){

	printf("\nINICIO DE LA PRUEBA DESTRUIR LISTA CON FREE\n");

	int* ele1 = malloc(sizeof(int));
	int* ele2 = malloc(sizeof(int));
	int* ele3 = malloc(sizeof(int));

	*ele1 = 2;
	*ele2 = 3;
	*ele3 = 10;

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);

	print_test("inserto el primer elemento", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el segundo elemento", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el tercer elemento", lista_insertar_ultimo(lista, ele3));

	lista_destruir(lista, free);

	print_test("la lista fue destruida junto con sus datos", true);

}

void destruir_pila_wrapper(void* pila){
	pila_destruir(pila);
}

static void prueba_insertar_y_destruir_pilas(void){

	printf("\nINICIO DE PRUEBA INSERTAR Y DESTRUIR PILAS\n");

	lista_t* lista = lista_crear();

	print_test("la lista fue creada", lista != NULL);

	printf("Creo varias pilas\n");
	pila_t* pila_1 = pila_crear();
	print_test("se creó la pila 1", pila_1 != NULL);
	pila_t* pila_2 = pila_crear();
	print_test("se creó la pila 2", pila_2 != NULL);
	pila_t* pila_3 = pila_crear();
	print_test("se creó la pila 3", pila_3 != NULL);

	printf("\nApilo algunos elementos en cada una\n");

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

	print_test("inserto la pila 1", lista_insertar_ultimo(lista, pila_1));
	print_test("inserto la pila 2", lista_insertar_ultimo(lista, pila_2));
	print_test("inserto la pila 3", lista_insertar_ultimo(lista, pila_3));
	print_test("ver primero devuelve la primer pila", lista_ver_primero(lista) == pila_1);
	print_test("ver último devuelve la última pila", lista_ver_ultimo(lista) == pila_3);
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("y su largo debe ser 3", lista_largo(lista) == 3);

	lista_destruir(lista, destruir_pila_wrapper);

	print_test("se destruyó la lista junto con las pilas", true);
}

bool sumar_elementos(void* elem, void* extra){

	int *sumatoria = extra;
	*sumatoria += *(int*) elem;
	return true;
}

static void prueba_iterador_interno_final(void){

	printf("\nINICIO DE PRUEBA ITERAR TODO LA LISTA CON EL ITERADOR INTERNO\n");

	lista_t* lista = lista_crear();

	print_test("la lista fue creada", lista != NULL);

	int i = 10;
	int j = 5;
	int k = 15;
	int l = 20;
	int m = 30;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;
	int* ele5 = &m;

	int res_esperado = 80;
	int sum = 0;

	print_test("inserto el ele1", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el ele2", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el ele3", lista_insertar_ultimo(lista, ele3));
	print_test("inserto el ele4", lista_insertar_ultimo(lista, ele4));
	print_test("inserto el ele5", lista_insertar_ultimo(lista, ele5));

	lista_iterar(lista, sumar_elementos, &sum);

	print_test("la sumatoria da lo esperado", res_esperado == sum);

	lista_destruir(lista, NULL);

	print_test("la lista fue destruida", true);
}

/* Cuenta la cantidad de números pares hasta que se encuentra con un impar */
bool hasta_impar(void* dato, void* extra){

	int* i = extra;
	if(*(int*) dato % 2 == 0){
		++(*i);
		return true;
	}
	return false;
}

static void prueba_iterador_interno_corte(void){

	printf("\nINICIO DE PRUEBA ITERAR HASTA CIERTO PUNTO\n");

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);

	int i = 10;
	int j = 12;
	int k = 16;
	int l = 25;
	int m = 32;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;
	int* ele5 = &m;

	int conteo_esperado = 3;
	int cont = 0;

	print_test("inserto el ele1", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el ele2", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el ele3", lista_insertar_ultimo(lista, ele3));
	print_test("inserto el ele4", lista_insertar_ultimo(lista, ele4));
	print_test("inserto el ele5", lista_insertar_ultimo(lista, ele5));

	lista_iterar(lista, hasta_impar, &cont);

	print_test("el conteo esperado es el mismo que el resultante", conteo_esperado == cont);

	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);
}

static void prueba_iterador_externo_lista_vacia(void){


	printf("\nINICIO DE PRUEBA ITERADOR EXTERNO CON LISTA VACÍA\n");
	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("se creó un iterador en una lista vacía", iter != NULL);

	print_test("no debo poder avanzar", lista_iter_avanzar(iter) == false);
	print_test("ver actual debe devolver NULL", lista_iter_ver_actual(iter) == NULL);
	print_test("si la lista está vacía, el iterador está al final", lista_iter_al_final(iter));
	print_test("si la lista está vacía, borrar un elemento me devuelve NULL", lista_iter_borrar(iter) == NULL);

	lista_iter_destruir(iter);
	print_test("el iterador fue destruido", true);
	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);

}

static void prueba_iterador_externo_insertar_extremos(void){

	printf("\nINICIO DE PRUEBAS DE INSERTAR EXTREMOS CON ITERADOR EXTERNO\n");

	int i = 10;
	int j = 12;
	int k = 16;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);
	lista_iter_t* iter = lista_iter_crear(lista);

	print_test("se creó un iterador en una lista vacía", iter != NULL);

	printf("\nInserto en una lista vacía\n");

	print_test("inserto un elemento", lista_iter_insertar(iter, ele1));
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("efectivamente se cambió el primer elemento de la lista", lista_ver_primero(lista) == ele1);
	print_test("así como el último", lista_ver_ultimo(lista) == ele1);
	print_test("y su largo es de 1", lista_largo(lista) == 1);

	print_test("ver actual debe devolver ese elemento", lista_iter_ver_actual(iter) == ele1);
	print_test("el iterador no debe estar al final", !lista_iter_al_final(iter));

	printf("\nInserto al principio\n");

	print_test("Inserto otro elemento con el iterador al principio", lista_iter_insertar(iter, ele2));
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("el primer elemento de la lista debe haber cambiado", lista_ver_primero(lista) == ele2);
	print_test("y el largo de la lista debe ser de 2", lista_largo(lista) == 2);
	print_test("pero el último elemento no cambió", lista_ver_ultimo(lista) == ele1);

	print_test("ver actual me debe devolver ese elemento", lista_iter_ver_actual(iter) == ele2);
	print_test("y no debo estar al final", !lista_iter_al_final(iter));

	printf("\nInserto al final\n");

	print_test("avanzo", lista_iter_avanzar(iter));
	print_test("avanzo", lista_iter_avanzar(iter));
	print_test("ahora estoy al final de la lista", lista_iter_al_final(iter));
	print_test("y no puedo seguir avanzando", !lista_iter_avanzar(iter));
	print_test("y ver actual devuelve NULL", lista_iter_ver_actual(iter) == NULL);

	print_test("inserto un elemento al final", lista_iter_insertar(iter, ele3));
	print_test("el último elemento de la lista debe haber cambiado", lista_ver_ultimo(lista) == ele3);
	print_test("el primer elemento no debe haber cambiado", lista_ver_primero(lista) == ele2);
	print_test("el largo de la lista debe ser 3", lista_largo(lista) == 3);

	print_test("ya no estoy parado al final de la lista", !lista_iter_al_final(iter));
	print_test("ver actual debe devolver el elemento que inserté", lista_iter_ver_actual(iter) == ele3);
	print_test("puedo avanzar una vez más", lista_iter_avanzar(iter));
	print_test("ahora estoy al final de la lista", lista_iter_al_final(iter));

	lista_iter_destruir(iter);
	print_test("el iterador fue destruido", true);
	lista_destruir(lista, NULL);
	print_test("la lista fue destruida", true);
}

static void prueba_iterador_externo_insertar_medio(void){

	printf("\nINICIO DE PRUEBA INSERTAR EN EL MEDIO CON UN ITERADOR EXTERNO\n");

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);

	int i = 5;
	int j = 13;
	int k = 16;
	int l = 12;
	int m = 72;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;
	int* ele5 = &m;

	int* vector[] = {ele4, ele2, ele3, ele1, ele5};

	print_test("inserto el elem4 en la lista", lista_insertar_ultimo(lista, ele4));
	print_test("inserto el elem2 en la lista", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el elem1 en la lista", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el elem5 en la lista", lista_insertar_ultimo(lista, ele5));

	lista_iter_t* iter_1 = lista_iter_crear(lista);
	print_test("se creó el iterador 1", iter_1 != NULL);

	bool ok_avanzar = true;

	while(lista_iter_ver_actual(iter_1) != ele1){
		ok_avanzar &= lista_iter_avanzar(iter_1);
	}

	print_test("pude avanzar hasta el elemento 1", ok_avanzar);
	print_test("verifico que no estoy al final", !lista_iter_al_final(iter_1));
	print_test("inserto el elemento 3 en esa posición", lista_iter_insertar(iter_1, ele3));

	lista_iter_destruir(iter_1);
	print_test("se destruyó el iterador 1", true);

	lista_iter_t* iter_2 = lista_iter_crear(lista);
	print_test("se creó el iterador 2", iter_2 != NULL);

	printf("\nVerifico que se insertó el elemento en la posición correspondiente\n");

	bool ok_orden = true;
	size_t indice = 0;

	while(!lista_iter_al_final(iter_2)){
		ok_orden &= vector[indice] == lista_iter_ver_actual(iter_2);
		indice++;
		ok_avanzar &= lista_iter_avanzar(iter_2);
	}

	print_test("pude avanzar hasta el final", ok_avanzar);
	print_test("estoy parado al final de la lista", lista_iter_al_final(iter_2));
	print_test("el elemento insertado se inserto correctamente", ok_orden);

	lista_iter_destruir(iter_2);
	print_test("se destruyó el iterador 2", true);
	lista_destruir(lista, NULL);
	print_test("se destruyó la lista", true);
}

static void prueba_iterador_externo_borrar_extremos(void){

	printf("\nINICIO DE PRUEBA BORRAR EXTREMOS CON ITERADOR EXTERNO\n");

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);

	int i = 50;
	int j = 12;
	int k = 17;
	int l = 120;
	int m = 8;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;
	int* ele5 = &m;

	print_test("inserto el elem4 en la lista", lista_insertar_ultimo(lista, ele4));
	print_test("inserto el elem2 en la lista", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el elem1 en la lista", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el elem5 en la lista", lista_insertar_ultimo(lista, ele5));
	print_test("inserto el elem3 en la lista", lista_insertar_ultimo(lista, ele3));

	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("se creó el iterador", iter != NULL);

	printf("\nBorro el primer elemento\n");

	print_test("está parado en la primera posición", lista_iter_ver_actual(iter) == lista_ver_primero(lista));
	print_test("borrar el primer elemento me devuelve ese elemento", lista_iter_borrar(iter) == ele4);
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("su largo pasa a ser 4", lista_largo(lista) == 4);
	print_test("el primer elemento se actualizó al siguiente de la lista", lista_ver_primero(lista) == ele2);
	print_test("el último elemento no se modificó", lista_ver_ultimo(lista) == ele3);
	print_test("el iterador está parado en el nuevo primer elemento", lista_iter_ver_actual(iter) == lista_ver_primero(lista));

	printf("\nBorro el último elemento\n");

	bool ok_avanzar = true;

	while(lista_iter_ver_actual(iter) != ele3){
		ok_avanzar &= lista_iter_avanzar(iter);
	}
	print_test("pude avanzar al último elemento", ok_avanzar);
	print_test("no estoy al final de la lista", !lista_iter_al_final(iter));

	print_test("borrar el último elemento me devuelve ese elemento", lista_iter_borrar(iter) == ele3);
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("el largo disminuyó a 3", lista_largo(lista) == 3);
	print_test("el primer elemento no se modificó", lista_ver_primero(lista) == ele2);
	print_test("pero sí se modificó el último, pasó a ser el anterior", lista_ver_ultimo(lista) == ele5);
	print_test("y el iterador pasa a estar parado al final", lista_iter_al_final(iter));

	lista_iter_destruir(iter);
	print_test("se destruyó el iterador", true);
	lista_destruir(lista, NULL);
	print_test("se destruyó la lista", true);
}

static void prueba_iterador_externo_borrar_medio(void){

	printf("\nINICIO DE PRUEBA BORRAR UN ELEMENTO INTERMEDIO CON ITERADOR EXTERNO\n");

	lista_t* lista = lista_crear();
	print_test("la lista fue creada", lista != NULL);

	int i = 50;
	int j = 12;
	int k = 17;
	int l = 120;
	int m = 80;

	int* ele1 = &i;
	int* ele2 = &j;
	int* ele3 = &k;
	int* ele4 = &l;
	int* ele5 = &m;

	print_test("inserto el elem4 en la lista", lista_insertar_ultimo(lista, ele4));
	print_test("inserto el elem2 en la lista", lista_insertar_ultimo(lista, ele2));
	print_test("inserto el elem1 en la lista", lista_insertar_ultimo(lista, ele1));
	print_test("inserto el elem5 en la lista", lista_insertar_ultimo(lista, ele5));
	print_test("inserto el elem3 en la lista", lista_insertar_ultimo(lista, ele3));

	int* esperado[] = {ele4, ele2, ele3};

	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("se creó el iterador 1", iter != NULL);

	bool ok_avanzar = true;

	while(lista_iter_ver_actual(iter) != ele1){
		ok_avanzar &= lista_iter_avanzar(iter);
	}

	printf("\nBorro dos elementos intermedios\n");

	print_test("pude avanzar a la posición del elem1", ok_avanzar);
	print_test("el iterador no está al final", !lista_iter_al_final(iter));
	print_test("borrar el elemento actual lo devuelve", lista_iter_borrar(iter) == ele1);
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("su largo disminuye a 4", lista_largo(lista) == 4);
	print_test("no se modificó el primero", lista_ver_primero(lista) == ele4);
	print_test("como tampoco el último", lista_ver_ultimo(lista) == ele3);
	print_test("ver actual me devuelve el siguiente elemento", lista_iter_ver_actual(iter) == ele5);

	print_test("borro el siguiente elemento", lista_iter_borrar(iter) == ele5);
	print_test("el largo de la lista disminuyó a 3", lista_largo(lista) == 3);
	print_test("otra vez, el primer elemento no se modificó", lista_ver_primero(lista) == ele4);
	print_test("como tampoco el último", lista_ver_ultimo(lista) == ele3);
	print_test("ver actual me devuelve el siguiente elemento", lista_iter_ver_actual(iter) == ele3);
	print_test("el iterador está en el último elemento, pero no al final", !lista_iter_al_final(iter));

	lista_iter_destruir(iter);
	print_test("destruyo el iterador 1", true);

	printf("\nVerifico que no están más en la lista\n");
	
	lista_iter_t* iter_2 = lista_iter_crear(lista);
	print_test("se creó el iterador 2", iter_2 != NULL);

	bool ok_orden = true;
	size_t indice = 0;

	while(!lista_iter_al_final(iter_2)){
		ok_orden &= esperado[indice] == lista_iter_ver_actual(iter_2);
		indice++;
		ok_avanzar &= lista_iter_avanzar(iter_2);
	}

	print_test("pude avanzar hasta el final", ok_avanzar);
	print_test("verifico que estoy al final", lista_iter_al_final(iter_2));
	print_test("los elementos borrados no están en la lista", ok_orden);

	lista_iter_destruir(iter_2);
	print_test("se destruyó el iterador 2", true);

	lista_destruir(lista, NULL);
	print_test("se destruyó la lista", true);
}

static void prueba_iterador_externo_insertar_borrar_NULL(void){

	printf("\nINICIO DE PRUEBA INSERTAR Y BORRAR NULL CON ITERADOR EXTERNO\n");

	lista_t* lista = lista_crear();
	print_test("se creó la lista", lista != NULL);

	print_test("la lista está vacía", lista_esta_vacia(lista));

	lista_iter_t* iter = lista_iter_crear(lista);

	printf("\nInserto dos NULLs\n");

	print_test("inserto un NULL con el iterador", lista_iter_insertar(iter, NULL));
	print_test("la lista no está vacía", !lista_esta_vacia(lista));
	print_test("su largo es 1", lista_largo(lista) == 1);
	print_test("ver actual me debe devolver NULL", lista_iter_ver_actual(iter) == NULL);
	print_test("pero el iterador no está al final de la lista", !lista_iter_al_final(iter));
	print_test("debo poder avanzar", lista_iter_avanzar(iter));
	print_test("ahora el iterador está al final de la lista", lista_iter_al_final(iter));

	print_test("inserto otro NULL", lista_iter_insertar(iter, NULL));
	print_test("el largo de la lista aumenta a 2", lista_largo(lista) == 2);
	print_test("ver actual devuelve NULL", lista_iter_ver_actual(iter) == NULL);
	print_test("pero el iterador no está al final de la lista", !lista_iter_al_final(iter));
	print_test("avanzo", lista_iter_avanzar(iter));
	print_test("ver actual devuelve NULL", lista_iter_ver_actual(iter) == NULL);
	print_test("pero esta vez el iterador si está al final de la lista", lista_iter_al_final(iter));


	lista_iter_destruir(iter);
	print_test("se destruyó el primer iterador", true);
	
	printf("\nBorro los NULLs\n");

	lista_iter_t* iter_2 = lista_iter_crear(lista);
	print_test("se creó el segundo iterador", iter_2 != NULL);

	print_test("borro el primer elemento, me devuelve NULL", lista_iter_borrar(iter_2) == NULL);
	print_test("el primero de la lista se actualizó a NULL", lista_ver_primero(lista) == NULL);
	print_test("pero no está vacía", !lista_esta_vacia(lista));
	print_test("el iterador no está en el final", !lista_iter_al_final(iter_2));
	print_test("borro el segundo NULL", lista_iter_borrar(iter_2) == NULL);
	print_test("ahora la lista sí está vacía", lista_esta_vacia(lista));
	print_test("ver actual devuelve NULL", lista_iter_ver_actual(iter_2) == NULL);
	print_test("pero ahora sí el iterador está al final", lista_iter_al_final(iter_2));
	print_test("como está al final no puede avanzar más", !lista_iter_avanzar(iter_2));
	print_test("y ya no puedo borrar nada más", lista_iter_borrar(iter_2) == NULL);

	lista_iter_destruir(iter_2);
	print_test("se destruyó el iterador 2", true);
	lista_destruir(lista, NULL);
	print_test("se destruyó la lista", true);
}

void pruebas_lista_estudiante(){

	printf("\nINICIO DE PRUEBAS ESTUDIANTE\n");

	pruebas_lista_vacia();
	pruebas_pocos_punteros();
	pruebas_null();
	prueba_volumen();
	prueba_destruir_lista_vacia_free();
	prueba_destruir_lista_no_vacia();
	prueba_destruir_lista_sin_destruir_datos();
	prueba_destruir_lista_free();
	prueba_insertar_y_destruir_pilas();
	prueba_iterador_interno_final();
	prueba_iterador_interno_corte();
	prueba_iterador_externo_lista_vacia();
	prueba_iterador_externo_insertar_extremos();
	prueba_iterador_externo_insertar_medio();
	prueba_iterador_externo_borrar_extremos();
	prueba_iterador_externo_borrar_medio();
	prueba_iterador_externo_insertar_borrar_NULL();

	printf("\nFIN DE PRUEBAS ESTUDIANTE\n");
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif