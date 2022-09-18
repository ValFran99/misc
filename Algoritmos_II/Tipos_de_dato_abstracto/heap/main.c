#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
void pruebas_heap_catedra(void);
void pruebas_heap_volumen_catedra(size_t, bool);
/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("~~~ PRUEBAS ESTUDIANTE ~~~\n");
        pruebas_heap_estudiante();
    }
    #ifdef CORRECTOR
        if (argc > 1) {
            // Asumimos que nos están pidiendo pruebas de volumen.
            long largo = strtol(argv[1], NULL, 10);
            pruebas_heap_volumen_catedra((size_t) largo, false);
            return 0;
        }
        printf("\n~~~ PRUEBAS CÁTEDRA ~~~\n");
        pruebas_heap_catedra();
    #endif
    return failure_count() > 0;
}