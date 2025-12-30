#include "control_esp.h"
#include "graficos.h"
#include <math.h>

void ini_joystick(EjeJoystick *Eje, Board *board, uint8_t pin)
{
    Eje->board = board;
    Eje->pin = pin;
    Eje->offset = 0.0f;
}

void ini_calibracion(EjeJoystick *Eje, int muestras)
{
    float suma = 0.0f;

    for (int i = 0; i < muestras; i++) {
        suma += Eje->board->analogRead(Eje->board, Eje->pin);
        ventana.texto(ventana.anchoVentana()/2,ventana.altoVentana()/2,"Calibrando Joystick...");
        ventana.actualizaVentana();
        ventana.limpiaVentana();
        ventana.espera(5);
    }

    float promedio = suma / (float)muestras;
    Eje->offset = promedio - 0.5f;
}

float leer_joystick(EjeJoystick *eje)
{
    float v = 0.0f;
    v = eje->board->analogRead(eje->board, eje->pin) - 0.5f - eje->offset;

    return v;
}

void ini_lens(Lectura *lens,Board *board,uint8_t pin_action, uint8_t pin_jump, uint8_t pin_down,uint8_t pin_mtr)
{
    lens->board = board;
    lens->pin_action = pin_action;
    lens->pin_down = pin_down;
    lens->pin_jump = pin_jump;
    lens->pin_mtr = pin_mtr;

    lens->lens_action = true;
    lens->lens_jump = true;
    lens->lens_down = true;
}

void leer_entrada(Lectura *lens,EjeJoystick *EjeX,bool terminal)
{
    lens->lens_jump = lens->board->digitalRead(lens->board,lens->pin_jump);
    lens->lens_action = lens->board->digitalRead(lens->board,lens->pin_action);
    lens->lens_down = lens->board->digitalRead(lens->board,lens->pin_down);

    lens->lensX = leer_joystick(EjeX);

    if (!(lens->lens_action)) lens->board->digitalWrite(lens->board,lens->pin_mtr,true);
    else lens->board->digitalWrite(lens->board,lens->pin_mtr,false);
    
    if(terminal)
{    ventana.imprimeEnConsola("LensX: %.2f | Btn-action: %i | Btn-jump : %i | Btn-down: %i\n",
        lens->lensX,
        lens->lens_action,
        lens->lens_jump,
        lens->lens_down);
    }
}

Imagen* animacion_actual(Animacion *anim)
{
    anim->contador++;

    if (anim->contador >= anim->delay_frames) {
        anim->contador = 0;
        anim->frame_actual++;
        if (anim->frame_actual >= anim->total_frames) {
            anim->frame_actual = 0;
        }
    }

    return anim->frames[anim->frame_actual];
}

void cargar_animacion(const char **rutas_img, const char **rutas_mask,int total_frames,int delay_frames,Animacion *anim) 
{
    anim->frames = malloc(sizeof(Imagen*) * total_frames);

    for (int i = 0; i < total_frames; i++) {
        anim->frames[i] = ventana.creaImagenConMascara(rutas_img[i],rutas_mask[i]);
    }

    anim->total_frames = total_frames;
    anim->frame_actual = 0;
    anim->contador = 0;
    anim->delay_frames = delay_frames;
}

void ini_kirby(Kirby *k, float x, float y) {
    k->x = x; //Aparece en la posicion inicial
    k->y = y;
    k->velX = 0;
    k->velY = 0;
    k->estado = ST_IDLE; //Inicia en estado quieto
    k->mirandoDerecha = true;
    k->timerAccion = 0.0f;
    k->animActual = NULL; // Se asignará en el primer frame
    k->jump_p = false;
    k->action_p = false;
    k->jump_prev = true;
    k->action_prev = true;
}

void actualizar_kirby(Kirby *k, Lectura *input, float dt, int anchoVentana, int altoVentana) {
    
    k->jump_p = (!input->lens_jump && k->jump_prev);
    k->action_p = (!input->lens_action && k->action_prev);

    k->jump_prev   = input->lens_jump;
    k->action_prev = input->lens_action;
    
    if (input->lensX > 0.1f) k->mirandoDerecha = true;
    else if (input->lensX < -0.1f) k->mirandoDerecha = false;

    // 2. MÁQUINA DE ESTADOS LÓGICA
    switch (k->estado) {
        
        case ST_IDLE:
            k->velX = 0;
            if (fabs(input->lensX) > 0.1f) k->estado = ST_WALKING;
            if (k->jump_p) { k->velY = -450.0f; k->estado = ST_JUMPING; }
            if (k->action_p) { k->estado = ST_EATING; k->timerAccion = 0; }
            break;

        case ST_WALKING:
            k->velX = input->lensX * 400.0f;
            if (fabs(input->lensX) <= 0.1f) k->estado = ST_IDLE;
            if (k->jump_p) { k->velY = -450.0f; k->estado = ST_JUMPING; }
            if (k->action_p) { k->estado = ST_EATING; k->timerAccion = 0; }
            break;

        case ST_JUMPING:
            k->velX = input->lensX * 400.0f;
            k->velY += 1200.0f * dt; // Gravedad normal
            if (k->y >= altoVentana - 50) { k->estado = ST_IDLE; k->velY = 0; }
            break;

        case ST_EATING:
            k->velX = 0;
            k->timerAccion += dt;
            if (k->timerAccion >= 0.5f) k->estado = ST_FAT_IDLE; // TIEMPO_COMER
            break;

        case ST_FAT_IDLE:
            k->velX = 0;
            if (k->jump_p) { k->velY = -300.0f; k->estado = ST_FAT_FLYING; }
            if (k->action_p) { k->estado = ST_SPITTING; k->timerAccion = 0; }
            break;

        case ST_FAT_FLYING:
            k->velX = input->lensX * 400.0f;
            k->velY += (1200.0f * 0.3f) * dt; // Gravedad reducida (flota)
            if (k->jump_p) k->velY = -300.0f; // Salto infinito
            if (k->action_p) { k->estado = ST_SPITTING; k->timerAccion = 0; }
            if (k->y >= altoVentana - 50) { k->estado = ST_FAT_IDLE; k->velY = 0; }
            break;

        case ST_SPITTING:
            k->velX = 0;
            k->timerAccion += dt;
            if (k->timerAccion >= 0.4f) k->estado = ST_IDLE; // TIEMPO_ESCUPIR
            break;
    }

    // 3. APLICAR FÍSICA Y LÍMITES
    k->x += k->velX * dt;
    k->y += k->velY * dt;

    if (k->x < 0) k->x = 0;
    if (k->x > anchoVentana - 64) k->x = anchoVentana - 64;
    if (k->y > altoVentana - 50) k->y = altoVentana - 50;
    if (k->y < 60) k->y = 60;
}

void seleccionar_animacion_kirby(Kirby *k, 
    Animacion *idle, Animacion *walk, Animacion *jump, 
    Animacion *eat, Animacion *fat_idle, Animacion *fly, Animacion *spit) 
{
    switch (k->estado) {
        case ST_IDLE:       k->animActual = idle; break;
        case ST_WALKING:    k->animActual = walk; break;
        case ST_JUMPING:    k->animActual = jump; break;
        case ST_EATING:     k->animActual = eat; break;
        case ST_FAT_IDLE:   k->animActual = fat_idle; break;
        case ST_FAT_FLYING: k->animActual = fly; break;
        case ST_SPITTING:   k->animActual = spit; break;
    }
}