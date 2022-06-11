# Version 3, ahora es personal

import soko
import gamelib
from modulo_cola_pila import Pila

# Controles:
# WASD o flechas -> movimiento
# h              -> pista (más que nada utiliza el algoritmo bracktrack para encontrar una solución y completar el nivel)
# b              -> deshacer para el último movimiento hecho
# r              -> reiniciar el nivel
# Esc            -> salir (por diversas carencias, intentar cerrar la ventana clickeando (X) crashea python)

# Se pueden cambiar los controles modificando el archivo "teclas.txt"

# Cosas extras de debug:
# tipear ascii     -> cambia la representación del juego a caracteres ascii
# tipear off       -> vuelve a la representación original
# tipear debug     -> da acceso a dos teclas extra
# n                -> ir al siguiente nivel
# p                -> volver al anterior nivel 

# niveles sacados de
# http://www.sneezingtiger.com/sokoban/levels/microbanText.html

# documentación de gamelib
# https://dessaya.github.io/python-gamelib/


NORTE = (0 ,-1)
SUR = (0, 1)
ESTE = (1, 0)
OESTE = (-1, 0)

SIGUIENTE_NIVEL = "n"
ANTERIOR_NIVEL = "p"

# Menu principal

VENTANA_MENU_X = 500 ; VENTANA_MENU_Y = 300

BOTON_Y_1_I = 30 ; BOTON_Y_1_F = 50
BOTON_Y_2_I = 200 ; BOTON_Y_2_F = 220
BOTON_NEW_GAME_SELECTOR_X_I = 20
BOTON_NEW_GAME_X_F = 150
BOTON_SELECTOR_X_F = 180
BOTON_ULTIMO_X_I = 220 ; BOTON_ULTIMO_X_F = 420
BOTON_SALIR_X_I = 420 ; BOTON_SALIR_X_F = 470

AJUSTE_MENU = 10

NUEVO_JUEGO_TEXTO = "Nuevo juego"
SALIR_TEXTO = "Salir"
SELECCIONAR_NIVEL_TEXTO = "Seleccionar nivel"

# Ruta a imágenes e archivos(las cte están en inglés para intentar diferenciarlas a simple vista de las constantes propias del módulo soko)

RUTA_NIVELES = "niveles.txt"
RUTA_TECLAS = "teclas.txt"
RUTA_SAVE = "save.txt"

# 100%

LONGITUD_CELDA_X = 64
LONGITUD_CELDA_Y = 64

PLAYER = "img/player.gif"
BOX = "img/box.gif"
GROUND = "img/ground.gif"
WALL = "img/wall.gif"
GOAL = "img/goal.gif"
END_KANNA = "img/kanna_thumbs_up.gif"
MIO_SCARED = "img/mio_scared.gif"

# 75%

LIMITE_CELDAS_75_X = 22 ; LIMITE_CELDAS_75_Y = 12

LONGITUD_CELDA_X_C = 48
LONGITUD_CELDA_Y_C = 48

PLAYER_48 = "img/player_48.gif"
BOX_48 = "img/box_48.gif"
GROUND_48 = "img/ground_48.gif"
WALL_48 = "img/wall_48.gif"
GOAL_48 = "img/goal_48.gif"

# 50%

LIMITE_CELDAS_50_X = 27
LIMITE_CELDAS_50_Y = 16

LONGITUD_CELDA_X_M = 32
LONGITUD_CELDA_Y_M = 32

PLAYER_32 = "img/player_32.gif"
BOX_32 = "img/box_32.gif"
GROUND_32 = "img/ground_32.gif"
WALL_32 = "img/wall_32.gif"
GOAL_32 = "img/goal_32.gif"

# Ascii

LONGITUD_CELDA_X_A = 20
LONGITUD_CELDA_Y_A = 20

PLAYER_A = "@"
BOX_A = "$"
GROUND_A = " "
WALL_A = "#"
GOAL_A = "."
BOX_GOAL = "*"
PLAYER_GOAL = "+"

