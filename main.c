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

    //Arreglos de rutas de Kirby
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
        "Kirby/Kirby23.bmp",
        "Kirby/Kirby22.bmp",
        "Kirby/Kirby19.bmp"
    };

    const char *kirby_eat_r_mask[] = {
        "Kirby/Kirby23mask.bmp",
        "Kirby/Kirby22mask.bmp",
        "Kirby/Kirby19mask.bmp" 
    };

    const char *kirby_inhale_r[] = {
        "Kirby/Kirby7.bmp",
        "Kirby/Kirby8.bmp"
    };

        const char *kirby_inhale_r_mask[] = {
        "Kirby/Kirby7mask.bmp",
        "Kirby/Kirby8mask.bmp"
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

    const char* humo_r[] = {
        "Kirby/Nube.bmp",
        "Kirby/Nube.bmp"
    };

    const char* humo_r_mask[] = {
        "Kirby/Nubemask.bmp",
        "Kirby/Nubemask.bmp"
    };

    const char* estrella[] = {
        "Kirby/Estrellita.bmp",
        "Kirby/Estrella2.bmp",
        "Kirby/Estrella.bmp"
    };

    const char* estrella_mask[] = {
        "Kirby/Estrellitamask.bmp",
        "Kirby/Estrella2mask.bmp",
        "Kirby/Estrellamask.bmp"
    };

    //-----------------------------------------Arreglos de rutas de Kirby en espejo---------------------------------------------------------

        const char *kirby_walk_r_mirror[] = {
        "Kirbymirror/Kirby4.bmp",
        "Kirbymirror/Kirby24.bmp",
        "Kirbymirror/Kirby5.bmp",
        "Kirbymirror/Kirby6.bmp"
    };

    const char *kirby_walk_r_mask_mirror[] = {
        "Kirbymirror/Kirby4mask.bmp",
        "Kirbymirror/Kirby24mask.bmp",
        "Kirbymirror/Kirby5mask.bmp",
        "Kirbymirror/Kirby6mask.bmp"
    };

    const char *kirby_eat_r_mirror[] = {
        "Kirbymirror/Kirby23.bmp",
        "Kirbymirror/Kirby22.bmp",
        "Kirbymirror/Kirby19.bmp"
    };

    const char *kirby_eat_r_mask_mirror[] = {
        "Kirbymirror/Kirby23mask.bmp",
        "Kirbymirror/Kirby22mask.bmp",
        "Kirbymirror/Kirby19mask.bmp" 
    };

    const char *kirby_inhale_r_mirror[] = {
        "Kirbymirror/Kirby7.bmp",
        "Kirbymirror/Kirby8.bmp",
    };

        const char *kirby_inhale_r_mask_mirror[] = {
        "Kirbymirror/Kirby7mask.bmp",
        "Kirbymirror/Kirby8mask.bmp",
    };

    const char* kirbyfat_walk_r_mirror[] = {
        "Kirbymirror/Kirby17.bmp",
        "Kirbymirror/Kirby18.bmp"
    };

    const char* kirbyfat_walk_r_mask_mirror[] = {
        "Kirbymirror/Kirby17mask.bmp",
        "Kirbymirror/Kirby18mask.bmp"
    };

    const char* kirbyfatair_walk_r_mirror[] = {
        "Kirbymirror/Kirby14.bmp",
        "Kirbymirror/Kirby15.bmp"
    };

    const char* kirbyfatair_walk_r_mask_mirror[] = {
        "Kirbymirror/Kirby14mask.bmp",
        "Kirbymirror/Kirby15mask.bmp"
    };

    const char* kirby_spit_r_mirror[] = {
        "Kirbymirror/Kirby13.bmp",
        "Kirbymirror/Kirby8.bmp",
        "Kirbymirror/Kirby7.bmp"
    };

    const char* kirby_spit_r_mask_mirror[] = {
        "Kirbymirror/Kirby13mask.bmp",
        "Kirbymirror/Kirby8mask.bmp",
        "Kirbymirror/Kirby7mask.bmp"
    };

    const char* kirby_idle_r_mirror[] = {
        "Kirbymirror/Kirby.bmp",
        "Kirbymirror/Kirby.bmp"
    };

    const char* kirby_idle_r_mask_mirror[] = {
        "Kirbymirror/Kirbymask.bmp",
        "Kirbymirror/Kirbymask.bmp"
    };

    const char* kirby_jump_r_mirror[] = {
        "Kirbymirror/Kirby2.bmp",
        "Kirbymirror/Kirby2.bmp"
    };

    const char* kirby_jump_r_mask_mirror[] = {
        "Kirbymirror/Kirby2mask.bmp",
        "Kirbymirror/Kirby2mask.bmp"
    };

    const char* kirbyfat_jump_r_mirror[] = {
        "Kirbymirror/Kirby19.bmp",
        "Kirbymirror/Kirby19.bmp"
    };

    const char* kirbyfat_jump_r_mask_mirror[] = {
        "Kirbymirror/Kirby19mask.bmp",
        "Kirbymirror/Kirby19mask.bmp"
    };

    const char* kirbyfat_idle_r_mirror[] = {
        "Kirbymirror/Kirby16.bmp",
        "Kirbymirror/Kirby16.bmp"
    };

    const char* kirbyfat_idle_r_mask_mirror[] = {
        "Kirbymirror/Kirby16mask.bmp",
        "Kirbymirror/Kirby16mask.bmp"
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

    cargar_animacion(kirby_eat_r,kirby_eat_r_mask,3,5,&kirby.arregloAnim[AN_EAT]);
    kirby.arregloAnim[AN_EAT].bucle = false;

    cargar_animacion(kirbyfat_idle_r,kirby_idle_r_mask,2,2,&kirby.arregloAnim[AN_FAT_IDLE]);
    kirby.arregloAnim[AN_FAT_IDLE].bucle = true;

    cargar_animacion(kirbyfat_walk_r,kirbyfat_walk_r_mask,2,2,&kirby.arregloAnim[AN_FAT_WALK]);
    kirby.arregloAnim[AN_FAT_WALK].bucle = true;

    cargar_animacion(kirbyfatair_walk_r,kirbyfatair_walk_r_mask,2,7,&kirby.arregloAnim[AN_FLY]);
    kirby.arregloAnim[AN_FLY].bucle = false;

    cargar_animacion(kirby_spit_r,kirby_spit_r_mask,3,8,&kirby.arregloAnim[AN_SPIT]);
    kirby.arregloAnim[AN_SPIT].bucle = false;

    cargar_animacion(kirby_inhale_r,kirby_inhale_r_mask,2,2,&kirby.arregloAnim[AN_INHALE]);
    kirby.arregloAnim[AN_INHALE].bucle = true;

    //Sprites de kirby en mirror
    cargar_animacion(kirbyfat_jump_r_mirror,kirbyfat_jump_r_mask_mirror,2,2,&kirby.arregloAnimMirror[AN_FAT_FALL]);
    kirby.arregloAnimMirror[AN_FAT_FALL].bucle = true;

    cargar_animacion(kirby_idle_r_mirror,kirby_idle_r_mask_mirror,2,2,&kirby.arregloAnimMirror[AN_IDLE]);
    kirby.arregloAnimMirror[AN_IDLE].bucle = true;

    cargar_animacion(kirby_jump_r_mirror,kirby_jump_r_mask_mirror,2,2,&kirby.arregloAnimMirror[AN_JUMP]);
    kirby.arregloAnimMirror[AN_JUMP].bucle = true;

    cargar_animacion(kirby_walk_r_mirror,kirby_walk_r_mask_mirror,4,3,&kirby.arregloAnimMirror[AN_WALK]);
    kirby.arregloAnimMirror[AN_WALK].bucle = true;

    cargar_animacion(kirby_eat_r_mirror,kirby_eat_r_mask_mirror,3,5,&kirby.arregloAnimMirror[AN_EAT]);
    kirby.arregloAnimMirror[AN_EAT].bucle = false;

    cargar_animacion(kirbyfat_idle_r_mirror,kirby_idle_r_mask_mirror,2,2,&kirby.arregloAnimMirror[AN_FAT_IDLE]);
    kirby.arregloAnimMirror[AN_FAT_IDLE].bucle = true;

    cargar_animacion(kirbyfat_walk_r_mirror,kirbyfat_walk_r_mask_mirror,2,2,&kirby.arregloAnimMirror[AN_FAT_WALK]);
    kirby.arregloAnimMirror[AN_FAT_WALK].bucle = true;

    cargar_animacion(kirbyfatair_walk_r_mirror,kirbyfatair_walk_r_mask_mirror,2,7,&kirby.arregloAnimMirror[AN_FLY]);
    kirby.arregloAnimMirror[AN_FLY].bucle = false;

    cargar_animacion(kirby_spit_r_mirror,kirby_spit_r_mask_mirror,3,8,&kirby.arregloAnimMirror[AN_SPIT]);
    kirby.arregloAnimMirror[AN_SPIT].bucle = false;

    cargar_animacion(kirby_inhale_r_mirror,kirby_inhale_r_mask_mirror,2,2,&kirby.arregloAnimMirror[AN_INHALE]);
    kirby.arregloAnimMirror[AN_INHALE].bucle = true;    

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