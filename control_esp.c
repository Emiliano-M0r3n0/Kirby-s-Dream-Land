#include "control_esp.h"
#include "graficos.h"
#include "sonidos.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#define PLAY(file) PlaySound(TEXT(file), NULL, SND_FILENAME | SND_ASYNC)
#pragma comment(lib, "winmm.lib")

void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin)
{
    Eje->board = board;
    Eje->pin = pin;
    Eje->offset = 0.0f;
}

void ini_calibracion(EjeJoystick *Eje, int muestras)
{
    float suma = 0.0f;
    ventana.texto((ventana.anchoVentana() / 2) - 150, ventana.altoVentana() / 2, "Calibrando Joystick...");
    ventana.actualizaVentana();

    for (int i = 0; i < muestras; i++)
    {
        suma += Eje->board->analogRead(Eje->board, Eje->pin);
    }

    ventana.limpiaVentana();

    float promedio = suma / (float)muestras;
    Eje->offset = promedio - 0.5f;
}

float leer_joystick(EjeJoystick *eje)
{
    float v = eje->board->analogRead(eje->board, eje->pin);

    v -= 0.5f;
    v -= eje->offset;

    if (fabs(v) < 0.08f)
        v = 0.0f;

    return v;
}

void ini_lens(Lectura *lens, Board *board, uint8_t pin_action, uint8_t pin_jump, uint8_t pin_down, uint8_t pin_mtr)
{
    lens->board = board;

    lens->button_action.pin = pin_action;
    lens->button_action.previo = true;

    lens->button_jump.pin = pin_jump;
    lens->button_jump.previo = true;

    lens->button_down.pin = pin_down;
    lens->button_down.previo = true;

    lens->pin_mtr = pin_mtr;

    lens->mtr_on = false;
}

void leer_entrada(Lectura *lens, EjeJoystick *EjeX, bool terminal)
{
    lens->lensX = leer_joystick(EjeX);
    lens->button_action.actual = lens->board->digitalRead(lens->board, lens->button_action.pin);
    lens->button_jump.actual = lens->board->digitalRead(lens->board, lens->button_jump.pin);
    lens->button_down.actual = lens->board->digitalRead(lens->board, lens->button_down.pin);

    // if (!(lens->button_action.actual)) lens->board->digitalWrite(lens->board,lens->pin_mtr,true);
    // else lens->board->digitalWrite(lens->board,lens->pin_mtr,false);

    if (terminal)
    {
        ventana.imprimeEnConsola("LensX: %.2f | Btn-action: %i | Btn-jump : %i | Btn-down: %i\n",
                                 lens->lensX,
                                 lens->button_action.actual,
                                 lens->button_jump.actual,
                                 lens->button_down.actual);
    }
}

bool fue_presionado(Boton *button, Board *board)
{
    bool presionado = (button->previo == true && button->actual == false);
    button->previo = button->actual;
    return presionado;
}

Imagen *animacion_actual(Animacion *anim)
{
    anim->contador++;
    if (anim->contador >= anim->delay_frames)
    {
        anim->contador = 0;

        if (anim->frame_actual < anim->total_frames - 1)
        {
            anim->frame_actual++;
        }
        else if (anim->bucle)
        {
            anim->frame_actual = 0; // Solo reinicia si es bucle
        }
    }
    return anim->frames[anim->frame_actual];
}

void cargar_animacion(const char **rutas_img, const char **rutas_mask, int total_frames, int delay_frames, Animacion *anim)
{
    anim->frames = malloc(sizeof(Imagen *) * total_frames);

    for (int i = 0; i < total_frames; i++)
    {
        anim->frames[i] = ventana.creaImagenConMascara(rutas_img[i], rutas_mask[i]);
    }

    anim->total_frames = total_frames;
    anim->frame_actual = 0;
    anim->contador = 0;
    anim->delay_frames = delay_frames;
}

void ini_camara(Camara *cam, float pantallaAncho, float pantallaAlto, float fondoAncho, float fondoAlto)
{
    cam->x = 0;
    cam->y = 0;
    cam->width = pantallaAncho;
    cam->height = pantallaAlto;
    cam->fondo_width = fondoAncho;
    cam->fondo_height = fondoAlto;
}