PENSANDO_A = "P..."
NO_SOL_TEXTO_A = "N S"
PISTA_DISPONIBLE_A = "P D!"

AJUSTE_PISTAS_X_A = 10
ALTURA_BARRA_INDICADORES_A = 30

ALTURA_BARRA_INDICADORES = 20
AJUSTE_NIVEL = 10
AJUSTE_PISTAS_X = 80
AJUSTE_PISTAS_Y = 10

PENSANDO = "Pensando..."
NO_SOL = "No sol"
NO_SOL_TEXTO = "No hay solución!"
NO_MORE_A = "=C"
PISTA_DISPONIBLE = "Pista disponible!"

# Max recursión

NO_MORE = "No, por favor"
MENSAJE_REC_1 = "Demasiada recursión"
MENSAJE_REC_2 = "No puedo con este nivel"
MENSAJE_REC_CONTINUAR = "(Presione cualquier tecla para continuar)"
AJUSTE_MITAD = 2
MITAD_IMAGEN = 75
AJUSTE_CUARTO = 4
AJUSTE_Y_1 = 1.25
AJUSTE_Y_2 = 1.20
AJUSTE_Y_3 = 1.15

VENTANA_RECURSION_ASCII = 400

# Pantalla final

KANNA_X = 180
KANNA_Y = 100

VENTANA_FINAL_X = 500
VENTANA_FINAL_Y = 500

MENSAJE_FIN_1 = "¡Felicidades por apretar n muy fuerte (o ganar)!"
MENSAJE_FIN_2 = "(Presione cualquier tecla para cerrar)"
MENSAJE_FIN_X = 250
MENSAJE_FIN_Y_1 = 350
MENSAJE_FIN_Y_2 = 370

def cargar_niveles(ruta_niveles):
    """Recibe una cadena con la ruta del archivo que contiene los niveles y devuelve un diccionario con formato
    {"Level x": [nivel], ...}, donde "nivel" es una lista de cadenas"""
    niveles = {}
    with open(ruta_niveles, "r", encoding="utf-8") as archivo_niveles:
        for line in archivo_niveles:
            if line == "\n":
                continue
            linea = line.rstrip("\n")
            if "'" in linea:
                continue
            if "L" in linea:
                niveles[linea] = []
                llave = linea
            elif "#" in linea:
                niveles[llave].append(linea)
    return niveles
    
def cargar_configuracion(ruta_config):
    """Recibe una cadena con la ruta del archivo que contiene la configuración de las teclas y devuelve un diccionario
    con las teclas como llaves y su respectiva accion como valores"""
    teclas = {}
    with open(ruta_config, "r", encoding="utf-8") as archivo_config:
        for line in archivo_config:
            if line == "\n":
                continue
            linea = line.rstrip("\n")
            tecla = linea.split(" = ")
            teclas[tecla[1]] = teclas.get(tecla[1], [])
            teclas[tecla[1]].append(tecla[0])
    return teclas

def cargar_savefile(ruta_save):
    """ Recibe una cadena con la ruta del archivo que contiene el número del último nivel visitado y devuelve el valor
        del nivel correspondiente
    """
    try:
        archivo_save = open(ruta_save)
        line = archivo_save.readline()
        if line[0].isdigit():
            nivel = int(line.rstrip("\n"))
        line = archivo_save.readline()
        if line == "\n":
            ganado = False
        else:
            linea = line.rstrip("\n")
            if linea[7:] == "True":
                ganado = True
            else:
                ganado = False  
        archivo_save.close() 
    except FileNotFoundError:
        nivel = 1
        ganado = False
        
    return nivel, ganado
      
def convertir_a_cadena(estado):
    """ Recibe un estado de juego mutable y lo convierte en un estado inmutable, en este caso a una cadena
    """
    estado_cadena = ""
    for f in estado:
        for c in f:
            estado_cadena += c
            
    return estado_cadena

def buscar_solucion(estado_inicial):
    """ Utiliza el algoritmo de bracktraking para intentar encontrar una solución al nivel actual
    """
    visitados = {}
    return backtrack(estado_inicial, visitados)

