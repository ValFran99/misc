#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 5

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};



/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/



//Auxiliares 

bool pila_redimensionar(pila_t *pila, size_t valor) {
    
    
    void** datos_nuevo = realloc(pila->datos, valor * sizeof(void*)); 
    if (datos_nuevo == NULL){
        return false;
    }
    pila->datos = datos_nuevo;
    pila->capacidad = valor; 
    return true;
     
    // Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
    // En toda otra situación significa que falló el realloc.
}



// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }

    pila->capacidad = CAPACIDAD_INICIAL;

    pila->cantidad = 0;

    pila->datos = malloc(pila->capacidad * sizeof(void*));
 
    return pila;
}


// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){ 

    free(pila->datos);
    
    free(pila);

}

// Devuelve verdadero si la pila no tiene elementos apilados, false en caso contrario.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){
    if (pila->cantidad == 0) {
        return true; 
    }

    return false;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor){

    if (pila->cantidad == pila->capacidad){
        pila_redimensionar(pila, (pila->capacidad)*2);
    }
    

    pila->datos[pila->cantidad] = valor;

    pila->cantidad++; 

    return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void *pila_ver_tope(const pila_t *pila){

    if (pila_esta_vacia(pila)){
        return NULL;
    }

    return pila->datos[(pila->cantidad)-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void *pila_desapilar(pila_t *pila){

    if (pila_esta_vacia(pila)){
        return NULL;
    }

    if (pila->cantidad <= (pila->capacidad)/4 && (pila->capacidad/2) >= CAPACIDAD_INICIAL){
        pila_redimensionar(pila, (pila->capacidad)/2);
    }
   
    pila->cantidad--;

    return pila->datos[pila->cantidad];
} 


// ...
