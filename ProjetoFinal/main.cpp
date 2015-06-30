#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <stdio.h>
#include "objects.h"				//Our primitive header file

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;
const int LINHAS = 6;
const int COLUNAS = 10;
const int NUM_BULLETS = 60;
const int NUM_ENERGYS = 60;
const int NUM_HEAT = 60;
const int NUM_ZOMBIES = 10;
const int DISTANCIAYZONE = 120;
const int ZONEX = WIDTH/COLUNAS;
const int ZONEY = (HEIGHT-DISTANCIAYZONE)/LINHAS;
enum KEYS {KEY_1, KEY_2, KEY_3, KEY_4};
bool keys[4] = {false, false, false, false};
int timer_tamenho_heat;

void InitGamer(Gamer &gamer);

void InitZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS);
void DrawZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, Zone zone[][COLUNAS]);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Zombie zombie[], int cSize, Gamer &gamer);

void InitEnergy(Energy energy[], int size);
void DrawEnergy(Energy energy[], int size);
void CreateEnergy(Energy energy[], int size, Zone zone[][COLUNAS]);

void InitHeat(Heat heat[], int size);
void DrawHeat(Heat heat[], int size, int tamanho);
void FireHeat(Heat heat[], int size, Zone zone[][COLUNAS]);
void CollideHeat(Heat heat[], int bSize, Zombie zombie[], int cSize, Gamer &gamer, int tamanho);

void InitZombie(Zombie zombie[], int size);
void DrawZombie(Zombie zombie[], int size);
void StartZombie(Zombie zombie[], int size);
void UpdateZombie(Zombie zombie[], int size);