def backtrack(estado, visitados):
    """ En cada ejecución trata de realizar todos los movimientos posibles con el estado que recibe, si encuentra la solución devuelve una
        pila con los movimientos que llevaron a esa solución, y False en caso contrario
    """
    direcciones = [ESTE, OESTE, NORTE, SUR]
    visitados[convertir_a_cadena(estado)] = True
    if soko.juego_ganado(estado):
        return True, Pila()
    for direccion in direcciones:
        nuevo_estado = soko.mover(estado, direccion)
        if visitados.get(convertir_a_cadena(nuevo_estado), False):
            continue
        solucion_encontrada, acciones = backtrack(nuevo_estado, visitados)
        if solucion_encontrada:
            acciones.apilar(nuevo_estado)
            return True, acciones
    return False, None

def texto_menu(nivel, ganado, cant_niveles):
    """
        Escribe el texto/botones necesarios en el menu principal
    """
    if nivel >= cant_niveles:
        cadena = "Juego terminado, nice"
    else:
        cadena = "Ultimo nivel visto, nivel " + str(nivel)

    gamelib.draw_rectangle(BOTON_NEW_GAME_SELECTOR_X_I, BOTON_Y_1_I, BOTON_NEW_GAME_X_F, BOTON_Y_1_F, fill="red", outline="white")
    gamelib.draw_text(NUEVO_JUEGO_TEXTO, BOTON_NEW_GAME_SELECTOR_X_I + AJUSTE_MENU, BOTON_Y_1_I, anchor="nw")

    gamelib.draw_rectangle(BOTON_ULTIMO_X_I, BOTON_Y_1_I, BOTON_ULTIMO_X_F, BOTON_Y_1_F, fill="red", outline="white")
    gamelib.draw_text(cadena, BOTON_ULTIMO_X_I + AJUSTE_MENU, BOTON_Y_1_I, anchor="nw")

    gamelib.draw_rectangle(BOTON_SALIR_X_I, BOTON_Y_2_I, BOTON_SALIR_X_F, BOTON_Y_2_F, fill="red", outline="white")
    gamelib.draw_text(SALIR_TEXTO,BOTON_SALIR_X_I + AJUSTE_MENU, BOTON_Y_2_I, anchor="nw")
    if ganado:
        gamelib.draw_rectangle(BOTON_NEW_GAME_SELECTOR_X_I, BOTON_Y_2_I, BOTON_SELECTOR_X_F, BOTON_Y_2_F, fill="red", outline="white")
        gamelib.draw_text(SELECCIONAR_NIVEL_TEXTO, BOTON_NEW_GAME_SELECTOR_X_I + AJUSTE_MENU, BOTON_Y_2_I, anchor="nw")

def what_button(x, y):
    """ 
        Recibe dos enteros indicando la posición del click y devuelve lo que se tiene que hacer
        de acuerdo con la posición tocada
    """
    if BOTON_Y_1_I <= y <= BOTON_Y_1_F:
        if BOTON_NEW_GAME_SELECTOR_X_I <= x <= BOTON_NEW_GAME_X_F:
            return True
        if BOTON_ULTIMO_X_I <= x <= BOTON_ULTIMO_X_F:
            return False

    if BOTON_Y_2_I <= y <= BOTON_Y_2_F:
        if BOTON_NEW_GAME_SELECTOR_X_I <= x <= BOTON_SELECTOR_X_F:
            return []
        if BOTON_SALIR_X_I <= x <= BOTON_SALIR_X_F:
            return {}
        
