import gamelib

def what_button(x, y):
    if 20 <= x <= 120 and 30 <= y <= 50:
        return "hello"
    if 220 <= x <= 420 and 30 <= y <= 50:
        return "Continue"
    if 420 <= x <= 490 and 200 <= y <= 220:
        return "chau"
        

def test_menu():
    ventana_x = 500
    ventana_y = 300
    
    gamelib.resize(ventana_x, ventana_y)
    nivel = 3
    cadena = "Continue last level, level " + str(nivel)
    while gamelib.is_alive():
        gamelib.draw_begin()
        gamelib.draw_rectangle(20, 30, 120, 50, fill="red", outline="white")
        gamelib.draw_text("New game", 30, 30, anchor="nw")
        gamelib.draw_rectangle(220, 30, 420, 50, fill="red", outline="white")
        gamelib.draw_text(cadena, 230, 30, anchor="nw")
        gamelib.draw_rectangle(420, 200, 470, 220, fill="red", outline="white")
        gamelib.draw_text("quit",430, 200, anchor="nw")
        gamelib.draw_end()
        ev = gamelib.wait(gamelib.EventType.ButtonPress)
        if not ev:
            break
        if ev.type == gamelib.EventType.ButtonPress:
            x, y = ev.x, ev.y
            what_to_do = what_button(x, y)
            if what_to_do == "chau":
                return {}
        
gamelib.init(test_menu)