int main(void)
{
    bool done = false;
    bool redraw = true;
    bool desenha = false;
    int draw[5]= {0, 0, 0, 0, 0};
    int pos_x = WIDTH / 2;
    int pos_y = HEIGHT / 2;
    int timer_tiros = 0;
    int timer_energy = 0;
    int timer_heats = 0;
    int timer_tamenho_heat = 0;
    int timer_componente = 0;
    int count = 0;

    Gamer(gamer);
    Zone zone[LINHAS][COLUNAS];
    Bullet bullets[NUM_BULLETS+1];
    Energy energy[NUM_ENERGYS+1];
    Heat heats[NUM_HEAT+1];
    Zombie zombie[NUM_ZOMBIES];

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;

    if(!al_init())										//initialize Allegro
        return -1;

    display = al_create_display(WIDTH, HEIGHT);			//create our display object

    if(!display)										//test display object
        return -1;

    al_init_primitives_addon();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_mouse();

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    InitGamer(gamer);
    InitZone(zone, LINHAS, COLUNAS);
    InitBullet(bullets, NUM_BULLETS+1);
    InitHeat(heats, NUM_HEAT+1);
    InitZombie(zombie, NUM_ZOMBIES);
    InitEnergy(energy, NUM_ENERGYS+1);

    font18 = al_load_font("arial.ttf", 18, 0);

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_hide_mouse_cursor(display);
    al_start_timer(timer);

    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;

            for(int i = 0; i < LINHAS; i++)
                for(int j=0; j<COLUNAS; j++)
                    if(zone[i][j].x < pos_x &&
                            zone[i][j].x + ZONEX > pos_x &&
                            zone[i][j].y < pos_y &&
                            zone[i][j].y + ZONEY > pos_y)
                        if(desenha)
                            if(zone[i][j].draw == 0)
                            {
                                if(keys[KEY_1] && gamer.energy >= 100)
                                {
                                    zone[i][j].draw = draw[1];
                                    gamer.energy -=100;
                                    draw[1] = 0;
                                }
                                if(keys[KEY_2] && gamer.energy >= 50)
                                {
                                    zone[i][j].draw = draw[2];
                                    gamer.energy -=50;
                                    draw[2] = 0;
                                }
                                if(keys[KEY_3] && gamer.energy >= 100)
                                {
                                    zone[i][j].draw = draw[3];
                                    gamer.energy -=100;
                                    draw[3] = 0;
                                }
                                if(keys[KEY_4] && gamer.energy >= 100)
                                {
                                    zone[i][j].draw = draw[4];
                                    gamer.energy -=100;
                                    draw[4] = 0;
                                }
                            }

            timer_heats++;

            for(int i=0; i<LINHAS; i++)
                for(int j=0; j<COLUNAS; j++)
                    if(zone[i][j].draw == 1)
                        if(timer_heats >= 200) // faz os Electronics atirarem numa velocidade constante
                        {
                            FireHeat(heats, NUM_HEAT+1, zone);
                            timer_heats = 0;
                        }

            timer_energy++;

            for(int i=0; i<LINHAS; i++)
                for(int j=0; j<COLUNAS; j++)
                    if(zone[i][j].draw == 2)
                        if(timer_energy >= 200) // faz os Electronics atirarem numa velocidade constante
                        {
                            CreateEnergy(energy, NUM_ENERGYS+1, zone);
                            timer_energy = 0;
                        }

            timer_tiros++;

            for(int i=0; i<LINHAS; i++)
                for(int j=0; j<COLUNAS; j++)
                    if(zone[i][j].draw == 3)
                        if(timer_tiros >= 200) // faz os Electronics atirarem numa velocidade constante
                        {
                            FireBullet(bullets, NUM_BULLETS+1, zone);
                            timer_tiros = 0;
                        }

            StartZombie(zombie, NUM_ZOMBIES);
            UpdateBullet(bullets, NUM_BULLETS+1);
            UpdateZombie(zombie, NUM_ZOMBIES);
            CollideBullet(bullets, NUM_BULLETS, zombie, NUM_ZOMBIES, gamer);
            CollideHeat(heats, NUM_HEAT, zombie, NUM_ZOMBIES, gamer, timer_tamenho_heat);

            for(int i = 0; i < NUM_ENERGYS; i++)
                if(energy[i].live)
                    if(energy[i].x - energy[i].boundx < pos_x &&
                            energy[i].x + energy[i].boundx > pos_x &&
                            energy[i].y - energy[i].boundy < pos_y &&
                            energy[i].y + energy[i].boundy > pos_y)
                    {
                        energy[i].live = false;
                        gamer.energy += 25;
                    }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_1:
                keys[KEY_1] = true;
                break;
            case ALLEGRO_KEY_2:
                keys[KEY_2] = true;
                break;
            case ALLEGRO_KEY_3:
                keys[KEY_3] = true;
                break;
            case ALLEGRO_KEY_4:
                keys[KEY_4] = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_1:
                keys[KEY_1] = false;
                break;
            case ALLEGRO_KEY_2:
                keys[KEY_2] = false;
                break;
            case ALLEGRO_KEY_3:
                keys[KEY_3] = false;
                break;
            case ALLEGRO_KEY_4:
                keys[KEY_4] = false;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if(ev.mouse.button & 1)
                desenha = !desenha;
            else if (ev.mouse.button & 2)
                done = true;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            pos_x = ev.mouse.x;
            pos_y = ev.mouse.y;
        }

        timer_componente++;

        if(timer_componente >= 120) // faz os Electronics atirarem numa velocidade constante
        {
            if(keys[KEY_1])
                draw[1] = 1;
            if(keys[KEY_2])
                draw[2] = 2;
            if(keys[KEY_3])
                draw[3] = 3;
            if(keys[KEY_4])
                draw[4] = 4;
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            DrawZone(zone, LINHAS, COLUNAS);
            DrawBullet(bullets, NUM_BULLETS+1);
            DrawEnergy(energy, NUM_ENERGYS+1);
            DrawZombie(zombie, NUM_ZOMBIES);
            timer_tamenho_heat++;
            DrawHeat(heats, NUM_HEAT+1, timer_tamenho_heat);
            int vivo = 0;
            for(int i=0; i<NUM_HEAT; i++){
                    if(heats[i].live)
                        vivo++;
            }
            if(timer_tamenho_heat > 80)
                for(int i=0; i<NUM_HEAT; i++)
            {
                heats[i].live = false;
                timer_tamenho_heat = 0;
            }
            al_draw_filled_rectangle(pos_x, pos_y, pos_x + 10, pos_y + 10, al_map_rgb(0, 0, 0));
            count++;
            al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH*13/16, 30, 0, "Time: %i", count/60);
            al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH*13/16, 10, 0, "Energy: %i", gamer.energy);
            al_draw_textf(font18, al_map_rgb(0, 0, 0), WIDTH*1/16, 10, 0, "vivos: %i", vivo);
            al_flip_display();
            al_clear_to_color(al_map_rgb(255,255,255));
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(font18);
    al_destroy_display(display);						//destroy our display object

    return 0;
}

