class _Nodo:
    
    def __init__(self, dato, prox=None):
        self.dato = dato
        self.prox = prox

class Cola:
    
    def __init__(self):
        """ Inicializa una cola vacía
        """
        self.frente = None
        self.ultimo = None
    
    def encolar(self, dato):
        """ Recibe un dato y lo encola al final de la cola
        """
        nuevo_nodo = _Nodo(dato)
        
        if self.frente == None:
            self.frente = nuevo_nodo
        else:
            self.ultimo.prox = nuevo_nodo
            
        self.ultimo = nuevo_nodo
    
    def ver_frente(self):
        """ Ve el nodo que está en frente de la cola, en caso de que la cola esté vacía lanza una excepción
        """
        if self.esta_vacia():
            raise ValueError("La cola está vacía")
        return self.frente.dato
    
    def desencolar(self):
        """ Desencola el elemento que está en frente de la cola y lo devuelve, el nuevo primer elemento será el anterior a él
            Si la cola está vacía lanza una excepción
        """
        if self.esta_vacia():
            raise ValueError("La cola está vacía")
        
        dato = self.frente.dato
        self.frente = self.frente.prox
        if self.frente == None:
            self.ultimo = None
        return dato
        
    def esta_vacia(self):
        """ Devuelve True en caso de que la cola esté vacía o False en caso contrario
        """
        return self.frente is None
    
class Pila:
    
    def __init__(self):
        """ Inicializa una pila vacía
        """
        self.tope = None
        
    def apilar(self, dato):
        """ Recibe un dato y lo ubica en el tope de la pila
        """
        self.tope = _Nodo(dato, self.tope)
        
    def desapilar(self):
        """ Desapila el elemento que está en el tope de la pila y lo devuelve, en caso de que la pila esté vacía lanza una excepción
        """  
        if self.esta_vacia():
            raise ValueError("La pila está vacía")
        dato = self.tope.dato
        self.tope = self.tope.prox
        return dato
    
    def ver_tope(self):
        """ Devuelve el dato del elemento que está en el tope de la pila, en caso de que la pila esté vacía lanza una excepción
        """
        if self.esta_vacia():
            raise ValueError("La pila está vacía")
        return self.tope.dato
        
    def esta_vacia(self):
        """ Devuelve True en caso de que la pila esté vacía o False en caso contrario
        """
        return self.tope is None
    
    def vaciar(self):
        """ Elimina todos los elementos de la lista
        """
        self.tope = None