def main_menu(ventana_x, ventana_y, nivel, ganado, cant_niveles):
    """
        Un menu principal bastante rancio para que al menos puedas hacer algo/indicar
        en qué nivel querés empezar
    """
    gamelib.resize(ventana_x, ventana_y)
    
    while gamelib.is_alive():

        gamelib.draw_begin()
        texto_menu(nivel, ganado, cant_niveles)
        gamelib.draw_end()
        ev = gamelib.wait()
        if not ev:
            break
        tecla = ev.key
        if ev.type == gamelib.EventType.ButtonPress:
            x, y = ev.x, ev.y
            what_to_do = what_button(x, y)
            if what_to_do == True:
                nivel = 1
                return nivel
            if what_to_do == False:
                return nivel
            if what_to_do == []:
                if ganado:
                    while True:
                        entrada = gamelib.input(f"Ingrese el nivel por el cual quiere empezar, debe ser anterior o igual al nivel {cant_niveles}")
                        if not entrada:
                            break
                        if entrada.isdigit() and int(entrada) <= cant_niveles:
                            nivel = int(entrada)
                            return nivel

            if what_to_do == {}:
                return {}

def max_recursion(ventana_x, ventana_y, ascii_on):
    """ Algo para evitar que explote el programa cuando backtrack supera el límite de recursión de python y poder continuar
        con el nivel
    """
    while gamelib.is_alive():
        if ascii_on:
            ventana_x = VENTANA_RECURSION_ASCII
            ventana_y = VENTANA_RECURSION_ASCII
            
        gamelib.resize(ventana_x, ventana_y)
        gamelib.draw_begin()
        gamelib.draw_image(MIO_SCARED, (ventana_x // AJUSTE_MITAD) - MITAD_IMAGEN, ventana_y // AJUSTE_CUARTO)
        gamelib.draw_text(MENSAJE_REC_1, ventana_x // AJUSTE_MITAD, ventana_y // AJUSTE_Y_1, size=17)
        gamelib.draw_text(MENSAJE_REC_2, ventana_x // AJUSTE_MITAD, ventana_y // AJUSTE_Y_2, size=11)
        gamelib.draw_text(MENSAJE_REC_CONTINUAR, ventana_x // AJUSTE_MITAD, ventana_y // AJUSTE_Y_3, size=11)
        gamelib.draw_end()
        
        ev = gamelib.wait(gamelib.EventType.KeyPress)
        
        if not ev:
            break
        if ev.type == gamelib.EventType.KeyPress:
            return {}
            
def resize_ventana(dimensiones):
    """ Recibe una tupla con las dimensiones en x e y de la grilla y calcula un tamaño de ventana acorde, devolviendo los valores
        enteros de x e y
    """
    if dimensiones[0] >= LIMITE_CELDAS_50_X or dimensiones[1] >= LIMITE_CELDAS_50_Y:
        tamanio_ventana_x = (dimensiones[0] * LONGITUD_CELDA_X_M)
        tamanio_ventana_y = (dimensiones[1] * LONGITUD_CELDA_Y_M)
    elif dimensiones[0] >= LIMITE_CELDAS_75_X or dimensiones[1] >= LIMITE_CELDAS_75_Y:
        tamanio_ventana_x = (dimensiones[0] * LONGITUD_CELDA_X_C)
        tamanio_ventana_y = (dimensiones[1] * LONGITUD_CELDA_Y_C)
    else:    
        tamanio_ventana_x = dimensiones[0] * LONGITUD_CELDA_X
        tamanio_ventana_y = dimensiones[1] * LONGITUD_CELDA_Y
    
    return int(tamanio_ventana_x), int(tamanio_ventana_y)

def mostrar_grilla(grilla, max_x, max_y, nivel, pistas, pensando, recursion, dimensiones, ascii):
    """Imprime el estado actual de la grilla en pantalla"""
    
    x = 0 ; y = 0
    
    if ascii:
        x = 10 ; y = 10
        for f in grilla:
            for c in range(len(f)):
                if f[c] ==  soko.JUGADOR:
                    gamelib.draw_text(GROUND_A, x, y)
                    gamelib.draw_text(PLAYER_A, x, y)
                    
                elif f[c] == soko.CAJA:
                    gamelib.draw_text(GROUND_A, x, y)
                    gamelib.draw_text(BOX_A, x, y)

                elif f[c] == soko.OBJETIVO:
                    gamelib.draw_text(GROUND_A, x, y)
                    gamelib.draw_text(GOAL_A, x, y)

                elif f[c] == soko.PARED:
                    gamelib.draw_text(GROUND_A, x, y)
                    gamelib.draw_text(WALL_A, x, y)

                elif f[c] == soko.OBJETIVO_CAJA:
                    gamelib.draw_text(BOX_GOAL, x, y)

                elif f[c] == soko.OBJETIVO_JUGADOR:
                    gamelib.draw_text(PLAYER_GOAL, x, y)
                else:
                    gamelib.draw_text(GROUND_A, x, y)
                
                x += LONGITUD_CELDA_X_A
                    
            x = 10
            y += LONGITUD_CELDA_Y_A
            
        gamelib.draw_text(f"N{nivel}", (max_x // AJUSTE_MITAD) - AJUSTE_NIVEL, y + AJUSTE_NIVEL, size=11)
        
        if pensando:
            gamelib.draw_text(PENSANDO_A, max_x - AJUSTE_PISTAS_X_A, y + AJUSTE_PISTAS_Y, size=12)
            return
        if recursion:
            gamelib.draw_text(NO_MORE_A, max_x - AJUSTE_PISTAS_X_A, y + AJUSTE_PISTAS_Y, size=12)
        if not pistas.esta_vacia():
            if pistas.ver_tope() == NO_SOL:
                gamelib.draw_text(NO_SOL_TEXTO_A, max_x - AJUSTE_PISTAS_X_A, y + AJUSTE_PISTAS_Y, size=12)
                return
            gamelib.draw_text(PISTA_DISPONIBLE_A, max_x - AJUSTE_PISTAS_X_A, y + AJUSTE_PISTAS_Y, size=12)

    else:
        if dimensiones[0] >= LIMITE_CELDAS_50_X or dimensiones[1] >= LIMITE_CELDAS_50_Y:
            for f in grilla:
                for c in range(len(f)):
                    gamelib.draw_image(GROUND_32, x, y)
                    if f[c] ==  soko.JUGADOR:
                        gamelib.draw_image(PLAYER_32, x, y)
                        
                    elif f[c] == soko.CAJA:
                        gamelib.draw_image(BOX_32, x, y)

                    elif f[c] == soko.OBJETIVO:
                        gamelib.draw_image(GOAL_32, x, y)

                    elif f[c] == soko.PARED:
                        gamelib.draw_image(WALL_32, x, y)

                    elif f[c] == soko.OBJETIVO_CAJA:
                        gamelib.draw_image(BOX_32, x, y)
                        gamelib.draw_image(GOAL_32, x, y)

                    elif f[c] == soko.OBJETIVO_JUGADOR:
                        gamelib.draw_image(PLAYER_32, x, y)
                        gamelib.draw_image(GOAL_32, x, y)

                    
                    x += LONGITUD_CELDA_X_M

                    # Ya que el archivo niveles.txt no tiene los espacios al final de cada fila para hacer que el formato sea
                    # rectangular, debo imprimir las celdas vacías que estén fuera de la grilla
                    while c == (len(f) - 1) and x < max_x:
                        gamelib.draw_image(GROUND_32, x, y)
                        x += LONGITUD_CELDA_X_M
                        
                x = 0
                y += LONGITUD_CELDA_Y_M
        
        elif dimensiones[0] >= LIMITE_CELDAS_75_X or dimensiones[1] >= LIMITE_CELDAS_75_Y:
            for f in grilla:
                for c in range(len(f)):
                    gamelib.draw_image(GROUND_48, x, y)
                    if f[c] ==  soko.JUGADOR:
                        gamelib.draw_image(PLAYER_48, x, y)
                        
                    elif f[c] == soko.CAJA:
                        gamelib.draw_image(BOX_48, x, y)

                    elif f[c] == soko.OBJETIVO:
                        gamelib.draw_image(GOAL_48, x, y)

                    elif f[c] == soko.PARED:
                        gamelib.draw_image(WALL_48, x, y)

                    elif f[c] == soko.OBJETIVO_CAJA:
                        gamelib.draw_image(BOX_48, x, y)
                        gamelib.draw_image(GOAL_48, x, y)

                    elif f[c] == soko.OBJETIVO_JUGADOR:
                        gamelib.draw_image(PLAYER_48, x, y)
                        gamelib.draw_image(GOAL_48, x, y)
                    
                    x += LONGITUD_CELDA_X_C
                    
                    while c == (len(f) - 1) and x < max_x:
                        gamelib.draw_image(GROUND_48, x, y)
                        x += LONGITUD_CELDA_X_C
                        
                x = 0
                y += LONGITUD_CELDA_Y_C
                
        else:
            for f in grilla:
                for c in range(len(f)):
                    gamelib.draw_image(GROUND, x, y)
                    if f[c] ==  soko.JUGADOR:
                        gamelib.draw_image(PLAYER, x, y)
                        
                    elif f[c] == soko.CAJA:
                        gamelib.draw_image(BOX, x, y)

                    elif f[c] == soko.OBJETIVO:
                        gamelib.draw_image(GOAL, x, y)

                    elif f[c] == soko.PARED:
                        gamelib.draw_image(WALL, x, y)

                    elif f[c] == soko.OBJETIVO_CAJA:
                        gamelib.draw_image(BOX, x, y)
                        gamelib.draw_image(GOAL, x, y)

                    elif f[c] == soko.OBJETIVO_JUGADOR:
                        gamelib.draw_image(PLAYER, x, y)
                        gamelib.draw_image(GOAL, x, y)
                    
                    x += LONGITUD_CELDA_X

                    # Ya que el archivo niveles.txt no tiene los espacios al final de cada fila para hacer que el formato sea
                    # rectangular, debo imprimir las celdas vacías que estén fuera de la grilla
                    while c == (len(f) - 1) and x < max_x:
                        gamelib.draw_image(GROUND, x, y)
                        x += LONGITUD_CELDA_X        
                        
                x = 0
                y += LONGITUD_CELDA_Y
    
        gamelib.draw_text(f"Nivel {nivel}", (max_x // AJUSTE_MITAD) - AJUSTE_NIVEL, y + AJUSTE_NIVEL, size=11)
            
        if pensando:
            gamelib.draw_text(PENSANDO, max_x - AJUSTE_PISTAS_X, y + AJUSTE_PISTAS_Y, size=12)
            return
        if recursion:
            gamelib.draw_text(NO_MORE, max_x - AJUSTE_PISTAS_X, y + AJUSTE_PISTAS_Y, size=12)
        if not pistas.esta_vacia():
            if pistas.ver_tope() == NO_SOL:
                gamelib.draw_text(NO_SOL_TEXTO, max_x - AJUSTE_PISTAS_X, y + AJUSTE_PISTAS_Y, size=12)
                return
            gamelib.draw_text(PISTA_DISPONIBLE, max_x - AJUSTE_PISTAS_X, y + AJUSTE_PISTAS_Y, size=12)

def main():
    # Inicializar el estado del juego
    niveles = cargar_niveles(RUTA_NIVELES)
    cant_niveles = len(niveles)
    teclas = cargar_configuracion(RUTA_TECLAS)

    nivel, ganado = cargar_savefile(RUTA_SAVE)
    nivel = main_menu(VENTANA_MENU_X, VENTANA_MENU_Y, nivel, ganado, cant_niveles)
    if nivel == {}:
        return {}
    debug = "debug"
    debug_on = False
    ascii = "ascii"
    ascii_on = False
    off = "off"
    codigo = []
    
    estados_pasados = Pila()
    pistas = Pila()
    
    pensando = False
    recursion = False
    
    # Todo está dentro de un while loop para que la progresión de niveles funcione y poder modificar el valor de esa
    # variable cuando desee
    
    while nivel <= cant_niveles:
        
        # Evito que el juego explote al retroceder o avanzar a un nivel inexistente, además de solo mostrar los niveles que
        # cumplan con el formato especificado de "Level (numero)"
        try:
            nivel_actual = soko.crear_grilla(niveles[f"Level {nivel}"])
        except KeyError:
            nivel = 1
            
        estado_anterior = nivel_actual
        estados_pasados.apilar(estado_anterior)
        # Las dimensiones de la ventana se ajustan a las dimensiones del nivel actual
        dimensiones = soko.dimensiones(nivel_actual)
        # También se ajustan a la cantidad que tenga y se modifican acorde a eso
        tamanio_ventana_x, tamanio_ventana_y = resize_ventana(dimensiones)
        gamelib.resize(tamanio_ventana_x, tamanio_ventana_y + ALTURA_BARRA_INDICADORES)

        while gamelib.is_alive():
            
            if ascii_on:
                tamanio_ventana_x = (dimensiones[0] * LONGITUD_CELDA_X_A)
                tamanio_ventana_y = (dimensiones[1] * LONGITUD_CELDA_Y_A)
                gamelib.resize(tamanio_ventana_x, tamanio_ventana_y + ALTURA_BARRA_INDICADORES_A)
                
            gamelib.draw_begin()
            # Dibujar la pantalla
            mostrar_grilla(nivel_actual, tamanio_ventana_x, tamanio_ventana_y, nivel, pistas, pensando, recursion, dimensiones, ascii_on)
            gamelib.draw_end()
            
            ev = gamelib.wait(gamelib.EventType.KeyPress)
            
            if not ev:
                break
            
            # Actualizar el estado del juego, según la `tecla` presionada
            
            tecla = ev.key
            
            # Activar las teclas especiales para saltarse y retroceder un nivel
            
            if not off in "".join(codigo) or not debug in "".join(codigo) or not ascii in "".join(codigo):
                codigo.append(tecla)
                if len(codigo) > 15:
                    codigo.clear()
                elif debug in "".join(codigo):
                    debug_on = True
                    codigo.clear()
                elif ascii in "".join(codigo):
                    ascii_on = True
                    codigo.clear()
                elif off in "".join(codigo):
                    ascii_on = False
                    tamanio_ventana_x, tamanio_ventana_y = resize_ventana(dimensiones)
                    gamelib.resize(tamanio_ventana_x, tamanio_ventana_y + ALTURA_BARRA_INDICADORES)
                    codigo.clear()
                    
            # Salir, reiniciar, deshacer y pistas
            # En todos los casos necesarios vacío las pilas de pistas y estados anteriores
            
            if ev.type == gamelib.EventType.KeyPress and tecla == teclas["SALIR"][0]:
                with open(RUTA_SAVE, "w") as save:
                    save.write(str(nivel) + "\n")
                    if ganado:
                        save.write("Ganado=True\n")
                    else:
                        save.write("Ganado=False\n")
                return 
                
            if ev.type == gamelib.EventType.KeyPress and tecla == teclas["REINICIAR"][0]:
                estados_pasados.vaciar()
                pistas.vaciar()
                recursion = False
                break
            
            if ev.type == gamelib.EventType.KeyPress and tecla == teclas["DESHACER"][0]:
                if estados_pasados.esta_vacia():
                    break
                nivel_actual = estados_pasados.desapilar()
                estado_anterior = nivel_actual
                pistas.vaciar()
                continue
            
            if ev.type == gamelib.EventType.KeyPress and tecla == teclas["PISTAS"][0]:    
                if recursion:
                    gamelib.draw_begin()
                    mostrar_grilla(nivel_actual, tamanio_ventana_x, tamanio_ventana_y, nivel, pistas, pensando, recursion, dimensiones, ascii_on)
                    gamelib.draw_end()
                    continue
                
                if pistas.esta_vacia():
                    pensando = True
                    gamelib.draw_begin()
                    mostrar_grilla(nivel_actual, tamanio_ventana_x, tamanio_ventana_y, nivel, pistas, pensando, recursion, dimensiones, ascii_on)
                    gamelib.draw_end()
                    pensando = False
                    try:
                        _, pistas = buscar_solucion(nivel_actual)
                    except RecursionError:
                        recursion = True
                        max_recursion(tamanio_ventana_x, tamanio_ventana_y + ALTURA_BARRA_INDICADORES, ascii_on)
                        continue
                    # Ya que la función devuelve "None" cuando no encuentra la solución, antes de continuar lo "atrapo" y lo convierto
                    # a una pila con un dato que indica que no hay solución,
                    # con esto evito que el programa explote al llamar a métodos de Pila en un None
                    if pistas == None:
                        pistas = Pila()
                        pistas.apilar(NO_SOL)
                        continue
                else:
                    if pistas.ver_tope() == NO_SOL:
                        continue
                    nivel_actual = pistas.desapilar()
                    estado_anterior = nivel_actual
                        
            # Estas dos son para debuggear, por eso no aparecen en el archivo "teclas.txt"
            # Se activan cuando se tipea el código "debug"
            
            if ev.type == gamelib.EventType.KeyPress and tecla == ANTERIOR_NIVEL and debug_on:
                # Retrocede un nivel
                nivel -= 1
                estados_pasados.vaciar()
                pistas.vaciar()
                recursion = False
                break
            
            if ev.type == gamelib.EventType.KeyPress and tecla == SIGUIENTE_NIVEL and debug_on:
                # Avanza un nivel
                nivel += 1
                estados_pasados.vaciar()
                pistas.vaciar()
                recursion = False
                break
            
            # Movimiento
            
            if ev.type == gamelib.EventType.KeyPress and tecla in teclas["NORTE"]:
                nivel_actual = soko.mover(nivel_actual, NORTE)
            
            if ev.type == gamelib.EventType.KeyPress and tecla in teclas["SUR"]:
                nivel_actual = soko.mover(nivel_actual, SUR)
            
            if ev.type == gamelib.EventType.KeyPress and tecla in teclas["OESTE"]:
                nivel_actual = soko.mover(nivel_actual, OESTE)

            if ev.type == gamelib.EventType.KeyPress and tecla in teclas["ESTE"]:
                nivel_actual = soko.mover(nivel_actual, ESTE)
            
            # Si apreto una tecla de movimiento se debe vaciar la pila de pistas porque no se puede asegurar que esas pistas sigan siendo
            # válidas con otro estado inicial
            
            if ev.type == gamelib.EventType.KeyPress and tecla in teclas["ESTE"] or tecla in teclas["OESTE"] or tecla in teclas["SUR"] or tecla in teclas["NORTE"]:
                pistas.vaciar()
            
            # Verifica si el juego está ganado, de ser así avanza al siguiente nivel y vacía las pilas de pistas y movimientos anteriores
            
            if soko.juego_ganado(nivel_actual):
                nivel += 1
                estados_pasados.vaciar()
                pistas.vaciar()
                recursion = False
                break
            
            # Antes de volver a ejecutar el loop apilo el estado anterior al actual y sobreescribo el estado anterior con el actual
            
            if estados_pasados.esta_vacia() or estados_pasados.ver_tope() != estado_anterior:
                estados_pasados.apilar(estado_anterior)
            estado_anterior = nivel_actual
         
            
    # Algo al finalizar todos los niveles para que no cierre abruptamente
    
    while gamelib.is_alive():
    
        gamelib.resize(VENTANA_FINAL_X, VENTANA_FINAL_Y)
        gamelib.draw_begin()
        gamelib.draw_image(END_KANNA, KANNA_X, KANNA_Y)
        gamelib.draw_text(MENSAJE_FIN_1, MENSAJE_FIN_X, MENSAJE_FIN_Y_1, size=15)
        gamelib.draw_text(MENSAJE_FIN_2, MENSAJE_FIN_X, MENSAJE_FIN_Y_2, size=15)
        gamelib.draw_end()
        
        
        ev = gamelib.wait(gamelib.EventType.KeyPress)
        
        if not ev:
            break
        if ev.type == gamelib.EventType.KeyPress:
            with open(RUTA_SAVE, "w", encoding="utf-8") as save:
                save.write(str(nivel) + "\n")
                save.write("Ganado=True\n")
            return {}

gamelib.init(main)