void InitGamer(Gamer &gamer)
{
    gamer.ID = PLAYER;
    gamer.energy = 60000;
    gamer.lives = 2;
    gamer.score = 0;
}

void InitZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS)
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
        {
            zone[i][j].ID = AREA;
            zone[i][j].x = j*ZONEX;
            zone[i][j].y = i*ZONEY+DISTANCIAYZONE;
            zone[i][j].draw = 0;
        }
}

void DrawZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS)
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
            al_draw_rectangle(zone[i][j].x, zone[i][j].y, zone[i][j].x+ZONEX, zone[i][j].y+ZONEY,al_map_rgb(0, 0, 0), 2);

    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
        {
            if(zone[i][j].draw == 1)
                al_draw_filled_rectangle(zone[i][j].x+1, zone[i][j].y+1, zone[i][j].x+ZONEX-1, zone[i][j].y+ZONEY-1,al_map_rgb(255, 0, 0));
            if(zone[i][j].draw == 2)
                al_draw_filled_circle(zone[i][j].x+ZONEX/2, zone[i][j].y+ZONEY/2, ZONEX/4, al_map_rgb(255, 255, 0));
            if(zone[i][j].draw == 3)
                al_draw_filled_rectangle(zone[i][j].x+1, zone[i][j].y+1, zone[i][j].x+ZONEX-1, zone[i][j].y+ZONEY-1,al_map_rgb(0, 0, 255));
            if(zone[i][j].draw == 4)
                al_draw_filled_rectangle(zone[i][j].x+1, zone[i][j].y+1, zone[i][j].x+ZONEX-1, zone[i][j].y+ZONEY-1,al_map_rgb(168, 168, 168));
        }
}

void InitBullet(Bullet bullet[], int size)
{
    for(int i = 0; i < size; i++)
    {
        bullet[i].ID = BULLET;
        bullet[i].speed = 4;
        bullet[i].live = false;
    }
}

void DrawBullet(Bullet bullet[], int size)
{
    for(int i = 0; i < size; i++)
        if(bullet[i].live)
            al_draw_filled_circle(bullet[i].x, bullet[i].y, 5, al_map_rgb(255, 255, 0));
}

void FireBullet(Bullet bullet[], int size, Zone zone[][COLUNAS])
{
    for(int i=0; i<LINHAS; i++)
        for(int j=0; j<COLUNAS; j++)
            if(zone[i][j].draw == 3)
                for( int k = 0; k < size; k++)
                    if(!bullet[k].live)
                    {
                        bullet[k].x = zone[i][j].x + (ZONEX/2);
                        bullet[k].y = zone[i][j].y + (ZONEY/2);
                        bullet[k].live = true;
                        break;
                    }
}

void UpdateBullet(Bullet bullet[], int size)
{
    for(int i = 0; i < size; i++)
        if(bullet[i].live)
        {
            bullet[i].x += bullet[i].speed;
            if(bullet[i].x > WIDTH)
                bullet[i].live = false;
        }
}

void CollideBullet(Bullet bullet[], int bSize, Zombie zombie[], int cSize, Gamer &gamer)
{
    for(int i = 0; i < bSize; i++)
        if(bullet[i].live)
            for(int j =0; j < cSize; j++)
                if(zombie[j].live)
                    if(bullet[i].x > (zombie[j].x - zombie[j].boundx) &&
                            bullet[i].x < (zombie[j].x + zombie[j].boundx) &&
                            bullet[i].y > (zombie[j].y - zombie[j].boundy) &&
                            bullet[i].y < (zombie[j].y + zombie[j].boundy))
                    {
                        bullet[i].live = false;
                        zombie[j].live = false;

                        gamer.score++;
                    }
}

