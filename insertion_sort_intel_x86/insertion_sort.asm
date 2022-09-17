
global main
extern puts
extern printf
extern gets
extern fopen
extern fread
extern fclose

section .data

	nombreArchivoNros 		db 		'entrada_nros_mayor_a_30',0
	modoArchivo 			db 		'rb',0

	cadAscendente			db 		'A',0
	cadDescendente			db 		'D',0

	longVector 				dq 		0

	contadorI 				db 		0
	contadorJ 				db 		0


	msjVector 				db 		'%hhi ',0
	msjFin 					db 		'',10,0
	msjIngOrden 			db 		'Ingrese A/D para ordenar de forma ascendente o descendente',0

	msjEstadoInicialVector 	db 		'El estado inicial del vector es:',10,0

	msjInicioWhile1 		db 		'Iniciando el ciclo de i menor a la longitud del vector',10,0
	msjInicioWhile2 		db 		'Iniciando el ciclo de j mayor a 0 y la condicion de ordenamiento',10,0
	msjValorAEvaluar 		db 		'El valor a evaluar es: %hhi',10,10,0
	msjOrdenoAscendentemente 	db 		'Como debo ordenar ascendentemente, comparo si el elemento anterior a este es más grande',10,0
	msjOrdenoDescendentemente 	db 		'Como debo ordenar descendentemente, comparo si el elemento anterior es más chico',10,0
	
	msjNoEstanOrdenadosAscendente db 		'Como %hhi >= %hhi, swapeo ambos valores',10,10,0
	msjNoEstanOrdenadosDescendente	db 		'Como %hhi <= %hhi, swapeo ambos valores',10,10,0

	msjFinWhile1 			db 		'Dado que i es mayor a la longitud del vector, llegué al final del algoritmo',10,0
	msjFinWhile2KMenorCero	db 		'Dado que j es menor a cero, el número está en la primera posición del vector y no hay número anterior con que comparar',10,0
	msjFinWhile2ElemOrdenados 	db 		'Dado que los dos elementos ya están en orden, no hago nada',10,0

	msjEstadoActual 		db 		'El estado actual del vector es: ',0
	msjEstadoPostSwap 		db  	'El estado post swap es: ',0

	msjVectorOrdenado 		db 		'El vector ordenado es:',0

	msjValoresASwapear	 	db  	'Los valores a swapear son: %hhi y %hhi',10,10,0

	msjErrOpen 				db 		'Error de apertura en el archivo de numeros',0
	msjArchivoVacio 		db 		'El archivo está vacío',0

section .bss

	registroNros			times 0 resb 	1
	numero 					resb 		 	1

	handleArchivoNros		resq			1

	vectorNros 				resb 			30
	ordenarAscODsc 			resb 			1
	bufferInputUser			resb 			30

	auxSwap 				resb 			1

	auxPrintf 				resb 			1
	auxPrintf2				resb 			1

	plusRsp 				resq 			1

section .text
main:

	
	mov 	rdi,nombreArchivoNros
	mov 	rsi,modoArchivo
	call 	fopen

	cmp 	rax,0
	jle 	errorApertura
	mov 	qword[handleArchivoNros],rax

	call 	pasarArchivoAVector

	mov 	rdi,[handleArchivoNros]
	call 	fclose

	cmp 	byte[longVector],0
	je 		archivoVacio

	call 	askUserOrdenAscendenteODescendente

	mov 	rdi,msjEstadoInicialVector
	call 	safePuts

	call 	imprimirVector

	call 	ordenarVector

	mov 	rdi,msjVectorOrdenado
	call 	puts

	call 	imprimirVector

	jmp 	fin

archivoVacio:

	mov 	rdi,msjArchivoVacio
	call 	puts

	jmp 	fin

errorApertura:

	mov 	rdi,msjErrOpen
	call 	puts

fin:
ret


;******************************************
; 			SUBRUTINAS INTERNAS
;******************************************

;***************** pasar archivo a vector *******************

pasarArchivoAVector:

	mov 	rcx,30
	mov 	rbx,0

leerSigNro:	

	push 	rcx

	mov 	rdi,registroNros
	mov 	rsi,1
	mov 	rdx,1
	mov 	rcx,[handleArchivoNros]
	call 	fread

	pop 	rcx

	cmp 	rax,0
	jle 	finLectura

	mov 	rax,0
	mov 	al,byte[numero]
	mov 	byte[vectorNros+rbx],al

	inc 	rbx
	add 	qword[longVector],1


	loop 	leerSigNro

finLectura:

ret
;**************** preguntar al user el método de ordenamiento *********************

askUserOrdenAscendenteODescendente:

	mov 	rdi,msjIngOrden
	call 	puts

	mov 	rdi,bufferInputUser
	call 	gets

	mov 	rbx,1

	mov 	al,[bufferInputUser + rbx]
	cmp 	al,0

	jne 	askUserOrdenAscendenteODescendente

	mov 	rcx,1
	lea 	rsi,[bufferInputUser]
	lea 	rdi,[cadAscendente]

	repe 	cmpsb

	je  	setearAscendente

	mov 	rcx,1
	lea 	rsi,[bufferInputUser]
	lea 	rdi,[cadDescendente]
	repe 	cmpsb

	je 		setearDescendente

	jmp 	askUserOrdenAscendenteODescendente	

setearAscendente:

	mov 	byte[ordenarAscODsc],'A'
	jmp 	finInput	

setearDescendente:

	mov 	byte[ordenarAscODsc],'D'

