#include "testing.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LEN_VOLUMEN 2000


int comparar_len(const void *a, const void *b){
    char* a_str = (char*) a;
    char* b_str = (char*) b;

    size_t len_a = strlen(a_str);
    size_t len_b = strlen(b_str);

    if(len_a == len_b) return 0;
    return (len_a > len_b)? 1:-1;
}

int comparar_mayor(const void *a, const void *b){
    int a_int = *(int*) a;
    int b_int = *(int*) b;

    if(a_int == b_int) return 0;
    return (a_int > b_int)? 1:-1;
}

static void prueba_heap_vacio(){

    printf("\nINICIO PRUEBAS HEAP VACIO\n");

    heap_t* heap = heap_crear(comparar_len);

    print_test("se créo el heap", heap != NULL);
    print_test("el heap debe estar vacío", heap_esta_vacio(heap));
    print_test("no debe tener máximo", heap_ver_max(heap) == NULL);
    print_test("su cantidad debe ser 0", heap_cantidad(heap) == 0);
    print_test("no debo poder desencolar nada", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
    print_test("el heap fue destruido", true);

}

static void prueba_heap_encolar_cadenas(){

    printf("\nINICIO PRUEBAS HEAP ENCOLAR CADENAS\n");

    char *clave1 = "perro";
    char *clave2 = "gato";
    char *clave3 = "elefante";
    char* clave4 = "bee";

    heap_t* heap = heap_crear(comparar_len);

    print_test("se creó el heap", heap != NULL);

    print_test("encolo la primer cadena", heap_encolar(heap, clave1));
    print_test("el heap ya no está vacío", !heap_esta_vacio(heap));
    print_test("su cantidad es 1", heap_cantidad(heap) == 1);
    print_test("clave1 debe ser su máximo", heap_ver_max(heap) == clave1);

    print_test("encolo la siguiente cadena", heap_encolar(heap, clave2));
    print_test("el heap sigue sin estar vacío", !heap_esta_vacio(heap));
    print_test("su cantidad es 2", heap_cantidad(heap) == 2);
    print_test("su valor máximo no debe haber cambiado", heap_ver_max(heap) == clave1);

    print_test("encolo la siguiente cadena", heap_encolar(heap, clave3));
    print_test("su cantidad es 3", heap_cantidad(heap) == 3);
    print_test("su valor máximo ahora debe ser el recién encolado", heap_ver_max(heap) == clave3);

    print_test("encolo la última cadena", heap_encolar(heap, clave4));
    print_test("su cantidad es 4", heap_cantidad(heap) == 4);
    print_test("su máximo no debe haber cambiado", heap_ver_max(heap) == clave3);

    // Ahora los desencolo, deben ser devueltos en orden

    print_test("desencolo por primera vez, el valor devuelto debe ser el máximo", heap_desencolar(heap) == clave3);
    print_test("su cantidad es 3", heap_cantidad(heap) == 3);
    print_test("el máximo se debe haber actualizado al siguiente más grande", heap_ver_max(heap) == clave1);
    print_test("el heap no debe estar vacío", !heap_esta_vacio(heap));

    print_test("desencolo el siguiente elemento", heap_desencolar(heap) == clave1);
    print_test("su cantidad es 2", heap_cantidad(heap) == 2);
    print_test("el máximo debe ser el siguiente más grande", heap_ver_max(heap) == clave2);
    print_test("el heap no debe estar vacío", !heap_esta_vacio(heap));

    print_test("desencolo el siguiente elemento", heap_desencolar(heap) == clave2);
    print_test("su cantidad es 1", heap_cantidad(heap) == 1);
    print_test("el máximo debe ser el siguiente más grande", heap_ver_max(heap) == clave4);
    print_test("el heap no debe estar vacio", !heap_esta_vacio(heap));

    print_test("desencolo el último elemento", heap_desencolar(heap) == clave4);
    print_test("su cantidad es 0", heap_cantidad(heap) == 0);
    print_test("por ende el heap ahora está vacío", heap_esta_vacio(heap));
    print_test("no puedo seguir desencolando", heap_desencolar(heap) == NULL);
    print_test("y por ende no existe un máximo", heap_ver_max(heap) == NULL);

    heap_destruir(heap, NULL);
    print_test("el heap fue destruido", true);
}

int cmp(const void* a, const void* b){
    if(a == NULL && b == NULL) return 0;
    return (a > b)? 1:-1;
}

static void prueba_heap_null(){

    printf("\nINICIO PRUEBAS HEAP NULL\n");

    heap_t* heap = heap_crear(cmp);

    print_test("el heap fue creado", true);

    // encolo NULLs
    print_test("encolo el primer NULL", heap_encolar(heap, NULL));
    print_test("encolo el segundo NULL", heap_encolar(heap, NULL));
    print_test("encolo el tercer NULL", heap_encolar(heap, NULL));

    print_test("su cantidad es 3", heap_cantidad(heap) == 3);
    print_test("ver maximo devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("pero el heap no está vacío", !heap_esta_vacio(heap));

    print_test("desencolo el primer NULL, devuelve NULL", heap_desencolar(heap) == NULL);
    print_test("su cantidad es 2", heap_cantidad(heap) == 2);
    print_test("ver maximo sigue devolviendo NULL", heap_ver_max(heap) == NULL);
    print_test("pero el heap sigue sin estar vacío", !heap_esta_vacio(heap));

    print_test("desencolo el segundo NULL, devuelve NULL", heap_desencolar(heap) == NULL);
    print_test("el máximo sigue siendo NULL", heap_ver_max(heap) == NULL);
    print_test("y el heap sigue sin estar vacío", !heap_esta_vacio(heap));

    print_test("desencolo el tercer NULL", heap_desencolar(heap) == NULL);
    print_test("ahora ver max devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("pero ahora sí el heap está vacío", heap_esta_vacio(heap));
    print_test("su cantidad es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
    print_test("el heap fue destruido", true);
}

static void prueba_heap_destruir_no_vacio(){

    printf("\nINICIO PRUEBA HEAP DESTRUCCION NO VACIO\n");

    
    char* cad1 = "VA-11 HALL-A";
    char* cad2 = "elefante";
    char* cad3 = "UNIST";
    char* cad4 = "yabai";
    char* cad5 = "kaguya";

    char* cad1_dup = strdup(cad1);
    char* cad2_dup = strdup(cad2);
    char* cad3_dup = strdup(cad3);
    char* cad4_dup = strdup(cad4);
    char* cad5_dup = strdup(cad5);


    // encolo copias dinámicas de las cadenas

    heap_t* heap = heap_crear(comparar_len);

    print_test("el heap fue creado", heap != NULL);

    print_test("encolo la primer cadena", heap_encolar(heap, cad1_dup));
    print_test("encolo la segunda cadena", heap_encolar(heap, cad2_dup));
    print_test("encolo la tercer cadena", heap_encolar(heap, cad3_dup));
    print_test("encolo la cuarta cadena", heap_encolar(heap, cad4_dup));
    print_test("encolo la quinta cadena", heap_encolar(heap, cad5_dup));

    print_test("su cantidad es 5", heap_cantidad(heap) == 5);
    print_test("el heap no está vacío", !heap_esta_vacio(heap));
    print_test("ver maximo devuelve la cadena correspondiente", heap_ver_max(heap) == cad1_dup);

    heap_destruir(heap, free);

    print_test("el heap fue destruido", true);
}

static void prueba_heap_destruccion_sin_datos(){

    printf("\nINICIO PRUEBA DESTRUIR HEAP SIN DESTRUIR SUS DATOS\n");

    char* cad1 = "VA-11 HALL-A";
    char* cad2 = "elefante";
    char* cad3 = "UNIST";
    char* cad4 = "yabai";
    char* cad5 = "kaguya";

    char* cad1_dup = strdup(cad1);
    char* cad2_dup = strdup(cad2);
    char* cad3_dup = strdup(cad3);
    char* cad4_dup = strdup(cad4);
    char* cad5_dup = strdup(cad5);


    // encolo copias dinámicas de las cadenas

    heap_t* heap = heap_crear(comparar_len);

    print_test("el heap fue creado", heap != NULL);

    print_test("encolo la primer cadena", heap_encolar(heap, cad1_dup));
    print_test("encolo la segunda cadena", heap_encolar(heap, cad2_dup));
    print_test("encolo la tercer cadena", heap_encolar(heap, cad3_dup));
    print_test("encolo la cuarta cadena", heap_encolar(heap, cad4_dup));
    print_test("encolo la quinta cadena", heap_encolar(heap, cad5_dup));

    print_test("su cantidad es 5", heap_cantidad(heap) == 5);
    print_test("el heap no está vacío", !heap_esta_vacio(heap));
    print_test("ver maximo devuelve la cadena correspondiente", heap_ver_max(heap) == cad1_dup);

    heap_destruir(heap, NULL);
    print_test("se destruyó el heap sin destruir los datos que almacenaba", true);

    free(cad1_dup);
    free(cad2_dup);
    free(cad3_dup);
    free(cad4_dup);
    free(cad5_dup);

    print_test("destruyo los datos individualmente", true);
}

static void prueba_heap_destruir_estatico(){

    printf("\nINICIO PRUEBA DESTRUIR HEAP DE DATOS ESTATICOS\n");

    char* cad1 = "VA-11 HALL-A";
    char* cad2 = "elefante";
    char* cad3 = "UNIST";
    char* cad4 = "yabai";
    char* cad5 = "kaguya";

    heap_t* heap = heap_crear(comparar_len);

    print_test("el heap fue creado", heap != NULL);

    print_test("encolo la primer cadena", heap_encolar(heap, cad1));
    print_test("encolo la segunda cadena", heap_encolar(heap, cad2));
    print_test("encolo la tercer cadena", heap_encolar(heap, cad3));
    print_test("encolo la cuarta cadena", heap_encolar(heap, cad4));
    print_test("encolo la quinta cadena", heap_encolar(heap, cad5));

    heap_destruir(heap, NULL);

    print_test("el heap fue destruido", true);
}

int cmp_int(const void *a, const void *b){

    if(*(int*) a == *(int*) b) return 0;

    return (*(int*) a > *(int*) b)? 1:-1;
}

void swap(size_t *x, size_t *y) {
    
    size_t valor_x = *x;
    *x = *y;
    *y = valor_x;
}

void shuffle(size_t arr[], size_t len){

    for(size_t i = 0; i < len; i++){
        size_t i_rand = random() % len;
        swap(&arr[i], &arr[i_rand]);
    }
}

static void prueba_heap_volumen(){

    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

    size_t len = LEN_VOLUMEN;
    size_t arr_ordenado[len];
    size_t arr_random[len];
    size_t* arr_p[len];

    for(size_t i = 0, j = 2; i < len; i++, j += 3){
        arr_random[i] = j;
        arr_ordenado[i] = j;
    }
    
    shuffle(arr_random, len);

    for(int i = 0; i < len; i++){
        arr_p[i] = &arr_random[i];
    }

    heap_t* heap = heap_crear(cmp_int);

    bool ok_encolar = true;

    for(int i = 0; i < len; i++){
        ok_encolar &= heap_encolar(heap, arr_p[i]);
    }

    print_test("se encolaron todos los elementos", ok_encolar);
    print_test("la cantidad es la correcta", heap_cantidad(heap) == len);
    print_test("el heap no está vacío", !heap_esta_vacio(heap));

    print_test("el máximo elemento es el correspondiente", *(size_t*) heap_ver_max(heap) == arr_ordenado[len - 1]);

    bool ok_desencolar = true;

    for(size_t i = len - 1; !heap_esta_vacio(heap); i--){
        ok_desencolar &= *(size_t*) heap_desencolar(heap) == arr_ordenado[i];
        if(!ok_desencolar) break;
    }
    print_test("se desencolaron todos los elementos en el orden correcto", ok_desencolar);
    heap_destruir(heap, NULL);
    print_test("el heap fue destruido", true);
}

static void prueba_heap_arr2(){

    printf("\nINICIO DE PRUEBAS DE HEAP ARR2\n");

    size_t cant = 7;

    void *claves[] = {"voz", "perro", "cocodrilo", "gato", "UV", "patata", "mandril"};

    void *claves_o[] = {"cocodrilo", "mandril", "patata", "perro", "gato", "voz", "UV"};

    printf("\nPaso2\n");

    heap_t* heap = heap_crear_arr(claves, cant, comparar_len);

    printf("\nPaso3\n");

    print_test("se créo el heap", heap != NULL);
    print_test("el heap no debe estar vacío", !heap_esta_vacio(heap));
    print_test("debe tener máximo, osea el char mas largo", heap_ver_max(heap) == claves_o[0]);
    print_test("su cantidad debe ser 7", heap_cantidad(heap) == cant);
    
    
    for (int i = 0; i < 7; i++){
        //printf("\n%s datos\n", (char*) heap_ver_max(heap));
        print_test("Mi heap es correcto", heap_desencolar(heap) == claves_o[i]);
    }

    printf("\nPaso4\n");

    heap_destruir(heap, free);

    print_test("el heap fue destruido", true);


}


static void prueba_heap_arr(){

    printf("\nINICIO DE PRUEBAS DE HEAP ARR\n");

    size_t cant = 7;

    int num[] = {4, 1, 9, 2, 12, 5, 7};

    int hc[] = {12, 9, 7, 5, 4, 2, 1}; //Orden correcto a desencolar
  
    int j;

    void *numeros[cant], *heap_correcto[cant];
 
    for (j = 0; j < cant; j++){
        numeros[j] = &num[j];
        heap_correcto[j] = &hc[j];
    }

    heap_t* heap = heap_crear_arr(numeros, cant, comparar_mayor);

    print_test("se créo el heap", heap != NULL);
    print_test("el heap no debe estar vacío", !heap_esta_vacio(heap));
    print_test("debe tener máximo, osea 12", *(int*) heap_ver_max(heap) == 12);
    print_test("su cantidad debe ser 7", heap_cantidad(heap) == cant);
     
    
    for (int i = 0; i < 7; i++){
        print_test("Mi heap es correcto", *(int*) heap_desencolar(heap) == *(int*) heap_correcto[i]);
    }


    heap_destruir(heap, NULL);

    print_test("el heap fue destruido", true);

    printf("\nINICIO DE PRUEBAS DE HEAP ARR, CON ARR DINAMICO\n");

    void** numeros_d = malloc(sizeof(void*) * cant);
 
    for (j = 0; j < cant; j++){
        numeros_d[j] = &num[j];
    }

    heap_t* heap2 = heap_crear_arr(numeros_d, cant, comparar_mayor);

    printf("\nLibero arr\n");

    free(numeros_d);

    print_test("se créo el heap", heap2 != NULL);
    print_test("el heap no debe estar vacío", !heap_esta_vacio(heap2));
    print_test("debe tener máximo, osea 12", *(int*) heap_ver_max(heap2) == 12);
    print_test("su cantidad debe ser 7", heap_cantidad(heap2) == cant);
     
    
    for (int i = 0; i < 7; i++){
        print_test("Mi heap es correcto", *(int*) heap_desencolar(heap2) == *(int*) heap_correcto[i]);
    }

    heap_destruir(heap2, free);

    print_test("el heap fue destruido", true);

    printf("\nArr Dinamico, borrar heap no vacio\n");

    void** numeros_d2 = malloc(sizeof(void*) * cant);
 
    for (j = 0; j < cant; j++){
        numeros_d2[j] = &num[j];
    }

    heap_t* heap3 = heap_crear_arr(numeros_d2, cant, comparar_mayor);

    printf("\nLibero arr\n");

    free(numeros_d2); 

    print_test("el heap no debe estar vacío", !heap_esta_vacio(heap3));

    heap_destruir(heap3, NULL);

    print_test("el heap fue destruido", true);

}


static void prueba_heap_sort(){

    printf("\nINICIO DE PRUEBAS DE HEAP SORT\n");

    printf("\nPrueba1\n");

    size_t cant = 7;

    int num[] = {4, 1, 9, 2, 12, 5, 7};

    int nc[] = {1, 2, 4, 5, 7, 9, 12};

    int k;

    void *numeros[cant], *numeros_correcto[cant];
 
    for ( k = 0; k < cant; k++) {
        numeros[k] = &num[k];
        numeros_correcto[k] = &nc[k]; 
    }

    heap_sort(numeros, cant, comparar_mayor);

    for(int i = 0; i < cant; i++){
        //printf("\n numeros el valor es %d\n", *(int*) numeros[i]);
        print_test("el valor es el correcto", *(int*) numeros[i] == *(int*) numeros_correcto[i]);
    }

    printf("\nPrueba2\n");

    size_t cant2 = 13;

    int num2[] = {4, 1, 9, 2, 12, 5, 7, 14, 24, 10, 31, 45, 3};

    int l;

    void* numeros2[cant2];
    
    for ( l = 0; l < cant2; l++) {
      numeros2[l] = &num2[l]; 
    }


    heap_sort(numeros2, cant2, comparar_mayor);

    bool error = false;

    for(int j = 0; j < 12; j++){
        if(*(int*) numeros2[j] > *(int*) numeros2[j+1]){
            error = true;
        }
    }

    print_test("los valores fueron correctos", !error);

}

void pruebas_heap_estudiante(){

    prueba_heap_vacio();
    prueba_heap_encolar_cadenas();
    prueba_heap_null();
    prueba_heap_destruir_no_vacio();
    prueba_heap_destruccion_sin_datos();
    prueba_heap_destruir_estatico();
    prueba_heap_volumen();

    prueba_heap_arr2();
    prueba_heap_arr(); 
    prueba_heap_sort();
}



#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.


int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif