#!/usr/bin/python3
import sys
import csv
import biblioteca
from grafo import Grafo
import random

RUTA_FILE = sys.argv[1]
CMD_CAMINO = "camino"
CMD_IMPORTANTES = "mas_importantes"
CMD_RECOMENDACION = "recomendacion"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"
RANKING = [] 
GRAFO_CANCIONES = None

NOMBRE_CMD = 0
PAGE_CUSTOM_LONG = 100

PLAY_ID = 0
USER = 1
PLAY_NAME = 2

CANT_PARAM_CANCION = 2
NOMBRE_CANCION = 0
ARTISTA = 1

CANT_PARAM_CAMINO = 2
CAMINO_INI = 0
CAMINO_FIN = 1

CANT_ITER_IMPORTANTES = 20

CANT_PARAM_REC = 3
TIPO_REC = 0
CANT_REC = 1
CANCIONES_REC = 2

CANT_PARAM_CICLO_RANGO = 2
CICLO_RANGO_CANT_CANCIONES = 0

CICLO_RANGO_CANT = 0
CICLO_RANGO_CANCION = 1

CLUSTERING_REDONDEO = 3

CAMINO_SALIDA_1 = " --> aparece en playlist --> "
CAMINO_SALIDA_2 = " --> de --> "
CAMINO_SALIDA_3 = " --> tiene una playlist --> "
CAMINO_SALIDA_4 = " --> donde aparece --> "

def cargar_grafo_bipartito(cancion, info, grafo_bipartito):

	if grafo_bipartito.existe_vertice(info[USER]) == False:
		grafo_bipartito.agregar_vertice(info[USER])

	if grafo_bipartito.existe_vertice(cancion) == False:
		grafo_bipartito.agregar_vertice(cancion)

	if grafo_bipartito.existe_arista(info[USER], cancion) == False:
		grafo_bipartito.agregar_arista(info[USER], cancion, info)

def cargar_grafo_canciones(cancion, info, playlist_name, dicc_playlists, grafo_canciones):

	if grafo_canciones.existe_vertice(cancion) == False:
		grafo_canciones.agregar_vertice(cancion)

	for cancion_playlist in dicc_playlists[playlist_name]:
		if grafo_canciones.existe_arista(cancion, cancion_playlist) == False:
			grafo_canciones.agregar_arista(cancion, cancion_playlist, info)

def cargar_grafos(ruta_tsv, grafo_bipartito, grafo_canciones, tipo):

	with open(ruta_tsv, "r") as archivo:
		tsv_reader = csv.reader(archivo, delimiter="\t")
		next(tsv_reader)	# Saltear el header

		dicc_playlists = {}

		for ID, USER_ID, TRACK_NAME, ARTIST, PLAYLIST_ID, PLAYLIST_NAME, GENRES in tsv_reader:

			if PLAYLIST_ID.isnumeric() == False:
				continue

			cancion = (TRACK_NAME, ARTIST)
			info = (int(PLAYLIST_ID), USER_ID, PLAYLIST_NAME)

			if tipo == False:
				if PLAYLIST_NAME not in dicc_playlists:
					dicc_playlists[PLAYLIST_NAME] = []
				cargar_grafo_canciones(cancion, info, PLAYLIST_NAME, dicc_playlists, grafo_canciones)
				dicc_playlists[PLAYLIST_NAME].append(cancion)
				continue
			cargar_grafo_bipartito(cancion, info, grafo_bipartito)

def quitar_espacios(string):
	return string.rstrip(' ').lstrip(' ')

def crear_cancion(param):

	param = param.split('-', CANT_PARAM_CANCION - 1)
	if len(param) != CANT_PARAM_CANCION:
		return ()

	return (quitar_espacios(param[NOMBRE_CANCION]), quitar_espacios(param[ARTISTA]))

def validar_cmd_camino(parametros):
	"""
	Recibe los parámetros necesarios para el comando camino y valida si todo está ok
	Retorna una tupla de dos canciones, la primera es el inicio del camino y la segunda es el final
	En caso de error devuelve una tupla vacía
	"""
	parametros_camino = parametros.split(">>>>")
	if len(parametros_camino) != CANT_PARAM_CAMINO:
		return ()

	cancion_inicio = crear_cancion(parametros_camino[CAMINO_INI])
	cancion_fin = crear_cancion(parametros_camino[CAMINO_FIN])

	if not cancion_inicio or not cancion_fin:
		print("Tanto el origen como el destino deben ser canciones")
		return ()

	return (cancion_inicio, cancion_fin)

def validar_cmd_importantes(parametros):
	"""
	Recibe los parámetros necesarios para el comando importantes
	Devuelve un entero con la cantidad de canciones a recomendar, devuelve False en caso de error
	"""
	if parametros.isnumeric() == False:
		# print("ERROR: El parámetro debe ser numérico")
		return False
	# Esto es solo la cantidad de canciones requerida
	return int(quitar_espacios(parametros))

