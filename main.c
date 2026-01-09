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
#include <time.h>
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
    srand(time(NULL));
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

    HordaEnemigos horda_1;

    // Cargar animaciones de kirby/enemigos y fondos
    cargar_all_fondos(&kirby.fondo);

    cargar_animaciones_enemies(&horda_1);

    inicializar_enemigos(&horda_1);

    generar_enemigo(&horda_1);

    cargar_animaciones_kirby(&kirby);

    cargar_animacionesmirror_kirby(&kirby);

    cargar_colisiones(&kirby.fondo.mapa, "Fondos_tiled/ATiled.csv");

    // inicializar variable kirby
    ini_kirby(&kirby, 100, ventana.altoVentana() - 150);

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

        if (vidas_kirby <= 0)
        {
            guardar_record(kirby.puntos);
        }
        
        if (time_invincible > 0)
        {
            time_invincible -= DT;
            if (time_invincible <= 0)
                lectura_general.mtr_on = false;
        }

        for (int i = 0; i < MAX_ENEMIGOS; i++)
        {
            if (!horda_1.enemigo[i].activo)
                continue;

            // Solo procesar enemigos que están cerca de la pantalla (Culling para evitar LAG)
            if (horda_1.enemigo[i].x > kirby.x - 600 && horda_1.enemigo[i].x < kirby.x + 600)
            {

                actualizar_enemigo(&horda_1.enemigo[i], &kirby.fondo.mapa, DT);
                seleccionar_enemies(&horda_1,i);
                actualizar_hitbox_enemie(&horda_1.enemigo[i]);
                calc_pos_pantalla(&kirby, &horda_1.enemigo[i]);

                // SUCCIÓN
                if (kirby.estado == ST_INHALE)
                {
                    actualizar_hitbox_succion(&kirby);
                    if (hitbox_colision(kirby.succionhitbox, horda_1.enemigo[i].hitbox))
                    {
                        // Atracción
                        horda_1.enemigo[i].x += (kirby.x - horda_1.enemigo[i].x) * DT * 5.0f;

                        if (hitbox_colision(kirby.kirbyhitbox, horda_1.enemigo[i].hitbox)) //El enemigo esta cerca de kirby
                        {
                            horda_1.enemigo[i].activo = false;
                            kirby.estado = ST_FAT_IDLE; // Kirby ahora está gordo
                        }
                    }
                }

                // Colision con el proyectil, sea humo o estrella
                if (kirby.proyectil.activo && hitbox_colision(kirby.proyectil.hitbox, horda_1.enemigo[i].hitbox))
                {
                    if (kirby.proyectil.esEstrella)
                    {
                        horda_1.enemigo[i].activo = false; // La estrella mata
                        kirby.proyectil.activo = false;
                        puntos_enemigo(&kirby,horda_1.enemigo[i].typeenemie);
                    }
                    else
                    {
                        // El humo solo empuja
                        horda_1.enemigo[i].x += (kirby.proyectil.velX > 0) ? 50 : -50;
                        kirby.proyectil.activo = false;
                    }
                }

                // DAÑO A KIRBY
                if (time_invincible <= 0 && hitbox_colision(kirby.kirbyhitbox, horda_1.enemigo[i].hitbox))
                {
                    vidas_kirby--;
                    time_invincible = 1.5f;
                    lectura_general.mtr_on = true; // Vibra el control
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
        for (int i = 0; i < MAX_ENEMIGOS; i++)
        {
            if (!horda_1.enemigo[i].activo)
                continue;
            else
            {
                ventana.muestraImagenEscalada(horda_1.enemigo[i].screenX, horda_1.enemigo[i].screenY, ESCALA, ESCALA, animacion_actual(horda_1.enemigo[i].animActual));
            }
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