#include <stdbool.h> 
#include <stdlib.h>

#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


/* Crea el Abb
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el Abb, si la clave ya se encuentra reemplaza su valor. De no poder
guardar devuelve false.
Pre: Se creó la estructura abb
Post: Se almacenó el par clave, dato*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

void *abb_borrar(abb_t *arbol, const char *clave);

void *abb_obtener(const abb_t *arbol, const char *clave);

bool abb_pertenece(const abb_t *arbol, const char *clave);

size_t abb_cantidad(abb_t *arbol);

void abb_destruir(abb_t *arbol);


struct abb_iter;
typedef struct abb_iter abb_iter_t;


/* ******************************************************************
 *                    		ITERADOR INTERNO
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

// void abb_in_order_rangos(abb_t* arbol, char* min, char* max, bool visitar(const char*, void*, void*), void *extra);

/* ******************************************************************
 *                    		ITERADOR EXTERNO
 * *****************************************************************/


abb_iter_t *abb_iter_in_crear(const abb_t *arbol, char* min, char* max);

bool abb_iter_in_avanzar(abb_iter_t *iter);

const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

bool abb_iter_in_al_final(const abb_iter_t *iter);

void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H