def validar_cmd_recomendaciones(parametros):
	"""
	Recibe los parámetros necesarios para el comando recomendaciones
	Devuelve una tupla con formato (recomendar, cant, canciones) y una tupla vacía en caso de error
	Saltea las canciones ingresadas por comando que no siguen el formato nombre - artista
	"""
	parametros_rec = parametros.split(' ', CANT_PARAM_REC - 1)
	if len(parametros_rec) != CANT_PARAM_REC:
		return ()

	if parametros_rec[CANT_REC].isnumeric() == False:
		return ()

	recomendar = parametros_rec[TIPO_REC]
	cant = int(parametros_rec[CANT_REC])

	canciones = []

	for cancion in parametros_rec[CANCIONES_REC].split(">>>>"):
		cancion_apendear = crear_cancion(cancion)
		if not cancion_apendear:
			continue
		canciones.append(crear_cancion(cancion))

	return (recomendar, cant, canciones)

def validar_cmd_ciclo_rango(parametros):
	"""
	Recibe los parámetros necesarios para los comandos ciclo/rango
	Devuelve una tupla con formato (cantidad, cancion) y una tupla vacía en caso de error
	"""
	params = parametros.split(' ', CANT_PARAM_CICLO_RANGO - 1)

	if len(params) != CANT_PARAM_CICLO_RANGO:
		return ()

	if params[CICLO_RANGO_CANT].isnumeric() == False:
		return ()

	cant = int(params[CICLO_RANGO_CANT])
	cancion = crear_cancion(params[CICLO_RANGO_CANCION])

	if not cancion:
		return ()

	return (cant, cancion)

def validar_cmd_clustering(parametros):
	"""
	Recibe los parámetros necesarios para el comando clustering
	Devuelve una tupla vacía en caso de no recibir una canción específica o la canción específicada
	en forma de tupla (nombre, artista)
	"""
	if parametros == "":
		return ()

	return crear_cancion(parametros)		

def str_cancion(cancion):

	return cancion[NOMBRE_CANCION] + " - " + cancion[ARTISTA]

def crear_salida_camino(camino, grafo_bipartito):

	campo_anterior = camino[0]
	salida = str_cancion(campo_anterior)
	cont = 0

	for i in range(1, len(camino)):
		campo_actual = camino[i]
		info = grafo_bipartito.obtener_peso(campo_anterior, campo_actual)
		if cont % 2 == 0:
			salida += CAMINO_SALIDA_1 + str(info[PLAY_NAME]) + CAMINO_SALIDA_2 + str(info[USER])
		else:
			salida += CAMINO_SALIDA_3 + str(info[PLAY_NAME]) + CAMINO_SALIDA_4 + str_cancion(campo_actual)
		campo_anterior = campo_actual
		cont += 1

	return salida

def armar_camino(inicio, fin, padres):
	
	camino = []
	act = fin
	camino.append(act)

	while padres[act] != None:
		camino.append(padres[act])
		act = padres[act]

	camino.reverse()

	return camino

def procesar_cmd_camino(canciones, grafo_bipartito):

	if grafo_bipartito.existe_vertice(canciones[CAMINO_INI]) == False:
		print("No se encontro recorrido")
		return

	padres = biblioteca.busqueda_bfs(canciones[CAMINO_INI], canciones[CAMINO_FIN], grafo_bipartito)

	if padres[0].get(canciones[CAMINO_FIN]) == None:
		print("No se encontro recorrido")
		return
	
	camino = armar_camino(canciones[CAMINO_INI], canciones[CAMINO_FIN], padres[0])
	print(crear_salida_camino(camino, grafo_bipartito))

def procesar_cmd_importantes(cant_importantes, grafo_bipartito):

	global RANKING

	cadena = ""

	if not RANKING:
		RANKING = biblioteca.page_rank(CANT_ITER_IMPORTANTES, grafo_bipartito) #llamado una vez primera vez 

	for i in range(cant_importantes):
		cadena = cadena + str_cancion(RANKING[i][0]) + "; "  

	print(cadena.rstrip("; "))

def recomendaciones(grafo_bipartito, lista_parametro, tipo_rec):
	
	diccionario_vertices = {}

	for vert in grafo_bipartito.obtener_vertices():
		if vert in lista_parametro:
			diccionario_vertices[vert] = 1/grafo_bipartito.obtener_grado_vertice(vert)
			continue
		diccionario_vertices[vert] = 0

	for v in lista_parametro:
		biblioteca.page_custom(v, PAGE_CUSTOM_LONG, diccionario_vertices, grafo_bipartito)

	ordenado_dicc = {}

	for vertice in diccionario_vertices:
		if vertice in lista_parametro:
			continue

		if type(vertice) == tuple and tipo_rec == True:
			ordenado_dicc[vertice] = diccionario_vertices[vertice]

		elif type(vertice) != tuple and tipo_rec == False:
			ordenado_dicc[vertice] = diccionario_vertices[vertice]

	return sorted(ordenado_dicc.items(), key=lambda item: item[1], reverse= True) 

