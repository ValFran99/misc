import random

class Grafo:

	def __init__(self, vert_inic = [], es_dirigido = False):

		self.vertices = {}
		self.es_dirigido = es_dirigido
		for v in vert_inic:
			self.vertices[v] = {}

	def __str__(self):

		cad = ""
		for v, destinos in self.vertices.items():
			cad += str(v)
			cad += " -> "
			for dest, peso in destinos.items():
				cad += "(" + str(dest) + ", " + str(peso) + ")"
			cad += "\n\n"
		return cad

	def __iter__(self):
		return iter(self.vertices)

	def __len__(self):
		return len(self.vertices)
	
	def existe_vertice(self, v):
		return self.vertices.get(v) != None

	def agregar_vertice(self, v):

		if self.existe_vertice(v):
			raise ValueError("ERROR: Ya existe el vértice " + str(v))
		self.vertices[v] = {}

	def sacar_vertice(self, v):

		if self.existe_vertice(v) == False:
			raise ValueError("ERROR: No existe el vértice "+ str(v))

		for w in self.vertices:
			if self.vertices[w].get(v) != None:
				self.vertices[w].pop(v)
		self.vertices.pop(v)

	def obtener_vertices(self):
		return list(self.vertices)

	def _obtener_grado_entrada(self, v):

		gr = 0

		for w in self:
			if self.existe_arista(w, v):
				gr += 1

		return gr

	def obtener_grado_vertice(self, v):
		""" Recibe un vértice y retorna su grado, si el grafo es dirigido retorna una tupla con
		formato (gr_entrada, gr_salida), si no lo es retorna un entero
		"""
		if self.existe_vertice(v) == False:
			raise ValueError("ERROR: No existe el vértice " + str(v))

		if self.es_dirigido == True:
			return (self._obtener_grado_entrada(v), len(self.obtener_adyacentes(v)))
			
		return len(self.obtener_adyacentes(v))

	def obtener_vertice_random(self):
		
		vert = self.vertices.keys()
		return random.choice(list(vert))

	def obtener_adyacentes(self, v):
		""" Retorna una lista con los vértices adyacentes al vertice recibido
		"""
		if self.existe_vertice(v) == False:
			raise ValueError("ERROR: No existe el vértice "+ str(v))
		return list(self.vertices[v])

	def obtener_aristas(self):

		aristas = []

		for vertice, adyacentes in self.vertices.items():
			for ady, peso in adyacentes.items():
				aristas.append((vertice, ady, peso))

		return aristas

	def existe_arista(self, v, w):

		if self.existe_vertice(v) == False or self.existe_vertice(w) == False:
			return False

		return self.vertices[v].get(w) != None

	def agregar_arista(self, v, w, peso = 1):
		""" Recibo dos vértices y un peso (por defecto 1) y agrega la arista correspondiente, si el grafo es 
		no dirigido además agrega la arista inversa (w, v)
		"""
		if self.existe_arista(v, w):
			raise ValueError("ERROR: Ya existe la arista " + str(v)+ ", " + str(w))

		existe_v = self.existe_vertice(v)
		existe_w = self.existe_vertice(w)

		if existe_v == None or existe_w == None:
			if existe_v == None and existe_w == None:
				raise ValueError("ERROR: No existen ni " + str(v) + "ni " + str(w))
			elif existe_v == None:
				raise ValueError("ERROR: No existe el vértice " + str(v))
			else:
				raise ValueError("ERROR: No existe el vértice " + str(w))

		self.vertices[v][w] = peso

		if self.es_dirigido == False:
			self.vertices[w][v] = peso		

	def sacar_arista(self, v, w):

		if self.existe_arista(v, w) == False:
			raise ValueError("ERROR: No existe la arista " + str(v) + ", " + str(w))

		self.vertices[v].pop(w)
		if self.es_dirigido == False:
			self.vertices[w].pop(v)

	def obtener_peso(self, v, w):

		if self.existe_arista(v, w) == False:
			raise ValueError("ERROR: No existe la arista" + str(v) + ", " + str(w))
		return self.vertices[v][w]

	def actualizar_peso(self, v, w, nuevo_peso):
		""" Actualiza el peso de la arista (v, w) al nuevo peso recibido por parámetro, si el grafo es no dirigido
		también actualiza la arista inversa
		"""
		if self.existe_arista(v, w) == False:
			raise ValueError("ERROR: No existe la arista" + str(v) + ", " + str(w))

		self.vertices[v][w] = nuevo_peso
		if self.es_dirigido == False:
			self.vertices[w][v] = nuevo_peso