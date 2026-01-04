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

    //Configuraciones de la ventana
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800,600);

    Imagen *sprite; //Imagen que utilizaremos constantemente

    Kirby kirby;

    Enemigo enemies;

    cargar_animaciones_enemies(&enemies);

    cargar_animaciones_kirby(&kirby);

    cargar_animacionesmirror_kirby(&kirby);

    ini_kirby(&kirby, 100, ventana.altoVentana() - 50);    

    enemies.x = 400.0f;
    enemies.y = ventana.altoVentana() - 50.0f;
    enemies.activo = true;
    enemies.typeenemie = EN_WADLE;

    actualizar_hitbox_enemie(&enemies);

    seleccionar_enemies(&enemies);
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
    //Logica
    static bool last_mtr = false;

    leer_entrada(&lectura_general, &EjeX, true);

    kirby.action_p = fue_presionado(&lectura_general.button_action,lectura_general.board);

    kirby.jump_p = fue_presionado(&lectura_general.button_jump,lectura_general.board);

    actualizar_kirby(&kirby,&lectura_general,DT,ventana.anchoVentana(),ventana.altoVentana());

    if (kirby.estado == ST_INHALE && enemies.activo)
    {
        if (kirby.timerAccion >= 0.2f && kirby.timerAccion < 0.4f)
        {
            if (hitbox_colision(kirby.succionhitbox,enemies.hitbox))
            {
                enemies.activo = false;
                kirby.stomach_wenemie = true;
                kirby.enemies_eaten++;
            }
        }
    }

    if (lectura_general.mtr_on != last_mtr) 
    {
    lectura_general.board->digitalWrite(lectura_general.board,lectura_general.pin_mtr,lectura_general.mtr_on);
    last_mtr = lectura_general.mtr_on;
    }

    seleccionar_animacion_kirby(&kirby);

    sprite = animacion_actual(kirby.animActual);

    actualizar_proyectil(&kirby.proyectil,DT);

    if (kirby.proyectil.activo && enemies.activo)
    {
        if (hitbox_colision(kirby.proyectil.hitbox,enemies.hitbox))
        {
            if (kirby.proyectil.esEstrella)
            {
                enemies.activo = false;
            }
            else
            {
                enemies.x += kirby.proyectil.velX * 0.1f;
                actualizar_hitbox_enemie(&enemies);
            }
            kirby.proyectil.activo = false;
        } 
    }
    
    //Dibujo
    ventana.limpiaVentana();
    if (enemies.activo)
    {
        ventana.muestraImagenEscalada((int)enemies.x,(int)enemies.y,ESCALA,ESCALA,animacion_actual(enemies.animActual));
    }
    if (kirby.proyectil.activo)
    {
        seleccionar_proyectil(&kirby);
        if(kirby.proyectil.esEstrella)
        {ventana.muestraImagenEscalada((int)kirby.proyectil.x,kirby.proyectil.y,ESCALA-35,ESCALA-35,animacion_actual(kirby.proyectil.animActual));}
        else
        {ventana.muestraImagenEscalada((int)kirby.proyectil.x,kirby.proyectil.y,ESCALA-25,ESCALA-25,animacion_actual(kirby.proyectil.animActual));}
    }
    
    ventana.muestraImagenEscalada((int)kirby.x, (int)kirby.y, ESCALA, ESCALA, sprite);
    ventana.actualizaVentana();
    ventana.espera(16);
}
return 0;
}