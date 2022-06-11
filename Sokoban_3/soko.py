JUGADOR = "@"
CAJA = "$"
PARED = "#"
OBJETIVO = "."
OBJETIVO_CAJA = "*"
OBJETIVO_JUGADOR = "+"
CELDA_VACIA = " "


def crear_grilla(desc):
    '''Crea una grilla a partir de la descripción del estado inicial.

    La descripción es una lista de cadenas, cada cadena representa una
    fila y cada caracter una celda. Los caracteres pueden ser los siguientes:

    Caracter  Contenido de la celda
    --------  ---------------------
           #  Pared
           $  Caja
           @  Jugador
           .  Objetivo
           *  Objetivo + Caja
           +  Objetivo + Jugador

    Ejemplo:

    >>> crear_grilla([
        '#####',
        '#.$ #',
        '#@  #',
        '#####',
    ])
    '''
    #Creo una copia de la descripción y la guardo en la variable grilla
    #Lo que hace es, crea una lista de listas donde cada, para todo indice x dentro del rango de la longitud de la lista
    #descripción me crea una lista a la que se le añade para todo indice perteneciente al rango de una fila de la descripción
    #el caracter perteneciente a esa direccion en la nueva lista
    grilla = [[desc[x][i] for i in range(len(desc[x]))] for x in range(len(desc))]
    # Inicializo el contador de niveles al crear la grilla
    return grilla

def dimensiones(grilla):
    '''Devuelve una tupla con la cantidad de columnas y filas de la grilla.'''
    #La cantidad de filas sería la longitud de la lista principal, ya que cada cadena representa una fila
    filas = len(grilla)
    # Ahora al tener los niveles en un formato .txt no puedo asegurar que los niveles tendrán los espacios necesarios para que
    # el nivel tenga una estructura rectangular, por lo que debo tomar la longitud más larga de todas las filas
    max_columnas = 0
    for l in grilla:
        if len(l) > max_columnas:
            max_columnas = len(l)
    return max_columnas, filas

def hay_pared(grilla, c, f):
    '''Devuelve True si hay una pared en la columna y fila (c, f).'''
    if grilla[f][c] == PARED:
        return True
    return grilla[f][c] == PARED
    
def hay_objetivo(grilla, c, f):
    '''Devuelve True si hay un objetivo en la columna y fila (c, f).'''
    
    if grilla[f][c] == OBJETIVO or grilla[f][c] == OBJETIVO_CAJA or grilla[f][c] == OBJETIVO_JUGADOR:
        return True
    
    return grilla[f][c] == OBJETIVO or grilla[f][c] == OBJETIVO_CAJA or grilla[f][c] == OBJETIVO_JUGADOR

def hay_caja(grilla, c, f):
    """ Devuelve True si hay una caja en la columna y fila (c, f). """
        
    if grilla[f][c] == CAJA or grilla[f][c] == OBJETIVO_CAJA:
        return True
    return grilla[f][c] == CAJA or grilla[f][c] == OBJETIVO_CAJA
    
def hay_jugador(grilla, c, f):
    '''Devuelve True si el jugador está en la columna y fila (c, f).'''

    if grilla[f][c] == JUGADOR or grilla[f][c] == OBJETIVO_JUGADOR:
        return True
    return grilla[f][c] == JUGADOR or grilla[f][c] == OBJETIVO_JUGADOR

def juego_ganado(grilla):
    '''Devuelve True si el juego está ganado.'''
    
    # Recorro la grilla y si detecto alguno de los elementos que indicarían un juego no ganado devuelvo False
    
    for f in range(len(grilla)):
        for c in range(len(grilla[f])):
            if grilla[f][c] in CAJA + OBJETIVO + OBJETIVO_JUGADOR:
                return False
    return True

