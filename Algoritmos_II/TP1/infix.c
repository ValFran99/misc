#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"

void imprimir_operador(struct calc_token* tok){

	printf("%c ", tok->oper.caracter);
	free(tok);
}

void procesar_token(pila_t* pila_op, struct calc_token* tok){

	struct calc_token* tok_salida;
	struct calc_token* tok_tope;

	if(tok->type == TOK_NUM){
		printf("%ld ", tok->value);
		free(tok);
	}
	else if(tok->type == TOK_OPER){

		tok_tope = pila_ver_tope(pila_op); 
		while(!pila_esta_vacia(pila_op) && tok_tope->type != TOK_LPAREN){

			if((tok_tope->oper.precedencia > tok->oper.precedencia) || ((tok_tope->oper.precedencia == tok->oper.precedencia) && (tok_tope->oper.asociatividad == ASSOC_LEFT))){
				tok_salida = pila_desapilar(pila_op);
				imprimir_operador(tok_salida);
				tok_tope = pila_ver_tope(pila_op);
				continue;
			}
			break;
		}

		if(!pila_apilar(pila_op, tok)){
			free(tok);
			return;
		}
	}
	else if(tok->type == TOK_LPAREN){
		if(!pila_apilar(pila_op, tok)){
			free(tok);
			return;
		}
	}
	else if(tok->type == TOK_RPAREN){

		tok_tope = pila_ver_tope(pila_op);

		while(!pila_esta_vacia(pila_op) && tok_tope->type != TOK_LPAREN){
			tok_salida = pila_desapilar(pila_op);
			imprimir_operador(tok_salida);
			tok_tope = pila_ver_tope(pila_op);
		}
		free(pila_desapilar(pila_op));
		free(tok);
	}
}

void infix(){

	char* linea = NULL;
	size_t len = 0;
	pila_t* pila_op = pila_crear();
	if(pila_op == NULL) return;

	while(getline(&linea,&len,stdin) != -1){

		if(strcmp(linea, "\n") == 0) continue;

		char** palabras = infix_split(linea);
		if(palabras[0] == NULL) continue;

		for(int i = 0; palabras[i] != NULL; i++){

			struct calc_token* tok = malloc(sizeof(struct calc_token));
			if(tok == NULL) return;

			if(!calc_parse(palabras[i], tok)){
				free(tok);
				break;
			}
			procesar_token(pila_op, tok);
		}

		free_strv(palabras);
		struct calc_token* tok_salida;

		while(!pila_esta_vacia(pila_op)){

			tok_salida = pila_desapilar(pila_op);
			imprimir_operador(tok_salida);
		}
		printf("\n");
	}
	free(linea);
	pila_destruir(pila_op);
}

int main(){

	infix();

	return 0;
}