void centrar_cam_kirby(Kirby *kirby)
{
    float targetX = kirby->x - kirby->camara.width / 2 + 32; // Los 32 son para centrar la pantalla en base a los 65 pixeles de kirby
    float targetY = kirby->y - kirby->camara.height / 2 + 32;

    // Limitar para no salir de los bordes del fondo
    if (targetX < 0)
        targetX = 0;
    if (targetX > kirby->camara.fondo_width - kirby->camara.width)
        targetX = kirby->camara.fondo_width - kirby->camara.width;
    if (targetY < 0)
        targetY = 0;
    if (targetY > kirby->camara.fondo_height - kirby->camara.height)
        targetY = kirby->camara.fondo_height - kirby->camara.height;

    kirby->camara.x = targetX;
    kirby->camara.y = targetY;
}

bool hitbox_colision(Hitbox a, Hitbox b)
{
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

void actualizar_hitbox_kirby(Kirby *k)
{

    float MargenIzq = 15.0f;
    float MargenDer = 20.0f;
    float MargenTop = 12.0f;
    float MargenBottom = 5.0f;

    k->kirbyhitbox.x = k->x + MargenIzq;
    k->kirbyhitbox.y = k->y + MargenBottom;
    k->kirbyhitbox.width = 64.0f - MargenIzq - MargenDer;
    k->kirbyhitbox.height = 64.0f - MargenBottom - MargenTop;
}

void actualizar_hitbox_succion(Kirby *k)
{
    float length_inhale = 220.0f; // Alcance horizontal de la succion
    float height_inhale = 25.0f;  // Alcance vertical de la succion
    float posBoca = 25.0f;

    k->succionhitbox.y = k->y + posBoca - (height_inhale / 2);
    k->succionhitbox.height = height_inhale;

    if (k->mirandoDerecha)
    {
        k->succionhitbox.x = k->x + 64.0f;
        k->succionhitbox.width = length_inhale;
    }
    else
    {
        k->succionhitbox.x = k->x - length_inhale;
        k->succionhitbox.width = length_inhale;
    }
}

void actualizar_hitbox_enemie(Enemigo *enemie)
{
    float margenIzq = 5.0f;
    float margenTop = 5.0f;
    float margenDer = 5.0f;
    float margenBottom = 4.0f;

    enemie->hitbox.x = enemie->x + margenIzq;
    enemie->hitbox.width = 50.0f - margenDer - margenIzq;
    enemie->hitbox.y = enemie->y + margenBottom;
    enemie->hitbox.height = 50.0f - margenTop - margenBottom;
}

void crear_proyectil(Proyectil *p, float x, float y, bool mirandoDerecha, bool esEstrella)
{
    p->x = x;
    p->y = y + 25.0f; // Altura de la boca
    p->velX = mirandoDerecha ? 500.0f : -500.0f;
    p->activo = true;
    p->esEstrella = esEstrella;
    p->timerdisparo = 90;

    // Creacion de hitbox
    p->hitbox.x = p->x;
    p->hitbox.y = p->y;
    if (esEstrella)
    {
        p->hitbox.width = 20.0f;
        p->hitbox.height = 20.0f;
    }
    else
    {
        p->hitbox.width = 30.0f;
        p->hitbox.height = 25.0f;
    }
}

void actualizar_proyectil(Proyectil *p, float dt)
{
    if (!p->activo)
        return;

    p->x += p->velX * dt;
    p->hitbox.x = p->x;
    p->hitbox.y = p->y;

    p->timerdisparo--;
    if (p->timerdisparo <= 0)
    {
        p->activo = false;
    }
}

void ini_kirby(Kirby *k, float x, float y)
{
    k->x = x; // Aparece en la posicion inicial
    k->y = y;
    k->velX = 0;
    k->velY = 0;
    k->estado = ST_IDLE; // Inicia en estado quieto
    k->mirandoDerecha = true;
    k->timerAccion = 0.0f;
    k->animActual = &k->arregloAnim[AN_IDLE]; // Se asignará en el primer frame
    k->jump_p = false;
    k->action_p = false;
    k->puntos = 0;

    k->enemies_eaten = 0;
    k->Gordito = false;
    k->stomach_wenemie = false;

    actualizar_hitbox_kirby(k);

    actualizar_hitbox_succion(k);
}

void actualizar_kirby(Kirby *k, Lectura *input, float dt, int anchoVentana, int altoVentana)
{

    actualizar_hitbox_kirby(k);

    if (k->estado == ST_INHALE)
    {
        actualizar_hitbox_succion(k);
    }

    if (input->lensX > 0.1f)
        k->mirandoDerecha = true;
    else if (input->lensX < -0.1f)
        k->mirandoDerecha = false;

    float multiplicador = k->mirandoDerecha ? 700.0f : 400.0f;

    switch (k->estado)
    {

    case ST_IDLE:
        k->velX = 0;
        k->velY += GRAVEDAD * dt;
        if (fabs(input->lensX) > 0.1f)
            k->estado = ST_WALKING;
        if (k->jump_p)
        {
            k->velY = -450.0f;
            k->estado = ST_JUMPING;
        }
        if (k->action_p)
        {
            k->estado = ST_INHALE;
            k->timerAccion = 0;
            k->stomach_wenemie = false;
        }
        break;

    case ST_WALKING:
        k->velX = input->lensX * multiplicador;
        k->velY += GRAVEDAD * dt;
        if (fabs(input->lensX) <= 0.1f)
            k->estado = ST_IDLE;
        if (k->jump_p)
        {
            k->velY = -450.0f;
            k->estado = ST_JUMPING;
        }
        if (k->action_p)
        {
            k->estado = ST_INHALE;
            k->timerAccion = 0;
            k->stomach_wenemie = false;
        }
        break;

    case ST_JUMPING:
        k->velX = input->lensX * multiplicador;
        k->velY += 1200.0f * dt; // Gravedad normal
        if (k->y >= altoVentana - 70)
        {
            k->estado = ST_IDLE;
            k->velY = 0;
        }
        break;

    case ST_INHALE:
        k->velX = 0;
        k->velY += GRAVEDAD * dt;
        k->timerAccion += dt;
        input->mtr_on = true;
        if (k->timerAccion >= 0.5f)
        {
            k->estado = ST_EATING;
            k->timerAccion = 0.0f;
            k->Gordito = true;
        }
        break;

    case ST_EATING:
        k->velX = 0;
        k->velY += GRAVEDAD * dt;
        k->timerAccion += dt;
        if (k->timerAccion >= 0.3f)
        {
            input->mtr_on = false;
            if (fabs(input->lensX) > 0.1f)
            {
                k->estado = ST_FAT_WALKING;
            }
            else
            {
                k->estado = ST_FAT_IDLE;
            }
        }
        break;

    case ST_FAT_IDLE:
        k->velX = 0;
        k->velY += (GRAVEDAD * 0.2f) * dt;
        if (fabs(input->lensX) > 0.1f)
            k->estado = ST_FAT_WALKING;
        if (k->jump_p)
        {
            k->velY = -300.0f;
            k->estado = ST_FAT_FLYING;
        }
        if (k->action_p)
        {
            k->estado = ST_SPITTING;
            k->timerAccion = 0;
        }
        break;

    case ST_FAT_WALKING:
        k->velX = input->lensX * multiplicador;
        k->velY += (GRAVEDAD * 0.2f) * dt;
        if (fabs(input->lensX) <= 0.1f)
            k->estado = ST_FAT_IDLE;
        if (k->jump_p)
        {
            k->velY = -300.0f;
            k->estado = ST_FAT_FLYING;
        }
        if (k->action_p)
        {
            k->estado = ST_SPITTING;
            k->timerAccion = 0;
        }
        break;

    case ST_FAT_FLYING:
        k->velX = input->lensX * multiplicador;
        k->velY += (1200.0f * 0.3f) * dt; // Gravedad reducida (flota)
        if (k->jump_p)
            k->velY = -300.0f;
        if (k->action_p)
        {
            k->estado = ST_SPITTING;
            k->timerAccion = 0;
        }
        if (k->y >= altoVentana - 70)
        {
            k->estado = ST_FAT_IDLE;
            k->velY = 0;
        }
        break;

    case ST_SPITTING:
        k->velX = 0;
        k->velY += GRAVEDAD * dt;
        k->timerAccion += dt;
        input->mtr_on = true;
        if (k->timerAccion < 0.1f)
        {
            crear_proyectil(&k->proyectil, k->x, k->y, k->mirandoDerecha, k->stomach_wenemie);
        }
        if (k->timerAccion >= 0.4f)
        {
            k->estado = ST_IDLE;
            input->mtr_on = false;
            k->Gordito = false;
            if (k->stomach_wenemie)
            {
                k->stomach_wenemie = false;
            }
        }
        break;
    }

    // APLICAR FÍSICA Y LÍMITES
    k->x += k->velX * dt;
    k->y += k->velY * dt;

    if (k->x < 0)
        k->x = 0;
    if (k->x > k->camara.fondo_width - 64)
        k->x = k->camara.fondo_width - 64;
    if (k->y > altoVentana - 50)
        k->y = altoVentana - 50;
    if (k->y < 60)
        k->y = 60;
}

void seleccionar_animacion_kirby(Kirby *k)
{
    int indice = AN_IDLE;
    if (k->estado == ST_IDLE)
        indice = AN_IDLE;
    else if (k->estado == ST_WALKING)
        indice = AN_WALK;
    else if (k->estado == ST_JUMPING)
        indice = AN_JUMP;
    else if (k->estado == ST_INHALE)
        indice = AN_INHALE;
    else if (k->estado == ST_EATING)
        indice = AN_EAT;
    else if (k->estado == ST_SPITTING)
        indice = AN_SPIT;
    else if (k->estado == ST_FAT_IDLE)
        indice = AN_FAT_IDLE;
    else if (k->estado == ST_FAT_WALKING)
        indice = AN_FAT_WALK;
    else if (k->estado == ST_FAT_FLYING)
    {
        indice = (k->velY > 10.0f) ? AN_FAT_FALL : AN_FLY;
    }
    if (k->mirandoDerecha == true)
    {
        if (k->animActual != &k->arregloAnim[indice])
        {
            k->animActual = &k->arregloAnim[indice];
            k->animActual->frame_actual = 0;
            k->animActual->contador = 0;
        }
    }
    else if (k->mirandoDerecha == false)
    {
        if (k->animActual != &k->arregloAnimMirror[indice])
        {
            k->animActual = &k->arregloAnimMirror[indice];
            k->animActual->frame_actual = 0;
            k->animActual->contador = 0;
        }
    }
}

void seleccionar_enemies(HordaEnemigos *horda, int iteracion)
{
    if (iteracion < 0 || iteracion >= MAX_ENEMIGOS) return;
    if (!horda->enemigo[iteracion].activo) return;
    
    int indice = horda->enemigo[iteracion].typeenemie;
    if (indice < 0 || indice >= EN_COUNT) return;
    
    horda->enemigo[iteracion].animActual = &horda->arregloAnimEnemies[indice];
}

void seleccionar_proyectil(Kirby *kirby)
{
    int indice;
    if (kirby->proyectil.esEstrella)
        indice = 1;
    else
    {
        indice = 0;
    }
    if (kirby->mirandoDerecha)
    {
        kirby->proyectil.animActual = &kirby->proyectil.arregloProyectil[indice];
    }
    else if (kirby->mirandoDerecha == false)
    {
        kirby->proyectil.animActual = &kirby->proyectil.arregloProyectilMirror[indice];
    }
}

//-------------------Funciones para la creacion y seleccion de fondos---------------------

void crear_fondos(Fondos *fondos, const char **rutas_fondos)
{
    for (int i = 0; i < FO_COUNT; i++)
    {
        fondos->img[i] = ventana.creaImagen(rutas_fondos[i]);

        if (fondos->img[i])
        {
            fondos->alto_original[i] = ventana.altoImagen(fondos->img[i]);
            fondos->ancho_original[i] = ventana.anchoImagen(fondos->img[i]);
            fondos->cargado[i] = true;
        }
        else
        {
            fondos->cargado[i] = false;
            fondos->alto_original[i] = 1;
            fondos->ancho_original[i] = 1;
        }
    }
}

void crear_escalas_fondos(Fondos *fondos, bool alto)
{
    int alto_ventana = ventana.altoVentana();
    int ancho_ventana = ventana.anchoVentana();

    for (int i = 0; i < FO_COUNT; i++)
    {
        if (!fondos->cargado[i] || fondos->alto_original[i] == 0)
        {
            fondos->escalas[i] = 1.0f;
            continue;
        }
        if (alto)
        {
            fondos->escalas[i] = (float)alto_ventana / (float)fondos->alto_original[i];
        }
        else if (!alto)
        {
            fondos->escalas[i] = (float)ancho_ventana / (float)fondos->ancho_original[i];
        }
        fondos->alto_escalado[i] = fondos->escalas[i] * fondos->alto_original[i];
        fondos->ancho_escalado[i] = fondos->escalas[i] * fondos->ancho_original[i];
    }
}

void dibujar_fondo(Kirby *kirby, int indice, bool fs)
{
    if (indice < 0 || indice >= FO_COUNT)
        return;
    if (!kirby->fondo.cargado[indice] || kirby->fondo.img[indice] == NULL)
        return;

    int x_pantalla = (int)(-kirby->camara.x);
    int y_pantalla = (int)(-kirby->camara.y);

    if (fs)
    {
        ventana.muestraImagenEscalada(x_pantalla, y_pantalla,
                                      (int)kirby->fondo.ancho_escalado[indice],
                                      (int)kirby->fondo.alto_escalado[indice],
                                      kirby->fondo.img[indice]);
    }
    else
    {
        ventana.muestraImagen(x_pantalla, y_pantalla, kirby->fondo.img[indice]);
    }
}

void calc_pos_pantalla(Kirby *k, Enemigo *e)
{
    k->screenX = (int)(k->x - k->camara.x);
    k->screenY = (int)(k->y - k->camara.y);

    if (e->activo)
    {
        e->screenX = (int)(e->x - k->camara.x);
        e->screenY = (int)(e->y - k->camara.y);
    }

    if (k->proyectil.activo)
    {
        k->proyectil.screenX = (int)(k->proyectil.x - k->camara.x);
        k->proyectil.screenY = (int)(k->proyectil.y - k->camara.y);
    }
}

//-------------Funciones para las colisiones de Kirby con el Fondo----------------------

void cargar_colisiones(Mapa *m, const char *ruta)
{
    FILE *file = fopen(ruta, "r");
    if (!file)
        return;
    for (int i = 0; i < MAPA_ALTO; i++)
    {
        for (int j = 0; j < MAPA_ANCHO; j++)
        {
            if (fscanf(file, "%d,", &m->datos[i][j]) == EOF)
                break;
        }
    }
    fclose(file);
}

void aplicar_colisiones(Kirby *k)
{
    // Colisiones verticales
    int col_centro = (int)((k->x + 32) / TILE_SIZE);
    int fila_pies = (int)((k->y + 64) / TILE_SIZE);
    if (col_centro >= 0 && col_centro < MAPA_ANCHO && fila_pies >= 0 && fila_pies < MAPA_ALTO)
    {
        if (k->fondo.mapa.datos[fila_pies][col_centro] != -1)
        {
            k->y = (fila_pies * TILE_SIZE) - 64; // Lo sacamos del suelo
            k->velY = 0;
            k->enSuelo = true;

            // Si estaba cayendo o saltando, vuelve a idle segun el estado anterior de kirby
            if (k->estado == ST_JUMPING)
            {
                k->estado = ST_IDLE;
            }
            else if (k->estado == ST_FAT_FLYING)
            {
                k->estado = ST_FAT_IDLE;
            }
        }
        else
        {
            k->enSuelo = false;
        }
    }

    // Colisiones horizontales
    int direccion_x = (k->velX > 0) ? 40 : 10; // Margen segun hacia donde mira
    int col_pared = (int)((k->x + direccion_x) / TILE_SIZE);
    int fila_cintura = (int)((k->y + 32) / TILE_SIZE);

    if (col_pared >= 0 && col_pared < MAPA_ANCHO && fila_cintura >= 0 && fila_cintura < MAPA_ALTO)
    {
        if (k->fondo.mapa.datos[fila_cintura][col_pared] != -1)
        {
            k->velX = 0;
            if (direccion_x == 40)
                k->x = (col_pared * TILE_SIZE) - 41;
            else
                k->x = (col_pared * TILE_SIZE) + TILE_SIZE + 1;
        }
    }
}

//---------------Funciones para leer records------------------

int leer_record()
{
    FILE *archivo = fopen("highscore.dat", "r");
    int record = 0;

    if (archivo == NULL)
        return 0; // Si no hay archivo la puntuacion maxima es cero

    fscanf(archivo, "%d", &record);
    fclose(archivo);
    return record;
}

void guardar_record(int puntos_actuales)
{
    int record_viejo = leer_record();

    if (puntos_actuales > record_viejo)
    {
        FILE *archivo = fopen("puntajes.p", "w");
        if (archivo != NULL)
        {
            fprintf(archivo, "%d", puntos_actuales);
            fclose(archivo);
        }
    }
}

//---------Funciones para enemigos----------

void inicializar_enemigo_default(Enemigo *e)
{
    e->activo = false;
    e->x = 0.0f;
    e->y = 0.0f;
    e->screenX = 0;
    e->screenY = 0;
    e->velX = 0.0f;
    e->velY = 0.0f;
    e->direccion = 0;
    e->typeenemie = 0;
    e->animActual = NULL;

    // Hitbox
    e->hitbox.x = 0.0f;
    e->hitbox.y = 0.0f;
    e->hitbox.width = 0.0f;
    e->hitbox.height = 0.0f;
}

void inicializar_enemigos(HordaEnemigos *horda)
{
    horda->enemigosactivos = 0;
    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        inicializar_enemigo_default(&horda->enemigo[i]);
    }
}

