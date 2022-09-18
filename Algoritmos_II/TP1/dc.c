#include "calc_helper.h"
#include "strutil.h"
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define BASE_MINIMA_LOG 2

bool obtener_operandos(pilanum_t* pila, struct calc_token tok, calc_num* op1, calc_num* op2, calc_num* op3){

	if(tok.oper.num_operandos == 2){
		if(!desapilar_num(pila, op2) || !desapilar_num(pila, op1)){
			return true;
		}
	}
	else if(tok.oper.num_operandos == 1){
		if(!desapilar_num(pila, op1)){
			return true;
		}
	} else{
		if(!desapilar_num(pila, op3) || !desapilar_num(pila, op2) || !desapilar_num(pila, op1)){
			return true;
		}
	}
	return false;
}

bool obtener_resultado(struct calc_token tok, calc_num op1, calc_num op2, calc_num op3, calc_num* result){

	switch(tok.oper.op){
		case OP_ADD:
			*result =  op1 + op2;
			break;
		case OP_SUB:
			*result = op1 - op2;
			break;
		case OP_MUL:
			*result =  op1 * op2;
			break;
		case OP_DIV:
			if(op2 == 0){
				return true;
			}
			*result = op1 / op2;
			break;

		case OP_POW:
			if(op2 < 0){
				return true;
			}
			double aux;
			aux = pow((double) op1, (double) op2);
			*result = (calc_num) aux;
			break;

		case OP_LOG:
			if(op2 < BASE_MINIMA_LOG){
				return true;
			}

			double aux_1; double aux_2; double result_aux;

			aux_1 = log10((double) op1);
			aux_2 = log10((double) op2);

			result_aux = aux_1 / aux_2;

			*result = (calc_num) result_aux;

			break;

		case OP_RAIZ:
			if(op1 < 0){
				return true;
			}
			double aux_3;
			aux_3 = sqrt((double) op1);
			*result = (calc_num) aux_3;
			break;
			
		case OP_TERN:
			*result = op1 ? op2:op3;
			break;
	}
	return false;
}

void calc_polaca(){
	char* linea = NULL;
	size_t len = 0;

	pilanum_t* pila = pilanum_crear();
	if(pila == NULL) return;

	struct calc_token tok;
	while(getline(&linea,&len,stdin) != -1){
		
		if(strcmp(linea, "\n") == 0) continue;

		char** palabras = dc_split(linea);
		bool err = false;
		if(palabras == NULL) continue;

		for(int i = 0; palabras[i] != NULL; i++){

			if(!calc_parse(palabras[i], &tok)){
				err = true;
				break;
			}
			else if(tok.type == TOK_NUM){
				apilar_num(pila, tok.value);
			}
			else if(tok.type == TOK_OPER){
					
				calc_num op1;
				calc_num op2;
				calc_num op3;
				
				if(obtener_operandos(pila, tok, &op1, &op2, &op3)){
					err = true;
					break;
				}

				calc_num result = 0;

				if(obtener_resultado(tok, op1, op2, op3, &result)){
					err = true;
					break;
				}

				apilar_num(pila, result);
			}
		}

		free_strv(palabras);

		calc_num imprimir;
		calc_num trash;

		if((desapilar_num(pila, &imprimir) && !pila_esta_vacia(pila)) || err){
			printf("ERROR\n");
			err = false;
		} else{
			printf("%ld\n", imprimir);
		}

		while(desapilar_num(pila, &trash)){
			continue;
		}
	}

	free(linea);
	pilanum_destruir(pila);
}

int main(){

	calc_polaca();

	return 0;
}