def mover(grilla, direccion):
    '''Mueve el jugador en la dirección indicada.

    La dirección es una tupla con el movimiento horizontal y vertical. Dado que
    no se permite el movimiento diagonal, la dirección puede ser una de cuatro
    posibilidades:

    direccion  significado
    ---------  -----------
    (-1, 0)    Oeste
    (1, 0)     Este
    (0, -1)    Norte
    (0, 1)     Sur

    La función debe devolver una grilla representando el estado siguiente al
    movimiento efectuado. La grilla recibida NO se modifica; es decir, en caso
    de que el movimiento sea válido, la función devuelve una nueva grilla.
    '''
    
    #Creo una nueva grilla
    nueva_grilla= []
    #A esa nueva grilla le voy agregando las componentes de la grilla vieja casteadas a listas, porque
    #de no ser así no podría modificarlos
    for i in range(len(grilla)):
        nueva_grilla.append(list(grilla[i]))
    #Ubico la posición del jugador en la nueva grilla
    for f in range(len(nueva_grilla)):
        for c in range(len(nueva_grilla[f])):
            if hay_jugador(nueva_grilla, c, f):
                #Cuando encuentro al jugador guardo esa posición en una tupla y rompo el for loop
                posicion_jugador = f, c
                break
    #Descompongo esa tupla en dos para poder operar con las direcciones, ya que no
    #podría operar con tuplas como lo haría con vectores [ejemplo: (1, 2) - (1, 0) != (0, 2), no es válido]
    fila_jugador, columna_jugador = posicion_jugador
    columna_direccion, fila_direccion = direccion
    #Tomo que el destino del jugador será la suma entre las componentes de la posición actual y
    #la dirección recibida
    fila_celda_destino = fila_jugador + fila_direccion
    columna_celda_destino = columna_jugador + columna_direccion
    
    #Hago todas las condiciones posibles necesarias
    
    if hay_pared(nueva_grilla, columna_celda_destino, fila_celda_destino):
        #Cuando hay una pared en la celda destino devuelvo la grilla original porque no se modificó nada
        return grilla
    
    elif nueva_grilla[fila_celda_destino][columna_celda_destino] == CELDA_VACIA:
        # Caso en el que hay una celda vacía y el movimiento es posible, tengo que tener en cuenta si el jugador estaba
        # parado en un objetivo o no
        if hay_objetivo(nueva_grilla, columna_jugador, fila_jugador):
            nueva_grilla[fila_jugador][columna_jugador] = OBJETIVO
            nueva_grilla[fila_celda_destino][columna_celda_destino] = JUGADOR
        else:
            nueva_grilla[fila_jugador][columna_jugador] = CELDA_VACIA
            nueva_grilla[fila_celda_destino][columna_celda_destino] = JUGADOR
    
    elif hay_caja(nueva_grilla, columna_celda_destino, fila_celda_destino):
        #Al haber una caja en frente del jugador le vuelvo a sumar a esa posición la dirección deseada para
        #obtener la celda siguiente
        fila_celda_destino_caja = fila_celda_destino + fila_direccion
        columna_celda_destino_caja = columna_celda_destino + columna_direccion
        
        # Me quito de encima las combinaciones que me restringen el movimiento
        
        if hay_pared(nueva_grilla, columna_celda_destino_caja, fila_celda_destino_caja):
            return grilla
        if hay_caja(nueva_grilla, columna_celda_destino_caja, fila_celda_destino_caja):
            return grilla
        
        # Separo en dos "ramas" los movimientos con la caja teniendo en cuenta si el jugador está parado en una celda vacía
        # o en un objetivo
        if hay_objetivo(nueva_grilla, columna_jugador, fila_jugador):
            # El código dentro de las dos "ramas" es bastante parecido, solo cambia lo que pasa con la posición en la
            # que estaba el jugador
            if nueva_grilla[fila_celda_destino][columna_celda_destino] == OBJETIVO_CAJA:
                if nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] == CELDA_VACIA:
                    nueva_grilla[fila_jugador][columna_jugador] = OBJETIVO
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = OBJETIVO_JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = CAJA
                else:
                    nueva_grilla[fila_jugador][columna_jugador] = OBJETIVO
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = OBJETIVO_JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = OBJETIVO_CAJA
            
            else:
                if nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] == CELDA_VACIA:
                    nueva_grilla[fila_jugador][columna_jugador] = OBJETIVO
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = CAJA
                else:
                    nueva_grilla[fila_jugador][columna_jugador] = OBJETIVO
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = OBJETIVO_CAJA
        else:
            if nueva_grilla[fila_celda_destino][columna_celda_destino] == OBJETIVO_CAJA:
                if nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] == CELDA_VACIA:
                    nueva_grilla[fila_jugador][columna_jugador] = CELDA_VACIA
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = OBJETIVO_JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = CAJA
                else:
                    nueva_grilla[fila_jugador][columna_jugador] = CELDA_VACIA
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = OBJETIVO_JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = OBJETIVO_CAJA
            
            else:
                if nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] == CELDA_VACIA:
                    nueva_grilla[fila_jugador][columna_jugador] = CELDA_VACIA
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = CAJA
                else:
                    nueva_grilla[fila_jugador][columna_jugador] = CELDA_VACIA
                    nueva_grilla[fila_celda_destino][columna_celda_destino] = JUGADOR
                    nueva_grilla[fila_celda_destino_caja][columna_celda_destino_caja] = OBJETIVO_CAJA
        
    elif hay_objetivo(nueva_grilla, columna_celda_destino, fila_celda_destino):
        #Si en la celda destino hay un objetivo, debo tenerlo en cuenta y reemplazarlo por
        #jugador + objetivo
        # Además tener en cuenta que si estoy parado en un objetivo no lo borre
        if hay_objetivo(nueva_grilla, columna_jugador, fila_jugador):
            nueva_grilla[fila_jugador][columna_jugador] = OBJETIVO
            nueva_grilla[fila_celda_destino][columna_celda_destino] = OBJETIVO_JUGADOR
        else:
            nueva_grilla[fila_jugador][columna_jugador] = CELDA_VACIA
            nueva_grilla[fila_celda_destino][columna_celda_destino] = OBJETIVO_JUGADOR
    
    else:
        return grilla
    return nueva_grilla

