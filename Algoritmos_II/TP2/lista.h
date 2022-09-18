#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/
// Crea una nueva lista.
// Post: Devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un nuevo elemento en la lista, devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: el nuevo elemento insertado se encuentra al principio de la lista, y la lista tiene un elemento más.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un nuevo elemento en la lista, devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: el nuevo elemento insertado se encuentra al final de la lista, y la lista tiene un elemento más.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Si la lista tiene elementos devuelve el valor del primer elemento y luego lo borra, en caso de que esté vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento de la lista y se eliminó, si la lista no estaba vacía tiene un elemento
// menos.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista, si la lista tiene elementos devuelve ese valor y
// en caso de que esté vacía devuelve NULL .
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista en caso de que no esté vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista, si la lista tiene elementos devuelve ese valor y
// en caso de que esté vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del último elemento de la lista en caso de que no esté vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// void lista_destruir_csv(lista_t* lista, void (*destruir_dato)(char**));

/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera por la lista y le aplica la función visitar a cada uno de los elementos de la lista.
// El parámetro extra se pasa directamente a la función visitar.
// Pre: la lista fue creada.
// Post: los datos de la lista fueron modificados acorde a las especificaciones de la función visitar.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador para la lista recibida, devuelve NULL en caso de error.
// Pre: la lista fue creada.
// Post: devuelve un iterador colocado en la primer posicion.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al siguiente elemento de la lista, al llegar al final de la lista devuelve false, devuelve true en caso contrario.
// Pre: fueron creados el iterador y la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato del elemento al que apunta actualmente el iterador y devuelve NULL si el iterador está en el final.
// Pre: fueron creados el iterador y la lista.
// Post: se devolvió el elemento actual de la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true en caso de que el iterador haya llegado al final de la lista o false en caso contrario.
// Pre: fueron creados el iterador y la lista.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: fueron creados el iterador y la lista.
// Post: el iterador fue destruido.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la lista, devuelve false en caso de error.
// Pre: fueron creados el iterador y la lista.
// Post: el elemento insertado se encuentra entre el elemento actual al que apunta el iterador y su elemento anterior, el
// elemento actual pasa a ser el elemento insertado y aumenta la lista tiene un elemento más.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento actual donde está apuntando el iterador y lo devuelve, devuelve NULL en el caso de 
// estar en el final de la lista.
// Pre: fueron creados el iterador y la lista.
// Post: se devuelve el valor del elemento borrado, en caso de no estar vacía la lista tiene un elemento menos.
// El elemento actual del iterador pasa a ser el siguiente del elemento borrado.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);


#endif  // LISTA_H
