#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#define TAM_INICIAL 10
#define REDIMENSION 2
#define FACT_ACHICAR 4

struct heap{
	void** datos;
	size_t cap;
	size_t cant;
	cmp_func_t cmp;
	void* extra_data;
};

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t *heap_crear(cmp_func_t cmp, void* extra_data){

	heap_t* heap = malloc(sizeof(heap_t));
	if(heap == NULL) return NULL;

	void** datos = malloc(sizeof(void*) * TAM_INICIAL);
	if(datos == NULL){
		free(heap);
		return NULL;
	} 

	heap->datos = datos;
	heap->cap = TAM_INICIAL;
	heap->cant = 0;
	heap->cmp = cmp;
	if(extra_data){
		heap->extra_data = extra_data;
	} else{
		heap->extra_data = NULL;
	}
	return heap;
}

bool heap_redimensionar(heap_t* heap, size_t tam){

	void** datos_nuevo = realloc(heap->datos, tam * sizeof(void*));
	if(datos_nuevo == NULL) return false;

	heap->datos = datos_nuevo;
	heap->cap = tam;
	return true;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){

	if(destruir_elemento){
		for(int i = 0; i < heap->cant; i++){
			destruir_elemento(heap->datos[i]);
		}
	}

	free(heap->datos);
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap){
	return heap->cant;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap){
	return heap->cant == 0;
}

void swap_heap(void *arreglo[], size_t i_hijo, size_t i_padre){
	
	void* valor_padre = arreglo[i_padre];
	arreglo[i_padre] = arreglo[i_hijo];
	arreglo[i_hijo] = valor_padre;
}

void upheap(void *arreglo[], size_t i_actual, cmp_func_t cmp, void* extra_data){

	if(i_actual == 0) return;

	size_t i_padre = (i_actual - 1) / 2;

	int cmp_result = cmp(arreglo[i_actual], arreglo[i_padre], extra_data);

	if(cmp_result <= 0) return;

	swap_heap(arreglo, i_actual, i_padre);
	upheap(arreglo, i_padre, cmp, extra_data);
}

/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t *heap, void *elem){
	
	if(heap->cant + 1 >= heap->cap){
		if(!heap_redimensionar(heap, heap->cap * REDIMENSION)) return false;
	}
	heap->datos[heap->cant] = elem;
	upheap(heap->datos, heap->cant, heap->cmp, heap->extra_data);
	heap->cant++;
	return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void downheap(void *arreglo[], size_t i_actual, size_t cant, cmp_func_t cmp, void* extra_data){

	if(i_actual >= cant) return;

	size_t hijo_max;

	size_t hijo_izq = 2*i_actual + 1;
	if(hijo_izq >= cant) return;

	size_t hijo_der = 2*i_actual + 2;
	if(hijo_der >= cant){
		hijo_max = hijo_izq;
	} else{
		hijo_max = (cmp(arreglo[hijo_izq], arreglo[hijo_der], extra_data) >= 0)? hijo_izq:hijo_der;	
	}

	int cmp_result = cmp(arreglo[hijo_max], arreglo[i_actual], extra_data);

	if(cmp_result <= 0) return;

	swap_heap(arreglo, hijo_max, i_actual);
	downheap(arreglo, hijo_max, cant, cmp, extra_data);
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void *heap_desencolar(heap_t *heap){

	if(heap_esta_vacio(heap)) return NULL;
	heap->cant--;
	if(heap->cant < heap->cap / FACT_ACHICAR){
		if(!heap_redimensionar(heap, heap->cap / REDIMENSION)) return NULL;
	}

	void* valor = heap->datos[0];
	swap_heap(heap->datos, heap->cant, 0);
	downheap(heap->datos, 0, heap->cant, heap->cmp, heap->extra_data);
	return valor;
} 


void heapify(void *arreglo[], size_t i_actual, size_t cant, cmp_func_t cmp, void* extra_data){
   
    downheap(arreglo, i_actual, cant, cmp, extra_data); //i_actual arranca en el ultimo     

	if(i_actual <= 0) return;

    heapify(arreglo, i_actual-1, cant, cmp, extra_data);

}

/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp, void* extra_data){
	
	heap_t* heap = heap_crear(cmp, extra_data);

	for(int i = 0; i < n; i++){
		heap_encolar(heap, arreglo[i]);
	}	

	return heap;
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */ 

void wrapper_heap_sort(void *elementos[], size_t i_actual, size_t cant, cmp_func_t cmp, void* extra_data){

    if(i_actual == 0) return;

    swap_heap(elementos, 0, i_actual);

    cant--;

    downheap(elementos,  0, cant, cmp, extra_data);

    wrapper_heap_sort(elementos, cant-1, cant, cmp, extra_data);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp, void* extra_data){

	heapify(elementos, cant-1, cant, cmp, extra_data);

	wrapper_heap_sort(elementos, cant-1, cant, cmp, extra_data);

	
}
