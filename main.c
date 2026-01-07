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
#include "sonidos.h"
#include "kirby_assets.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Pines para el ESP32
#define JX 35
#define JY 34
#define MTR 19
#define BUTTON_JUMP 26
#define BUTTON_ACTION 25
#define BUTTON_DOWN 27

int main()
{
    // Elementos de puntuacion
    int puntos_partida = 0;
    int record_maximo = leer_record();
    int vidas_kirby = 5;
    float time_invincible = 0;
    // Elementos para imagenes
    const int ESCALA = 65;
    const int DELAY = 50;
    bool fullscreen = false;
    int escalafondo;

    // Configuraciones de la ventana
    ventana.tituloVentana("Kirby's Dream Land");
    ventana.tamanioVentana(800, 600);

    Imagen *sprite; // Imagen que utilizaremos constantemente

    Kirby kirby;

    Enemigo enemies;

    // Cargar animaciones de kirby/enemigos y fondos
    cargar_all_fondos(&kirby.fondo);

    cargar_animaciones_enemies(&enemies);

    cargar_animaciones_kirby(&kirby);

    cargar_animacionesmirror_kirby(&kirby);

    cargar_colisiones(&kirby.fondo.mapa, "Fondos_tiled/ATiled.csv");

    // inicializar variable kirby
    ini_kirby(&kirby, 100, ventana.altoVentana() - 150);

    // inicializar variable enemies
    enemies.x = 400.0f;
    enemies.y = ventana.altoVentana() - 50.0f;
    enemies.activo = true;
    enemies.typeenemie = EN_WADLE;

    actualizar_hitbox_enemie(&enemies);

    seleccionar_enemies(&enemies);
    // Configuraciones y calibraciones del Esp32
    Board *esp32 = connectDevice("COM6", B230400);
    esp32->pinMode(esp32, JX, INPUT);
    esp32->pinMode(esp32, JY, INPUT);
    esp32->pinMode(esp32, MTR, OUTPUT);
    esp32->pinMode(esp32, BUTTON_JUMP, INPUT_PULLUP);
    esp32->pinMode(esp32, BUTTON_ACTION, INPUT_PULLUP);
    esp32->pinMode(esp32, BUTTON_DOWN, INPUT_PULLUP);

    EjeJoystick EjeX;

    Lectura lectura_general;

    ini_joystick(&EjeX, esp32, JX);

    ini_lens(&lectura_general, esp32, BUTTON_ACTION, BUTTON_JUMP, BUTTON_DOWN, MTR);

    ventana.colorFondo(COLORES.NEGRO);
    ventana.limpiaVentana();
    ventana.actualizaVentana();
    ventana.espera(100);

    ini_calibracion(&EjeX, 500);

    Animacion *anim_act;

    ini_camara(&kirby.camara, (float)ventana.anchoVentana(), (float)ventana.altoVentana(), kirby.fondo.ancho_original[FO_A], kirby.fondo.alto_original[FO_A]);

    while (1)
    {
        // Logica
        if (ventana.teclaPresionada() == TECLAS.F9)
        {
            fullscreen = !fullscreen;
            ventana.pantallaCompleta(fullscreen);
            crear_escalas_fondos(&kirby.fondo, true);
        }

        static bool last_mtr = false;

        leer_entrada(&lectura_general, &EjeX, false);

        kirby.action_p = fue_presionado(&lectura_general.button_action, lectura_general.board);
        kirby.jump_p = fue_presionado(&lectura_general.button_jump, lectura_general.board);

        actualizar_kirby(&kirby, &lectura_general, DT, ventana.anchoVentana(), ventana.altoVentana());

        centrar_cam_kirby(&kirby);

        aplicar_colisiones(&kirby);

        if (time_invincible > 0)
        {
            time_invincible -= DT;
        }
        else if (verificar_colision_entidades(&kirby, &enemies))
        {
            vidas_kirby--;
            time_invincible = 2.0f; // 2 segundos de invulnerabilidad

            if (vidas_kirby <= 0)
            {
                guardar_record(puntos_partida);
            }
        }

        if (kirby.estado == ST_INHALE && enemies.activo)
        {
            if (kirby.timerAccion >= 0.2f && kirby.timerAccion < 0.4f)
            {
                if (hitbox_colision(kirby.succionhitbox, enemies.hitbox))
                {
                    enemies.activo = false;
                    kirby.stomach_wenemie = true;
                    kirby.enemies_eaten++;
                }
            }
        }

        if (lectura_general.mtr_on != last_mtr)
        {
            lectura_general.board->digitalWrite(lectura_general.board, lectura_general.pin_mtr, lectura_general.mtr_on);
            last_mtr = lectura_general.mtr_on;
        }

        seleccionar_animacion_kirby(&kirby);
        sprite = animacion_actual(kirby.animActual);

        actualizar_proyectil(&kirby.proyectil, DT);

        if (kirby.proyectil.activo && enemies.activo)
        {
            if (hitbox_colision(kirby.proyectil.hitbox, enemies.hitbox))
            {
                if (kirby.proyectil.esEstrella)
                {
                    enemies.activo = false;
                    switch (enemies.typeenemie)
                    {
                    case EN_WADLE:
                        puntos_partida += PTS_WADLE;
                        break;
                    case EN_BRONTO:
                        puntos_partida += PTS_BRONTO;
                        break;
                    case EN_GRIZZO:
                        puntos_partida += PTS_GRIZZO;
                        break;
                    default:
                        puntos_partida += 50;
                        break;
                    }
                }
                else
                {
                    enemies.x += kirby.proyectil.velX * 0.1f;
                    actualizar_hitbox_enemie(&enemies);
                }
                kirby.proyectil.activo = false;
            }
        }

        // Calcular la nuevas posiciones de los objetos en la pantalla
        calc_pos_pantalla(&kirby, &enemies);

        // Dibujo
        ventana.limpiaVentana();
        dibujar_fondo(&kirby, FO_A, fullscreen);
        char texto_hud[100];
        sprintf(texto_hud, "VIDAS: %d  SCORE: %d  HI-SCORE: %d", vidas_kirby, puntos_partida, record_maximo);
        ventana.texto(10, 10, texto_hud);

        if (vidas_kirby <= 0)
        {
            ventana.texto(300, 300, "GAME OVER - RECORD GUARDADO");
            ventana.actualizaVentana();
            ventana.espera(3000);
            return 0;    
        }
        if (enemies.activo)
        {
            ventana.muestraImagenEscalada(enemies.screenX, enemies.screenY, ESCALA, ESCALA, animacion_actual(enemies.animActual));
        }
        if (kirby.proyectil.activo)
        {
            seleccionar_proyectil(&kirby);
            if (kirby.proyectil.esEstrella)
            {
                ventana.muestraImagenEscalada(kirby.proyectil.screenX, kirby.proyectil.screenY, ESCALA - 35, ESCALA - 35, animacion_actual(kirby.proyectil.animActual));
            }
            else
            {
                ventana.muestraImagenEscalada(kirby.proyectil.screenX, kirby.proyectil.screenY, ESCALA - 25, ESCALA - 25, animacion_actual(kirby.proyectil.animActual));
            }
        }
        ventana.muestraImagenEscalada(kirby.screenX, kirby.screenY, ESCALA, ESCALA, sprite);
        ventana.actualizaVentana();
        ventana.espera(16);
    }
    return 0;
}