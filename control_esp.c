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
    ventana.texto(ventana.anchoVentana()/2,ventana.altoVentana()/2,"Calibrando Joystick...");
    ventana.actualizaVentana();

    for (int i = 0; i < muestras; i++) {
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

void ini_lens(Lectura *lens,Board *board,uint8_t pin_action, uint8_t pin_jump, uint8_t pin_down,uint8_t pin_mtr)
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

void leer_entrada(Lectura *lens,EjeJoystick *EjeX,bool terminal)
{
    lens->lensX = leer_joystick(EjeX);
    lens->button_action.actual = lens->board->digitalRead(lens->board,lens->button_action.pin);
    lens->button_jump.actual = lens->board->digitalRead(lens->board,lens->button_jump.pin);
    lens->button_down.actual = lens->board->digitalRead(lens->board,lens->button_down.pin);

    //if (!(lens->button_action.actual)) lens->board->digitalWrite(lens->board,lens->pin_mtr,true);
    //else lens->board->digitalWrite(lens->board,lens->pin_mtr,false);
    
    if(terminal)
{    ventana.imprimeEnConsola("LensX: %.2f | Btn-action: %i | Btn-jump : %i | Btn-down: %i\n",
        lens->lensX,
        lens->button_action.actual,
        lens->button_jump.actual,
        lens->button_down.actual);
    }
}

bool fue_presionado(Boton *button,Board *board)
{
    bool presionado = (button->previo == true && button->actual == false);
    button->previo = button->actual;
    return presionado;
}

Imagen* animacion_actual(Animacion *anim)
{
anim->contador++;
    if (anim->contador >= anim->delay_frames) {
        anim->contador = 0;
        
        if (anim->frame_actual < anim->total_frames - 1) {
            anim->frame_actual++;
        } else if (anim->bucle) {
            anim->frame_actual = 0; // Solo reinicia si es bucle
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

bool hitbox_colision(Hitbox a, Hitbox b) {
    return (a.x < b.x + b.width &&   
            a.x + a.width > b.x &&   
            a.y < b.y + b.height &&   
            a.y + a.height > b.y);    
}

void actualizar_hitbox_kirby(Kirby *k) {

    float MargenIzq = 15.0f;
    float MargenDer = 20.0f;
    float MargenTop = 12.0f;
    float MargenBottom = 5.0f;

    k->kirbyhitbox.x = k->x + MargenIzq;
    k->kirbyhitbox.y = k->y + MargenBottom;
    k->kirbyhitbox.width = 64.0f - MargenIzq - MargenDer;
    k->kirbyhitbox.height = 64.0f - MargenBottom - MargenTop;

}

void actualizar_hitbox_succion(Kirby *k) {
    float length_inhale = 60.0f; //Alcance horizontal de la succion
    float height_inhale = 25.0f; //Alcance vertical de la succion
    float posBoca = 25.0f;

    k->succionhitbox.y = k->y + posBoca - (height_inhale/2);
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

void ini_kirby(Kirby *k, float x, float y) {
    k->x = x; //Aparece en la posicion inicial
    k->y = y;
    k->velX = 0;
    k->velY = 0;
    k->estado = ST_IDLE; //Inicia en estado quieto
    k->mirandoDerecha = true;
    k->timerAccion = 0.0f;
    k->animActual = &k->arregloAnim[AN_IDLE]; // Se asignará en el primer frame
    k->jump_p = false;
    k->action_p = false;

    k->enemies_eaten = 0;
    k->Gordito = false;
    k->stomach_wenemie = false;

    actualizar_hitbox_kirby(k);

    actualizar_hitbox_succion(k);
}

void actualizar_kirby(Kirby *k, Lectura *input, float dt, int anchoVentana, int altoVentana) {

    actualizar_hitbox_kirby(k);

    if (k->estado == ST_INHALE)
    {
        actualizar_hitbox_succion(k);
    }
    
    if (input->lensX > 0.1f) k->mirandoDerecha = true;
    else if (input->lensX < -0.1f) k->mirandoDerecha = false;

    switch (k->estado) {
        
        case ST_IDLE:
            k->velX = 0;
            k->velY += GRAVEDAD * dt;
            if (fabs(input->lensX) > 0.1f) k->estado = ST_WALKING;
            if (k->jump_p) { k->velY = -450.0f; k->estado = ST_JUMPING; }
            if (k->action_p) { k->estado = ST_INHALE; k->timerAccion = 0; k->stomach_wenemie = false; }
            break;

        case ST_WALKING:
            k->velX = input->lensX * 400.0f;
            k->velY += GRAVEDAD *dt;
            if (fabs(input->lensX) <= 0.1f) k->estado = ST_IDLE;
            if (k->jump_p) { k->velY = -450.0f; k->estado = ST_JUMPING; }
            if (k->action_p) { k->estado = ST_INHALE; k->timerAccion = 0; k->stomach_wenemie = false; }
            break;

        case ST_JUMPING:
            k->velX = input->lensX * 400.0f;
            k->velY += 1200.0f * dt; // Gravedad normal
            if (k->y >= altoVentana - 70) { k->estado = ST_IDLE; k->velY = 0; }
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
                } else{
                    k->estado = ST_FAT_IDLE;
                }
            } 
            break;

        case ST_FAT_IDLE:
            k->velX = 0;
            k->velY += (GRAVEDAD * 0.2f) *dt;
            if (fabs(input->lensX) > 0.1f) k->estado = ST_FAT_WALKING;
            if (k->jump_p) { k->velY = -300.0f; k->estado = ST_FAT_FLYING; }
            if (k->action_p) { k->estado = ST_SPITTING; k->timerAccion = 0; }
            break;

        case ST_FAT_WALKING:
            k->velX = input->lensX * 400.0f;
            k->velY += (GRAVEDAD * 0.2f) *dt;
            if (fabs(input->lensX) <= 0.1f) k->estado = ST_FAT_IDLE;
            if (k->jump_p) { k->velY = -300.0f; k->estado = ST_FAT_FLYING; }
            if (k->action_p) { k->estado = ST_SPITTING; k->timerAccion = 0; }
            break;

        case ST_FAT_FLYING:
            k->velX = input->lensX * 400.0f;
            k->velY += (1200.0f * 0.3f) * dt; // Gravedad reducida (flota)
            if (k->jump_p) k->velY = -300.0f; 
            if (k->action_p) { k->estado = ST_SPITTING; k->timerAccion = 0; }
            if (k->y >= altoVentana - 70) { k->estado = ST_FAT_IDLE; k->velY = 0; }
            break;

        case ST_SPITTING:
            k->velX = 0;
            k->velY += GRAVEDAD *dt;
            k->timerAccion += dt;
            input->mtr_on = true;
            if (k->timerAccion >= 0.4f) {
                k->estado = ST_IDLE; 
                input->mtr_on = false; 
                if (k->stomach_wenemie)
                {
                    k->stomach_wenemie = false;
                } 
            } 
            break;
    }

    //APLICAR FÍSICA Y LÍMITES
    k->x += k->velX * dt;
    k->y += k->velY * dt;

    if (k->x < 0) k->x = 0;
    if (k->x > anchoVentana - 64) k->x = anchoVentana - 64;
    if (k->y > altoVentana - 70) k->y = altoVentana - 70;
    if (k->y < 60) k->y = 60;
}

void seleccionar_animacion_kirby(Kirby *k)
{
    int indice = AN_IDLE;
    if (k->estado == ST_IDLE) indice = AN_IDLE;
    else if (k->estado == ST_WALKING) indice = AN_WALK;
    else if (k->estado == ST_JUMPING) indice = AN_JUMP;
    else if (k->estado == ST_INHALE) indice = AN_INHALE;
    else if (k->estado == ST_EATING) indice = AN_EAT;
    else if (k->estado == ST_SPITTING) indice = AN_SPIT;
    else if (k->estado == ST_FAT_IDLE) indice = AN_FAT_IDLE;
    else if (k->estado == ST_FAT_WALKING) indice = AN_FAT_WALK;
    else if (k->estado == ST_FAT_FLYING) {
        indice = (k->velY > 10.0f) ? AN_FAT_FALL : AN_FLY;
    }
if(k->mirandoDerecha == true)
{
if (k->animActual != &k->arregloAnim[indice]) {
        k->animActual = &k->arregloAnim[indice];
        k->animActual->frame_actual = 0;
        k->animActual->contador = 0;
    }
}
else if(k->mirandoDerecha == false)
{
if (k->animActual != &k->arregloAnimMirror[indice]) {
        k->animActual = &k->arregloAnimMirror[indice];
        k->animActual->frame_actual = 0;
        k->animActual->contador = 0;
    }
}
}

void seleccionar_enemies(Enemigo *enemies)
{
    int indice = enemies->typeenemie;
    enemies->animActual = &enemies->arregloEnemies[indice];
}