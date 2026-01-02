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

    //Arreglos de rutas
    const char *kirby_walk_r[] = {
        "Kirby/Kirby4.bmp",
        "Kirby/Kirby24.bmp",
        "Kirby/Kirby5.bmp",
        "Kirby/Kirby6.bmp"
    };

    const char *kirby_walk_r_mask[] = {
        "Kirby/Kirby4mask.bmp",
        "Kirby/Kirby24mask.bmp",
        "Kirby/Kirby5mask.bmp",
        "Kirby/Kirby6mask.bmp"
    };

    const char *kirby_eat_r[] = {
        "Kirby/Kirby7.bmp",
        "Kirby/Kirby8.bmp",
        "Kirby/Kirby23.bmp",
        "Kirby/Kirby22.bmp",
        "Kirby/Kirby19.bmp"
    };

    const char *kirby_eat_r_mask[] = {
        "Kirby/Kirby7mask.bmp",
        "Kirby/Kirby8mask.bmp",
        "Kirby/Kirby23mask.bmp",
        "Kirby/Kirby22mask.bmp",
        "Kirby/Kirby19mask.bmp" 
    };

    const char* kirbyfat_walk_r[] = {
        "Kirby/Kirby17.bmp",
        "Kirby/Kirby18.bmp"
    };

    const char* kirbyfat_walk_r_mask[] = {
        "Kirby/Kirby17mask.bmp",
        "Kirby/Kirby18mask.bmp"
    };

    const char* kirbyfatair_walk_r[] = {
        "Kirby/Kirby14.bmp",
        "Kirby/Kirby15.bmp"
    };

    const char* kirbyfatair_walk_r_mask[] = {
        "Kirby/Kirby14mask.bmp",
        "Kirby/Kirby15mask.bmp"
    };

    const char* kirby_spit_r[] = {
        "Kirby/Kirby13.bmp",
        "Kirby/Kirby8.bmp",
        "Kirby/Kirby7.bmp"
    };

    const char* kirby_spit_r_mask[] = {
        "Kirby/Kirby13mask.bmp",
        "Kirby/Kirby8mask.bmp",
        "Kirby/Kirby7mask.bmp"
    };

    const char* kirby_idle_r[] = {
        "Kirby/Kirby.bmp",
        "Kirby/Kirby.bmp"
    };

    const char* kirby_idle_r_mask[] = {
        "Kirby/Kirbymask.bmp",
        "Kirby/Kirbymask.bmp"
    };

    const char* kirby_jump_r[] = {
        "Kirby/Kirby2.bmp",
        "Kirby/Kirby2.bmp"
    };

    const char* kirby_jump_r_mask[] = {
        "Kirby/Kirby2mask.bmp",
        "Kirby/Kirby2mask.bmp"
    };

    const char* kirbyfat_jump_r[] = {
        "Kirby/Kirby19.bmp",
        "Kirby/Kirby19.bmp"
    };

    const char* kirbyfat_jump_r_mask[] = {
        "Kirby/Kirby19mask.bmp",
        "Kirby/Kirby19mask.bmp"
    };

    const char* kirbyfat_idle_r[] = {
        "Kirby/Kirby16.bmp",
        "Kirby/Kirby16.bmp"
    };

    const char* kirbyfat_idle_r_mask[] = {
        "Kirby/Kirby16mask.bmp",
        "Kirby/Kirby16mask.bmp"
    };
    //Configuraciones de la ventana
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800,600);

    //Animaciones de los enemigos
    Animacion enemianimbronto_l;

    Imagen *sprite; //Imagen que utilizaremos constantemente

    Kirby kirby;

    //Sprites de Kirby
    cargar_animacion(kirbyfat_jump_r,kirbyfat_jump_r_mask,2,2,&kirby.arregloAnim[AN_FAT_FALL]);
    kirby.arregloAnim[AN_FAT_FALL].bucle = true; //True se repite la animacion, false no

    cargar_animacion(kirby_idle_r,kirby_idle_r_mask,2,2,&kirby.arregloAnim[AN_IDLE]);
    kirby.arregloAnim[AN_IDLE].bucle = true;

    cargar_animacion(kirby_jump_r,kirby_jump_r_mask,2,2,&kirby.arregloAnim[AN_JUMP]);
    kirby.arregloAnim[AN_JUMP].bucle = true;

    cargar_animacion(kirby_walk_r,kirby_walk_r_mask,4,3,&kirby.arregloAnim[AN_WALK]);
    kirby.arregloAnim[AN_WALK].bucle = true;

    cargar_animacion(kirby_eat_r,kirby_eat_r_mask,5,7,&kirby.arregloAnim[AN_EAT]);
    kirby.arregloAnim[AN_EAT].bucle = false;

    cargar_animacion(kirbyfat_idle_r,kirby_idle_r_mask,2,2,&kirby.arregloAnim[AN_FAT_IDLE]);
    kirby.arregloAnim[AN_FAT_IDLE].bucle = true;

    cargar_animacion(kirbyfat_walk_r,kirbyfat_walk_r_mask,2,2,&kirby.arregloAnim[AN_FAT_WALK]);
    kirby.arregloAnim[AN_FAT_WALK].bucle = true;

    cargar_animacion(kirbyfatair_walk_r,kirbyfatair_walk_r_mask,2,7,&kirby.arregloAnim[AN_FLY]);
    kirby.arregloAnim[AN_FLY].bucle = false;

    cargar_animacion(kirby_spit_r,kirby_spit_r_mask,3,8,&kirby.arregloAnim[AN_SPIT]);
    kirby.arregloAnim[AN_SPIT].bucle = false;

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