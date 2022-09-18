from collections import deque
from grafo import Grafo
import random

COEF_DAMPING = 0.85

def busqueda_bfs(origen, destino, grafo):
	""" 
	Recibo un vertice de origen, uno de destino y un grafo, realiza un recorrido bfs por el grafo hasta
	visitar todos los vertices dentro de la componente conexa del origen o hasta encontrarse con el vértice 
	destino. Devuelve una tupla de formato (padres, orden), donde padres es un diccionario que contiene los padres
	de cada vértice dentro del recorrido y orden es otro diccionario que contiene el orden de cada vértice relativo
	al vértice de origen
	"""
	padres = {}
	orden = {}
	visitados = set()
	q = deque()

	padres[origen] = None
	orden[origen] = 0
	visitados.add(origen)
	q.append(origen)

	while q:
		v = q.popleft()

		for ady in grafo.obtener_adyacentes(v):
			if ady not in visitados:
				visitados.add(ady)
				orden[ady] = orden[v] + 1
				padres[ady] = v
				if ady == destino:
					return (padres, orden)
				q.append(ady)

	return (padres, orden) 

def _ciclo_largo_n(origen, actual, visitados, n, camino, grafo):

	if len(camino) == n+1 and camino[0] == camino[len(camino) - 1]:
		return camino

	visitados.add(actual)
	camino.append(actual)
	
	for ady in grafo.obtener_adyacentes(actual):

		if ady not in visitados:
			if len(camino) == n:
				continue
			check = _ciclo_largo_n(origen, ady, visitados, n, camino, grafo)
			if check == []:
				continue
			return check

		elif ady == origen and len(camino) == n:
			camino.append(ady)
			return _ciclo_largo_n(origen, ady, visitados, n, camino, grafo)

	visitados.remove(actual)
	camino.pop()

	return []

def ciclo_largo_n(origen, n, grafo):
	""" 
	Recibe un vértice de origen, un entero n y un grafo y devuelve una lista con el ciclo de largo n
	que comienza y termina en el vértice origen
	"""
	visitados = set()
	camino = []

	return _ciclo_largo_n(origen, origen, visitados, n, camino, grafo)

def todas_a_rango(origen, rango, grafo):
	""" 
	Recibe un vértice origen, un entero rango y un grafo y devuelve la cantidad de vértices que se
	encuentran exactamente a rango pasos del vértice de origen
	"""
	orden = {}
	visitados = set()
	cant_vertices = 0
	orden[origen] = 0
	q = deque()
	visitados.add(origen)
	q.append(origen)

	while q:
		v = q.popleft()

		for ady in grafo.obtener_adyacentes(v):
			if ady not in visitados:
				visitados.add(ady)
				orden[ady] = orden[v] + 1
				if orden[ady] == rango:
					cant_vertices += 1
				if orden[ady] > rango:
					return cant_vertices
				q.append(ady)

	return cant_vertices

def page_rank(cant_iteraciones, grafo):
	""" 
	Recibe un grafo y aplica el algoritmo pagerank al grafo las veces indicadas por en el entero cant_iteraciones,
	devuelve un diccionario con formato (vertice, valor) ordenado de mayor a menor en base al valor de pagerank de cada
	vértice
	"""
	page_ranks = {}
	cant_vert = len(grafo)
	constante = (1 - COEF_DAMPING) / cant_vert

	for v in grafo:
		page_ranks[v] = 1 / cant_vert

	suma = 0
	while cant_iteraciones != 0:

		for v in grafo:
			for ady in grafo.obtener_adyacentes(v):
				suma += page_ranks[ady] / len(grafo.obtener_adyacentes(ady))

			page_ranks[v] = constante + COEF_DAMPING*suma
			suma = 0

		cant_iteraciones -= 1

	ordenado_dicc = {}

	for w in page_ranks:
		if type(w) == tuple:
			ordenado_dicc[w] = page_ranks[w]  

	return sorted(ordenado_dicc.items(), key=lambda item: item[1], reverse= True)

def page_custom(actual, longitud, dict_vertices, grafo_bipartito):
	""" 
	Recibe un vértice, un entero, un diccionario de vértices y un grafo bipartito y actualiza el diccionario
	de vértices con los valores correspondientes luego de aplicar al grafo bipartito una versión personalizada
	de PageRank
	"""
	interes = actual

	while longitud != 0:

		for a in grafo_bipartito.obtener_adyacentes(actual):
			dict_vertices[a] = dict_vertices[a] + (dict_vertices[actual] / grafo_bipartito.obtener_grado_vertice(actual))

		booleanos = [True , False]
		vertice_random = random.choices(booleanos, cum_weights=(15, 85), k=1) #Factor dumping

		if vertice_random[0] == True: #Esto es para ir parte no conexas del grafo
			actual = interes
		else:
			actual = random.choice(grafo_bipartito.obtener_adyacentes(actual))	

		longitud -= 1

	return 