void generar_enemigo(HordaEnemigos *horda)
{
    int posX[MAX_ENEMIGOS] = {760, 970, 1000, 1200, 1540, 1700, 1940, 2120, 2400, 2700, 3040, 3830, 4600, 5200,5200};
    int posY[MAX_ENEMIGOS] = {450, 450, 450, 450, 370, 370, 370, 450, 450, 140, 450, 290, 450, 450,450};
    int indice = rand() % 14;
    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (!horda->enemigo[i].activo)
        {
            horda->enemigo[i].x = posX[indice];
            horda->enemigo[i].y = posY[indice];
            horda->enemigo[i].typeenemie = rand() % 7;
            horda->enemigo[i].direccion = (rand() % 2) ? 1 : -1;
            horda->enemigo[i].velY = 0;
            horda->enemigo[i].activo = true;
            horda->enemigosactivos++;

            actualizar_hitbox_enemie(&horda->enemigo[i]);
            break;
        }
    }
}

void actualizar_enemigo(Enemigo *e, Mapa *m, float dt)
{
    if (!e->activo)
        return;

    e->velY += 15.0f; // Gravedad constante
    e->y += e->velY * dt;

    // Colision con el suelo
    int col = (int)((e->x + 15) / TILE_SIZE);
    int fila_pies = (int)((e->y + 30) / TILE_SIZE);

    if (m->datos[fila_pies][col] != -1)
    {
        e->y = (fila_pies * TILE_SIZE) - 30;
        e->velY = 0;
    }

    e->x += (e->direccion * 100.0f) * dt;

    // Evitar que se se salga de los margenes
    int col_adelante = (int)((e->x + (e->direccion == 1 ? 45 : -5)) / TILE_SIZE);

    // Si choca con pared o detecta que el siguiente bloque es aire (-1)
    if (m->datos[fila_pies - 1][col_adelante] != -1 || m->datos[fila_pies][col_adelante] == -1)
    {
        e->direccion *= -1; // Da la vuelta
    }
}

void puntos_enemigo(Kirby *k, int tipo_enemigo)
{
    switch (tipo_enemigo)
    {
    case EN_BRONTO:
        k->puntos += PTS_BRONTO;
        break;

    case EN_CAPPY:
        k->puntos += PTS_CAPPY;
        break;
    case EN_GRIZZO:
        k->puntos += PTS_GRIZZO;
        break;
    case EN_POPPY:
        k->puntos += PTS_POPPY;
        break;
    case EN_TWIZZY:
        k->puntos += PTS_TWIZZY;
        break;
    case EN_WADLE:
        k->puntos += PTS_WADLE;
        break;
    default:
        k->puntos += 25;
        break;
    }
}