/**
 * @file main.c
 * @author Moreno Silva Victor Emiliano (vmorenos2200@alumno.ipn.mx)
 * @brief Kirbys Dreamland
 * @version 0.1
 * @date 2025-12-22A
 * 
 * @copyright Copyright (c) 2025
 * 
 * Numero de boleta 2026640066
 * Grupo: 1MM3
 */
#include "graficos.h"
#include "serial.h"
#include "simplecontroller.h"
#include "control_esp.h"
#include "kirby_assets.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//Pines para el ESP32
#define JX 35
#define JY 34
#define MTR 19
#define BUTTON_JUMP 26
#define BUTTON_ACTION 25
#define BUTTON_DOWN 27

int main()
{
    //Elementos para imagenes
    const int ESCALA = 65;
    const int DELAY = 50;

    const char *bronto_walk[] = {
        "Enemigos/Bronto.bmp",
        "Enemigos/Bronto2.bmp"
    }; 

    const char *bronto_walk_mask[] = {
        "Enemigos/Brontomask.bmp",
        "Enemigos/Bronto2mask.bmp"        
    };

    const char *cappy_walk[] = {
        "Enemigos/Cappy.bmp",
        "Enemigos/Cappy2.bmp",
        "Enemigos/Cappy3.bmp"
    };

    const char *cappy_walk_mask[] = {
        "Enemigos/Cappymask.bmp",
        "Enemigos/Cappy2mask.bmp",
        "Enemigos/Cappy3mask.bmp"
    };

    const char *grizzo[] = {
        "Enemigos/Grizzo.bmp",
        "Enemigos/Grizzo2.bmp"
    };

    const char *grizzo_mask[] = {
        "Enemigos/Grizzomask.bmp",
        "Enemigos/Grizzo2mask.bmp"
    };

    const char* manzana[] = {
        "Enemigos/Manzana.bmp",
        "Enemigos/Manzana2.bmp",
        "Enemigos/Manzana3.bmp",
        "Enemigos/Manzana4.bmp"
    };

    const char* manzana_mask[] = {
        "Enemigos/Manzanamask.bmp",
        "Enemigos/Manzana2mask.bmp",
        "Enemigos/Manzana3mask.bmp",
        "Enemigos/Manzana4mask.bmp"
    };

    const char* poppybros[] = {
        "Enemigos/Poppybros.bmp",
        "Enemigos/Poppybros2.bmp"
    };

    const char* poppybros_mask[] = {
        "Enemigos/Poppybrosmask.bmp",
        "Enemigos/Poppybros2mask.bmp"
    };

    const char* twizzy[] = {
        "Enemigos/Twizzy.bmp",
        "Enemigos/Twizzy2.bmp"
    };

    const char* twizzy_mask[] = {
        "Enemigos/Twizzymask.bmp",
        "Enemigos/Twizzy2mask.bmp"
    };

    const char* wadledee[] = {
        "Enemigos/WadleDee.bmp",
        "Enemigos/WadleDee2.bmp"
    };

    const char* wadledee_mask[] = {
        "Enemigos/WadleDeemask.bmp",
        "Enemigos/WadleDee2mask.bmp"
    };

    //Configuraciones de la ventana
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800,600);

    //Animaciones de los enemigos
    Animacion enemianimbronto_l;

    Imagen *sprite; //Imagen que utilizaremos constantemente

    Kirby kirby;

    cargar_animaciones_kirby(&kirby);

    cargar_animacionesmirror_kirby(&kirby);

    ini_kirby(&kirby, 100, ventana.altoVentana() - 50);    

    //Sprites de Enemigos
    cargar_animacion(bronto_walk,bronto_walk_mask,2,2,&enemianimbronto_l);

    //Configuraciones y calibraciones del Esp32
    Board *esp32 = connectDevice("COM6",B230400);
    esp32->pinMode(esp32,JX,INPUT);
    esp32->pinMode(esp32,JY,INPUT);
    esp32->pinMode(esp32,MTR,OUTPUT);
    esp32->pinMode(esp32,BUTTON_JUMP,INPUT_PULLUP);
    esp32->pinMode(esp32,BUTTON_ACTION,INPUT_PULLUP);
    esp32->pinMode(esp32,BUTTON_DOWN,INPUT_PULLUP);

    EjeJoystick EjeX;
    EjeJoystick EjeY;

    Lectura lectura_general;

    ini_joystick(&EjeX,esp32,JX);
    ini_joystick(&EjeY,esp32,JY);

    ini_lens(&lectura_general,esp32,BUTTON_ACTION,BUTTON_JUMP,BUTTON_DOWN,MTR);

    ventana.colorFondo(COLORES.NEGRO);
    ventana.limpiaVentana();
    ventana.actualizaVentana();
    ventana.espera(100);

    ini_calibracion(&EjeX,500);
    ini_calibracion(&EjeY,500);

    Animacion *anim_act;

while (1)
{
    static bool last_mtr = false;

    leer_entrada(&lectura_general, &EjeX, true);

    kirby.action_p = fue_presionado(&lectura_general.button_action,lectura_general.board);

    kirby.jump_p = fue_presionado(&lectura_general.button_jump,lectura_general.board);

    actualizar_kirby(&kirby,&lectura_general,DT,ventana.anchoVentana(),ventana.altoVentana());

    if (lectura_general.mtr_on != last_mtr) 
    {
    lectura_general.board->digitalWrite(lectura_general.board,lectura_general.pin_mtr,lectura_general.mtr_on);
    last_mtr = lectura_general.mtr_on;
    }

    seleccionar_animacion_kirby(&kirby);

    sprite = animacion_actual(kirby.animActual);

    ventana.limpiaVentana();
    ventana.muestraImagenEscalada((int)kirby.x, (int)kirby.y, ESCALA, ESCALA, sprite);
    ventana.actualizaVentana();
    ventana.espera(16);
}
return 0;
}