def procesar_cmd_recomendacion(parametros_rec, grafo_bipartito):

	cant_rec = parametros_rec[1]
	tipo_rec = True

	if parametros_rec[0] != "canciones":
		tipo_rec = False

	recomendados = recomendaciones(grafo_bipartito, parametros_rec[2], tipo_rec)
	cadena = "" 

	for i in range(cant_rec):
		if tipo_rec == True:
			cadena = cadena + str_cancion(recomendados[i][0]) + "; " 
		else: 
			cadena = cadena + recomendados[i][0] + "; " 

	print(cadena.rstrip("; "))

def crear_salida_ciclo(ciclo):

	salida = ""

	for cancion in ciclo:
		salida += str_cancion(cancion) + " --> "

	return salida.rstrip(" --> ")

def procesar_cmd_ciclo(parametros, grafo_canciones):

	if grafo_canciones.existe_vertice(parametros[CICLO_RANGO_CANCION]) == False:
		return

	ciclo = biblioteca.ciclo_largo_n(parametros[CICLO_RANGO_CANCION], parametros[CICLO_RANGO_CANT], grafo_canciones)

	if not ciclo:
		print("No se encontro recorrido")
		return

	print(crear_salida_ciclo(ciclo))

def procesar_cmd_rango(parametros, grafo_canciones):

	if grafo_canciones.existe_vertice(parametros[CICLO_RANGO_CANCION]) == False:
		return

	cant_a_rango = biblioteca.todas_a_rango(parametros[CICLO_RANGO_CANCION], parametros[CICLO_RANGO_CANT], grafo_canciones)
	print(cant_a_rango)

def clustering_individual(v, grafo_canciones):

	if not grafo_canciones.existe_vertice(v):
		return

	grado = grafo_canciones.obtener_grado_vertice(v)
	if grado < 2:
		return 0

	cant_aristas = 0

	for w in grafo_canciones.obtener_adyacentes(v):
		for y in grafo_canciones.obtener_adyacentes(v):
			if grafo_canciones.existe_arista(w, y):
				cant_aristas += 1

	denominador = grado * (grado - 1)

	return round(cant_aristas / denominador, CLUSTERING_REDONDEO)

def clustering_promedio(grafo_canciones):
	
	cant_vertices = 0
	clustering_acc = 0

	for v in grafo_canciones:
		cant_vertices += 1
		clustering_acc += clustering_individual(v, grafo_canciones)

	return round(clustering_acc / cant_vertices, CLUSTERING_REDONDEO)

def procesar_cmd_clustering(cancion, grafo_canciones):

	if not cancion:
		return clustering_promedio(grafo_canciones)

	return clustering_individual(cancion, grafo_canciones)

def procesar_comando(comando, parametros, grafo_bipartito, grafo_canciones):

	global GRAFO_CANCIONES

	if (comando == CMD_CICLO or comando == CMD_RANGO or comando == CMD_CLUSTERING) and GRAFO_CANCIONES is None:
		cargar_grafos(RUTA_FILE, grafo_bipartito, grafo_canciones, False)
		GRAFO_CANCIONES = grafo_canciones

	if comando == CMD_CAMINO:
		canciones = validar_cmd_camino(parametros)
		if not canciones: return
		procesar_cmd_camino(canciones, grafo_bipartito)

	elif comando == CMD_IMPORTANTES:
		cant_importantes = validar_cmd_importantes(parametros)
		if not cant_importantes: return
		procesar_cmd_importantes(cant_importantes, grafo_bipartito)

	elif comando == CMD_RECOMENDACION:
		parametros_rec = validar_cmd_recomendaciones(parametros)
		if not parametros_rec: return
		procesar_cmd_recomendacion(parametros_rec, grafo_bipartito)

	elif comando == CMD_CICLO:
		parametros_ciclo = validar_cmd_ciclo_rango(parametros)
		if not parametros_ciclo: return
		procesar_cmd_ciclo(parametros_ciclo, GRAFO_CANCIONES)

	elif comando == CMD_RANGO:
		parametros_rango = validar_cmd_ciclo_rango(parametros)
		if not parametros_rango: return
		procesar_cmd_rango(parametros_rango, GRAFO_CANCIONES)

	elif comando == CMD_CLUSTERING:
		cancion = validar_cmd_clustering(parametros)
		resultado = procesar_cmd_clustering(cancion, GRAFO_CANCIONES)
		print(resultado)

	else:
		print("ERROR: No existe el comando", comando)

def main():

	grafo_bipartito = Grafo()
	grafo_canciones = Grafo()

	cargar_grafos(RUTA_FILE, grafo_bipartito, grafo_canciones, True)

	linea = "placeholder"

	while linea:

		linea = sys.stdin.readline().rstrip('\n')
		comando = linea.split(' ', 1)

		if len(comando) == 1 and comando[NOMBRE_CMD] == CMD_CLUSTERING:
			procesar_comando(comando[NOMBRE_CMD], "", grafo_bipartito, grafo_canciones)
			continue

		try:
			cmd, parametros = comando
		except ValueError:
			continue

		procesar_comando(cmd, parametros, grafo_bipartito, grafo_canciones)
		
main()