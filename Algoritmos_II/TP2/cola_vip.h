#ifndef COLA_VIP_H
#define COLA_VIP_H

#include <stdbool.h>
#include "heap.h"
#include <stddef.h>

typedef struct cola_vip cola_vip_t;

/*
Prototipo de función de comparación utilizada
Recibe dos punteros como datos a comparar y un parámetro extra opcional por si se necesita
información externa a los datos en sí
debe devolver:
    menor a 0  si  a < b
        0      si  a == b
    mayor a 0  si  a > b

*/



/*
Crea la cola_vip, recibe como parámetros la función de comparación a utilizar y
un parámetro opcional y extra que alberga información extra útil para la función
de comparación
Devuelve un puntero a una cola si la creación fue exitosa o NULL en caso
contrario
*/
cola_vip_t* cola_vip_crear(cmp_func_t cmp, void* extra_data);

/*
Destruye la cola_vip y toda la memoria reservada por ella, la función recibida es opcional
pero en caso de recibirla la utiliza para destruir los datos almacenados
*/
void cola_vip_destruir(cola_vip_t* cola_vip, void (*destruir_dato)(void *));

/*
Agrega el elemento recibido a la cola, puede ser NULL? (no lo probé, probar)
El valor booleano indica si el elemento a agregar tiene prioridad absoluta
con true o false en caso contrario
Devuelve true si la operación fue exitosa o false en caso contrario
Pre: la cola fue creada
Post: se agregó el nuevo elemento a la cola
*/
bool cola_vip_encolar(cola_vip_t* cola_vip, void* valor, bool vip);

/*
Elimina el elemento con máxima prioridad y lo devuelve
Si la cola está vacía devuelve NULL
Pre: la cola fue creada
Post: el elemento desencolado ya no se encuentra en la cola
*/
void* cola_vip_desencolar(cola_vip_t* cola_vip);


/*
Indica si la cola está vacía o no
Devuelve true si tiene 0 elementos o false en caso contrario
*/
bool cola_vip_esta_vacia(const cola_vip_t* cola_vip);


/*
Devuelve el elemento con la máxima prioridad sin eliminarlo de la cola
En caso de estar vacía devuelve NULL
Pre: la cola fue creada
*/
void* cola_vip_ver_primero(const cola_vip_t* cola_vip);

/*
Devuelve la cantidad de elementos que hay en la cola
*/
size_t cola_vip_cantidad(const cola_vip_t* cola_vip);

#endif  // COLA_VIP_H