void InitEnergy(Energy energy[], int size)
{
    for(int i = 0; i < size; i++)
    {
        energy[i].ID = ENERGY;
        energy[i].speed = 1;
        energy[i].live = false;
        energy[i].boundx = 5;
        energy[i].boundy = 5;
    }
}


void DrawEnergy(Energy energy[], int size)
{
    for(int i = 0; i < size; i++)
        if(energy[i].live)
            al_draw_filled_circle(energy[i].x, energy[i].y, 5, al_map_rgb(0, 255, 0));
}

void CreateEnergy(Energy energy[], int size, Zone zone[][COLUNAS])
{
    for(int i=0; i<LINHAS; i++)
        for(int j=0; j<COLUNAS; j++)
            if(zone[i][j].draw == 2)
                for( int k = 0; k < size; k++)
                    if(!energy[k].live)
                    {
                        energy[k].x = zone[i][j].x + (ZONEX/2);
                        energy[k].y = zone[i][j].y + ZONEY/2;
                        energy[k].live = true;
                        break;
                    }
}

void InitHeat(Heat heats[], int size)
{
    for(int i = 0; i < size; i++)
    {
        heats[i].ID = HEAT;
        heats[i].live = false;
        heats[i].boundx = 80;
        heats[i].boundy = 80;
    }
}

void DrawHeat(Heat heats[], int size, int tamanho)
{
    for(int i = 0; i < size; i++)
        if(heats[i].live)
            al_draw_filled_circle(heats[i].x, heats[i].y, tamanho, al_map_rgb(255, 255, 0));
}

void FireHeat(Heat heats[], int size, Zone zone[][COLUNAS])
{
    for(int i=0; i<LINHAS; i++)
        for(int j=0; j<COLUNAS; j++)
            if(zone[i][j].draw == 1)
                for( int k = 0; k < size; k++)
                    if(!heats[k].live)
                    {
                        heats[k].x = zone[i][j].x + (ZONEX/2);
                        heats[k].y = zone[i][j].y + (ZONEY/2);
                        heats[k].live = true;
                        break;
                    }
}

void CollideHeat(Heat heats[], int bSize, Zombie zombie[], int cSize, Gamer &gamer, int tamanho)
{
    for(int i = 0; i < bSize; i++)
        if(heats[i].live)
            for(int j =0; j < cSize; j++)
                if(zombie[j].live)
                    if(heats[i].x + heats[i].boundx*tamanho/80 > (zombie[j].x - zombie[j].boundx) &&
                            heats[i].x - heats[i].boundx*tamanho/80 < (zombie[j].x + zombie[j].boundx) &&
                            heats[i].y + heats[i].boundy*tamanho/80 > (zombie[j].y - zombie[j].boundy) &&
                            heats[i].y - heats[i].boundy*tamanho/80 < (zombie[j].y + zombie[j].boundy))
                    {
                        zombie[j].live = false;
                        gamer.score++;
                    }
}

void InitZombie(Zombie zombie[], int size)
{
    for(int i = 0; i < size; i++)
    {
        zombie[i].ID = ENERGY;
        zombie[i].speed = 1;
        zombie[i].live = false;
        zombie[i].boundx = 5;
        zombie[i].boundy = 5;
    }
}

void DrawZombie(Zombie zombie[], int size)
{
    for(int i = 0; i < size; i++)
        if(zombie[i].live)
            al_draw_filled_circle(zombie[i].x, zombie[i].y, 20, al_map_rgb(255, 0, 0));
}

void StartZombie(Zombie zombie[], int size)
{
    for(int k = 0; k < size; k++)
        if(!zombie[k].live)
            if(rand() % 500 == 0)
            {
                zombie[k].live = true;
                zombie[k].x = WIDTH;
                zombie[k].y = DISTANCIAYZONE +ZONEY/2 + (rand() % LINHAS)*ZONEY;
                break;
            }
}

void UpdateZombie(Zombie zombie[], int size)
{
    for(int i = 0; i < size; i++)
        if(zombie[i].live)
            zombie[i].x -= zombie[i].speed;
}