finInput:
ret

;******************** ordenar el vector en el lugar ***********************

ordenarVector:

	mov 	byte[contadorI],1

; ************* start while 1 *****************

	mov 	rdi,msjInicioWhile1
	call 	safePuts

while1:

	mov 	rax,0
	mov 	al,[contadorI]
	cmp 	al,[longVector]
	jge 	finWhile1


	mov 	byte[contadorJ],al

; ************ start while 2 *****************

	mov 	rdi,msjInicioWhile2
	call 	safePuts

	mov 	rdi,msjValorAEvaluar
	mov 	rsi,[vectorNros+rax]

	call 	safePrintf

while2:

	cmp 	byte[contadorJ],0
	jle  	finWhile2KMenorCero

	mov 	bl,[contadorJ]
	dec 	bl
	mov 	cl,[contadorJ]
	mov 	al,[vectorNros+rbx]

	mov 	rdi,msjEstadoActual
	call 	safePuts
	
	call 	imprimirVector
	
	cmp 	byte[ordenarAscODsc],'D'
	je 		ordenDescendente

	mov 	rdi,msjOrdenoAscendentemente
	call 	safePuts

	cmp 	al,[vectorNros+rcx]
	jle 	finWhile2ElemOrdenados

	mov 	rdi,msjNoEstanOrdenadosAscendente
	mov 	rsi,rax
	mov 	rdx,[vectorNros+rcx]

	call 	safePrintf

	jmp 	bodyWhile2

ordenDescendente:

	mov 	rdi,msjOrdenoDescendentemente
	call 	safePuts
	
	cmp 	al,[vectorNros+rcx]
	jge 	finWhile2ElemOrdenados

	mov 	rdi,msjNoEstanOrdenadosDescendente
	mov 	rsi,rax
	mov 	rdx,[vectorNros+rcx]

	call 	safePrintf



bodyWhile2:

	call 	swap
	sub 	byte[contadorJ],1

	mov 	rdi,msjEstadoPostSwap
	call 	safePuts

	call 	imprimirVector

	jmp 	while2

; *********** end while 2 ****************
finWhile2KMenorCero:
	
	mov 	rdi,msjFinWhile2KMenorCero
	call 	safePuts
	jmp 	finWhile2

finWhile2ElemOrdenados:

	mov 	rdi,msjFinWhile2ElemOrdenados
	call 	safePuts

finWhile2:

	add 	byte[contadorI],1
	jmp 	while1

; ********* end while 1 ******************
finWhile1:	
	
	mov 	rdi,msjFinWhile1
	call 	safePuts

ret

;****************** imprimir los dos valores a swapear ***********************

imprimirValoresASwapear:

	mov 	rax,0
	mov 	al,[vectorNros+rbx]
	mov 	byte[auxPrintf],al

	mov 	rax,0
	mov 	al,[vectorNros+rcx]
	mov 	byte[auxPrintf2],al

	mov 	rdi,msjValoresASwapear
	mov 	rsi,[auxPrintf]
	mov 	rdx,[auxPrintf2]

	call 	safePrintf

ret

;****************** swapear dos elementos del vector *********************

swap:

	; swapea el contenido de [vectorNros+bl] con el contenido de [vectorNros+cl]

	; auxSwap <--- vectorNros[j]

	mov 	rax,0
	mov 	al,[vectorNros+rcx]
	mov 	byte[auxSwap],al


	; vectorNros[j] <-- vectorNros[j-1]

	mov 	rax,0
	mov 	al,[vectorNros+rbx]
	mov 	byte[vectorNros+rcx],al

	;vectorNros[j-1] <--- auxSwap

	mov 	rax,0
	mov 	al,[auxSwap]
	mov 	byte[vectorNros+rbx],al


ret

;********************** safe printf ************************

safePrintf:

	push 	rax
	push 	rbx
	push 	rcx
	call 	checkAlign
	sub 	rsp,[plusRsp]
	call 	printf
	add 	rsp,[plusRsp]
	pop 	rcx
	pop 	rbx
	pop 	rax

ret

;******************** safe puts ************************

safePuts:
; imprime por pantalla el contenido del registro rdi dejando intacto los registros rax,rbx y rcx

	push 	rax
	push 	rbx
	push 	rcx
	call 	puts
	pop 	rcx
	pop 	rbx
	pop 	rax
	
ret

;******************************** imprimir vector ******************************

imprimirVector:

	push 	rax
	push 	rbx
	push 	rcx

	mov 	rbx,0

imprimirSiguienteNro:

	mov 	al,[vectorNros+rbx]
	mov 	rdi,msjVector
	mov 	rsi,rax

	call 	safePrintf

	inc 	rbx
	cmp 	rbx,[longVector]
	jl 		imprimirSiguienteNro

	mov 	rdi,msjFin
	call 	puts

	pop 	rcx
	pop 	rbx
	pop 	rax

ret


;***************** para evitar que las funciones llamadas de C no exploten ************************

checkAlign:

	push 	rax
	push	rbx
	push	rdx
	push	rdi

	mov 	qword[plusRsp],0
	mov 	rdx,0

	mov 	rax,rsp
	add 	rax,8
	add 	rax,32

	mov 	rbx,16
	idiv	rbx

	cmp 	rdx,0
	je 		finCheckAlign

	mov 	qword[plusRsp],8

finCheckAlign:

	pop 	rdi
	pop 	rdx
	pop 	rbx
	pop 	rax
ret