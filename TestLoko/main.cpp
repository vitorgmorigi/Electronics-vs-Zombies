#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <stdio.h>
#include "objects.h"				//Our primitive header file

const int WIDTH = 640;
const int HEIGHT = 480;
const int FPS = 60;
const int LINHAS = 6;
const int COLUNAS = 10;
const int NUM_BULLETS = 12;
const int DISTANCIAYZONE = 96;
const int ZONEX = 640/COLUNAS;
const int ZONEY = (480-DISTANCIAYZONE)/LINHAS;

void InitGamer(Gamer &gamer);

void InitZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS);
void DrawZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS);				//Our primitive header file

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, Zone zone[][COLUNAS]);
void UpdateBullet(Bullet bullet[], int size);

int main(void)
{
    enum KEYS {KEY_1, KEY_2, KEY_3, KEY_4};
    bool keys[4] = {false, false, false, false};

    bool done = false;
    bool redraw = true;
    bool desenha = false;
    int draw[4]= {0, 0, 0, 0};
    int pos_x = WIDTH / 2;
    int pos_y = HEIGHT / 2;
    int timer_tiros;

    Gamer(gamer);
    Zone zone[LINHAS][COLUNAS];
    Bullet bullets[NUM_BULLETS+1];

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
            {
                for(int j=0; j<COLUNAS; j++)
                {
                    if(zone[i][j].x < pos_x &&
                            zone[i][j].x + ZONEX > pos_x &&
                            zone[i][j].y < pos_y &&
                            zone[i][j].y + ZONEY > pos_y)
                    {
                        if(zone[i][j].draw == 0)
                        {
                            if(keys[KEY_1])
                                zone[i][j].draw = draw[0];
                            if(keys[KEY_2])
                                zone[i][j].draw = draw[1];
                            if(keys[KEY_3])
                                zone[i][j].draw = draw[2];
                            if(keys[KEY_4])
                                zone[i][j].draw = draw[3];
                        }
                    }
                }
            }

            timer_tiros++;

            for(int i=0; i<LINHAS; i++)
            {
                for(int j=0; j<COLUNAS; j++)
                    if(zone[i][j].draw == 3)
                    {
                        if(timer_tiros >= 200) // faz os Electronics atirarem numa velocidade constante
                        {
                            FireBullet(bullets, NUM_BULLETS+1, zone);
                            timer_tiros = 0;
                        }
                    }
            }

            UpdateBullet(bullets, NUM_BULLETS+1);
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

        if(keys[KEY_1])
            draw[0] = 1;
        if(keys[KEY_2])
            draw[1] = 2;
        if(keys[KEY_3])
            draw[2] = 3;
        if(keys[KEY_4])
            draw[3] = 4;

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            DrawZone(zone, LINHAS, COLUNAS);
            DrawBullet(bullets, NUM_BULLETS+1);
            al_draw_filled_rectangle(pos_x, pos_y, pos_x + 10, pos_y + 10, al_map_rgb(0, 0, 0));
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
    gamer.energy = 0;
    gamer.lives = 3;
    gamer.score = 0;
}

void InitZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS)
{
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            zone[i][j].ID = AREA;
            zone[i][j].x = j*ZONEX;
            zone[i][j].y = i*ZONEY+DISTANCIAYZONE;
            zone[i][j].draw = 0;
        }
    }
}

void DrawZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS)
{
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
            al_draw_rectangle(zone[i][j].x, zone[i][j].y, zone[i][j].x+ZONEX, zone[i][j].y+ZONEY,al_map_rgb(0, 0, 0), 2);
    }

    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(zone[i][j].draw == 1)
            {
                al_draw_filled_rectangle(zone[i][j].x+1, zone[i][j].y+1, zone[i][j].x+ZONEX-1, zone[i][j].y+ZONEY-1,al_map_rgb(255, 0, 0));
            }
            if(zone[i][j].draw == 2)
                al_draw_filled_circle(zone[i][j].x+ZONEX/2, zone[i][j].y+ZONEY/2, ZONEX/4, al_map_rgb(255, 255, 0));
            if(zone[i][j].draw == 3)
                al_draw_filled_rectangle(zone[i][j].x+1, zone[i][j].y+1, zone[i][j].x+ZONEX-1, zone[i][j].y+ZONEY-1,al_map_rgb(0, 0, 255));
            if(zone[i][j].draw == 4)
                al_draw_filled_rectangle(zone[i][j].x+1, zone[i][j].y+1, zone[i][j].x+ZONEX-1, zone[i][j].y+ZONEY-1,al_map_rgb(168, 168, 168));
        }
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
    for(int i = size-1; i > 0; i--)
    {
        if(bullet[i].live)
            al_draw_filled_circle(bullet[i].x, bullet[i].y, 2*i, al_map_rgb(100*i, 100*i, 100*1));
    }
}
void FireBullet(Bullet bullet[], int size, Zone zone[][COLUNAS])
{
    for(int i=0; i<LINHAS; i++)
    {
        for(int j=0; j<COLUNAS; j++)
        {
            if(zone[i][j].draw == 3)
            {
                for( int k = size-1; k > 0; k--)
                {
                    if(!bullet[k].live)
                    {
                        bullet[k].x = zone[i][j].x + (ZONEX/2);
                        bullet[k].y = zone[i][j].y + (ZONEY/2);
                        bullet[k].live = true;
                        break;
                    }
                }
            }
        }
    }
}



void UpdateBullet(Bullet bullet[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bullet[i].live)
        {
            bullet[i].x += bullet[i].speed;
            if(bullet[i].x > WIDTH)
                bullet[i].live = false;
        }
    }
}
