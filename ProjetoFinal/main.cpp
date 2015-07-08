#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
const int NUM_BATTERY = 4;
int NUM_ZOMBIES = 10;
const int DISTANCIAYZONE = 120;
const int ZONEX = WIDTH/COLUNAS;
const int ZONEY = (HEIGHT-DISTANCIAYZONE)/LINHAS;
enum KEYS {KEY_1, KEY_2, KEY_3, KEY_4};
enum STATE {MENU, PLAYING, GAMEOVER};
bool keys[4] = {false, false, false, false};
int timer_tamenho_heat;
int timer_battery_start;
int timer_battery_speed;
int timer_energy_cap_death;
int timer_zombie_start;
int timer_zombie_speed;
int timer_spec_zombie_speed;
int timer_collide_diodo;
int timer_collide_diversos;
int timer_stop_zombie;
int state = MENU;
int dificuldade = 500;
int timer_dificuldade;


void InitGamer(Gamer &gamer);

void InitZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS);
void DrawZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS, ALLEGRO_BITMAP *resistor, ALLEGRO_BITMAP *capacitor, ALLEGRO_BITMAP *indutor, ALLEGRO_BITMAP *diodo);
void CollideZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS, Zombie zombie[], int cSize);
void CollideZoneDiodo(Zone zone[][COLUNAS], int LINHAS, int COLUNAS, Zombie zombie[], int cSize);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *ataque_eletromagnetico);
void FireBullet(Bullet bullet[], int size, Zone zone[][COLUNAS]);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Zombie zombie[], int cSize, Gamer &gamer);

void InitEnergy(Energy energy[], int size);
void DrawEnergy(Energy energy[], int size, ALLEGRO_BITMAP *energia_capacitor);
void CreateEnergy(Energy energy[], int size, Zone zone[][COLUNAS]);

void InitHeat(Heat heat[], int size);
void DrawHeat(Heat heat[], int size, int tamanho, ALLEGRO_BITMAP *fogo);
void FireHeat(Heat heat[], int size, Zone zone[][COLUNAS]);
void CollideHeat(Heat heat[], int bSize, Zombie zombie[], int cSize, Gamer &gamer, int tamanho);

void InitZombie(Zombie zombie[], int size);
void DrawZombie(Zombie zombie[], int size, ALLEGRO_BITMAP *zombie_bitmap);
void StartZombie(Zombie zombie[], int size);
void UpdateZombie(Zombie zombie[], int size);

void InitBattery(Battery battery[], int size);
void DrawBattery(Battery battery[], int size, ALLEGRO_BITMAP *bateria);
void StartBattery(Battery battery[], int size);
void UpdateBattery(Battery battery[], int size);
void CaptureBattery(Battery battery[], int size, float mouse_x, float mouse_y, Gamer &gamer);

int main(void)
{
    bool done = false; // Variavel booleana para identificar se o programa terminou de ser executado
    bool redraw = true; // Enquanto essa variavel for verdadeira, ira ser desenhado algo na tela
    bool desenha = true;
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
    Battery battery[NUM_BATTERY];


    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;
    ALLEGRO_FONT *font_maior = NULL;
    ALLEGRO_BITMAP *resistor = NULL;
    ALLEGRO_BITMAP *capacitor = NULL;
    ALLEGRO_BITMAP *indutor = NULL;
    ALLEGRO_BITMAP *diodo = NULL;
    ALLEGRO_BITMAP *bateria = NULL;
    ALLEGRO_BITMAP *protoboard = NULL;
    ALLEGRO_BITMAP *fogo = NULL;
    ALLEGRO_BITMAP *zombie_bitmap = NULL;
    ALLEGRO_BITMAP *ataque_eletromagnetico = NULL;
    ALLEGRO_BITMAP *energia_capacitor = NULL;
    ALLEGRO_BITMAP *logo = NULL;


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
    al_init_image_addon();

    resistor = al_load_bitmap("Resistor.png");
    capacitor = al_load_bitmap("Capacitor.png");
    indutor = al_load_bitmap("Indutor.png");
    diodo = al_load_bitmap("Diodo.png");
    bateria = al_load_bitmap("Bateria.png");
    protoboard = al_load_bitmap("Protoboard.png");
    fogo = al_load_bitmap("fogo_resistor/fire1.png");
    zombie_bitmap = al_load_bitmap("Zombie.png");
    ataque_eletromagnetico = al_load_bitmap("eletromagnetismo.jpg");
    energia_capacitor = al_load_bitmap("energia_capacitor.png");
    logo = al_load_bitmap("logo_EvsZ.jpg");

    al_convert_mask_to_alpha(resistor, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(capacitor, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(indutor, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(diodo, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(bateria, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(zombie_bitmap, al_map_rgb(255, 255, 255));
    al_convert_mask_to_alpha(ataque_eletromagnetico, al_map_rgb(255, 255, 255));


    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    InitGamer(gamer);
    InitZone(zone, LINHAS, COLUNAS);
    InitBullet(bullets, NUM_BULLETS+1);
    InitHeat(heats, NUM_HEAT+1);
    InitZombie(zombie, NUM_ZOMBIES);
    InitEnergy(energy, NUM_ENERGYS+1);
    InitBattery(battery, NUM_BATTERY);

    font18 = al_load_font("arial.ttf", 18, 0);
    font_maior = al_load_font("arial.ttf", 24, 0);

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_hide_mouse_cursor(display);
    al_start_timer(timer);

    while(!done)
    {
        ALLEGRO_EVENT ev;
/*===============================================================================================================================================*/
        if(state == MENU)
        {
            al_wait_for_event(event_queue, &ev);

            al_draw_bitmap(logo, WIDTH / 2 - 145, HEIGHT - 500, 0);
            al_draw_text(font18, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Os Zombies querem roubar seu diploma, proteja-o");
            al_draw_text(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTRE, "Pressione Space para jogar");
            al_draw_text(font_maior, al_map_rgb(0, 255, 0), WIDTH / 2, HEIGHT / 2 + 150, ALLEGRO_ALIGN_CENTRE, "Tecla 1 = Resistor Tecla 2 = Capacitor  Tecla 3 = Indutor  Tecla 4 = Diodo");
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
                if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
                    state = PLAYING;

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;

            al_flip_display();

        }
/*===============================================================================================================================================*/
        if(state == PLAYING)
        {
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;

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
                timer_battery_speed++;
                timer_battery_start++;
                if(timer_battery_start >= 360) // diminui a frequencia com que nasce uma bateria nova
                {
                    StartBattery(battery, NUM_BATTERY);
                    timer_battery_start = 0;
                }

                if(timer_battery_speed >= 2) // reduz um pouco a velocidade da bateria
                {
                    UpdateBattery(battery, NUM_BATTERY);
                    timer_battery_speed = 0;
                }


                timer_heats++;

                for(int i=0; i<LINHAS; i++)
                    for(int j=0; j<COLUNAS; j++)
                        if(zone[i][j].draw == 1)
                            if(timer_heats >= 200)
                            {
                                FireHeat(heats, NUM_HEAT+1, zone);
                                timer_heats = 0;
                            }

                timer_energy++;
                timer_energy_cap_death++;

                for(int i=0; i<LINHAS; i++)
                    for(int j=0; j<COLUNAS; j++)
                        if(zone[i][j].draw == 2)
                        {
                            if(timer_energy >= 420)
                            {
                                CreateEnergy(energy, NUM_ENERGYS+1, zone);
                                timer_energy = 0;
                            }

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
                timer_zombie_start++;
                timer_zombie_speed++;
                timer_dificuldade++;
                if(timer_zombie_start >= 3)
                {
                    StartZombie(zombie, NUM_ZOMBIES);
                    timer_zombie_start = 0;
                }
                if(dificuldade >20 && dificuldade <=500)
                {
                if(timer_dificuldade >= 300)
                    {
                        dificuldade -= 30;
                        timer_dificuldade = 0;
                    }
                }
                if(timer_zombie_speed >= 3)
                {
                    UpdateZombie(zombie, NUM_ZOMBIES);
                    timer_zombie_speed = 0;
                }


                UpdateBullet(bullets, NUM_BULLETS+1);
                CollideBullet(bullets, NUM_BULLETS, zombie, NUM_ZOMBIES, gamer);
                CollideHeat(heats, NUM_HEAT, zombie, NUM_ZOMBIES, gamer, timer_tamenho_heat);
                CollideZone(zone, LINHAS, COLUNAS, zombie, NUM_ZOMBIES);
                CollideZoneDiodo(zone, LINHAS, COLUNAS, zombie, NUM_ZOMBIES);


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
                if (ev.mouse.button & 2)
                    done = true;
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                pos_x = ev.mouse.x;
                pos_y = ev.mouse.y;
                CaptureBattery(battery, NUM_BATTERY, ev.mouse.x, ev.mouse.y, gamer);
            }

            timer_componente++;

            if(timer_componente >= 10)
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


                DrawZone(zone, LINHAS, COLUNAS, resistor, capacitor, indutor, diodo);
                DrawBullet(bullets, NUM_BULLETS+1, ataque_eletromagnetico);
                DrawEnergy(energy, NUM_ENERGYS+1, energia_capacitor);
                DrawZombie(zombie, NUM_ZOMBIES, zombie_bitmap);
                DrawBattery(battery, NUM_BATTERY, bateria);
                timer_tamenho_heat++;
                DrawHeat(heats, NUM_HEAT+1, timer_tamenho_heat, fogo);
                if(timer_tamenho_heat > 80)
                    for(int i=0; i<NUM_HEAT; i++)
                    {
                        heats[i].live = false;
                        timer_tamenho_heat = 0;
                    }


                al_draw_filled_rectangle(pos_x, pos_y, pos_x + 10, pos_y + 10, al_map_rgb(0, 0, 0));
                count++;
                al_draw_textf(font18, al_map_rgb(255, 0, 0), WIDTH*13/16, 85, 0, "Time: %i", count/60);
                al_draw_textf(font18, al_map_rgb(255, 0, 0), WIDTH*13/300, 85, 0, "Energy: %i", gamer.energy);
                al_draw_textf(font18, al_map_rgb(255, 0, 0), WIDTH*13/25, 85, 0, "Score: %i", gamer.score);
                al_flip_display();
                al_draw_bitmap(protoboard, 0, 0, 0);
            }

        }
    /*================================================================================================================================================*/
        if(state == GAMEOVER)
        {
            al_wait_for_event(event_queue, &ev);
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_text(font18, al_map_rgb(255, 168, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Os Zombies roubaram seu diploma");
            al_draw_text(font18, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 +40, ALLEGRO_ALIGN_CENTRE, "Pressione Space para sair");
            al_draw_textf(font_maior, al_map_rgb(255, 0, 0), WIDTH*13/28, 85, 0, "Score: %i", gamer.score);
            al_flip_display();

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                done = true;

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
                if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                al_destroy_event_queue(event_queue);
                al_destroy_timer(timer);
                al_destroy_font(font18);
                al_destroy_bitmap(resistor);
                al_destroy_bitmap(capacitor);
                al_destroy_bitmap(indutor);
                al_destroy_bitmap(diodo);
                al_destroy_bitmap(bateria);
                al_destroy_bitmap(protoboard);
                al_destroy_bitmap(zombie_bitmap);
                al_destroy_display(display);

            }




        }
    }


    return 0;
}

void InitGamer(Gamer &gamer)
{
    gamer.ID = PLAYER;
    gamer.energy = 50;
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

void DrawZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS, ALLEGRO_BITMAP *resistor, ALLEGRO_BITMAP *capacitor, ALLEGRO_BITMAP *indutor, ALLEGRO_BITMAP *diodo)
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
            al_draw_rectangle(zone[i][j].x, zone[i][j].y, zone[i][j].x+ZONEX, zone[i][j].y+ZONEY,al_map_rgb(0, 0, 0), 2);

    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
        {
            if(zone[i][j].draw == 1)
                al_draw_bitmap(resistor, zone[i][j].x+10, zone[i][j].y+1, 0);
            if(zone[i][j].draw == 2)
                al_draw_bitmap(capacitor, zone[i][j].x+10, zone[i][j].y+10, 0);
            if(zone[i][j].draw == 3)
                al_draw_bitmap(indutor, zone[i][j].x+2, zone[i][j].y+10, 0);
            if(zone[i][j].draw == 4)
                al_draw_bitmap(diodo, zone[i][j].x+1, zone[i][j].y+10, 0);
        }
}
void CollideZone(Zone zone[][COLUNAS], int LINHAS, int COLUNAS, Zombie zombie[], int cSize)
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
            for(int k = 0; k < cSize; k++)
            {
                if(zombie[k].live)
                    if(zone[i][j].x + ZONEX/2 > (zombie[k].x - zombie[k].boundx) &&
                            zone[i][j].x + ZONEX/2 < (zombie[k].x + zombie[k].boundx) &&
                            zone[i][j].y + ZONEY/2 > (zombie[k].y - zombie[k].boundy) &&
                            zone[i][j].y + ZONEY/2 < (zombie[k].y + zombie[k].boundy))

                    {
                        if((zone[i][j].draw >= 1) && (zone[i][j].draw <= 3))
                            zone[i][j].draw = 0;

                    }

            }

}
void CollideZoneDiodo(Zone zone[][COLUNAS], int LINHAS, int COLUNAS, Zombie zombie[], int cSize)
{
    for(int i = 0; i < LINHAS; i++)
        for(int j = 0; j < COLUNAS; j++)
            for(int k = 0; k < cSize; k++)
            {
                if(zombie[k].live)
                    if(zone[i][j].x + ZONEX/2 > (zombie[k].x - zombie[k].boundx) &&
                            zone[i][j].x + ZONEX/2 < (zombie[k].x + zombie[k].boundx) &&
                            zone[i][j].y + ZONEY/2 > (zombie[k].y - zombie[k].boundy) &&
                            zone[i][j].y + ZONEY/2 < (zombie[k].y + zombie[k].boundy))
                        if(zone[i][j].draw == 4)
                        {
                            zombie[k].speed = 0;
                            timer_collide_diodo++;
                            if(timer_collide_diodo >= 600)
                            {
                                zone[i][j].draw = 0;
                                for(int c = 0; c < cSize; c++)
                                    zombie[c].speed = 1;

                                timer_collide_diodo = 0;
                            }

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

void DrawBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *ataque_eletromagnetico)
{
    for(int i = 0; i < size; i++)
        if(bullet[i].live)
            al_draw_bitmap(ataque_eletromagnetico, bullet[i].x, bullet[i].y-20, 0);
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
            {
                if(zombie[j].live)
                    if(bullet[i].x > (zombie[j].x - zombie[j].boundx) &&
                            bullet[i].x < (zombie[j].x + zombie[j].boundx) &&
                            bullet[i].y > (zombie[j].y - zombie[j].boundy) &&
                            bullet[i].y < (zombie[j].y + zombie[j].boundy))
                    {
                        bullet[i].live = false;
                        zombie[j].life -= 25;

                        if(zombie[j].life <= 0)
                            zombie[j].live = false;

                        gamer.score++;
                    }

            }
}

void InitEnergy(Energy energy[], int size)
{
    for(int i = 0; i < size; i++)
    {
        energy[i].ID = ENERGY;
        energy[i].speed = 1;
        energy[i].live = false;
        energy[i].boundx = 20;
        energy[i].boundy = 20;
    }
}


void DrawEnergy(Energy energy[], int size, ALLEGRO_BITMAP *energia_capacitor)
{
    for(int i = 0; i < size; i++)
        if(energy[i].live)
            al_draw_bitmap(energia_capacitor, energy[i].x-10, energy[i].y-30, 0);
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
        heats[i].boundx = 25;
        heats[i].boundy = 80;
    }
}

void DrawHeat(Heat heats[], int size, int tamanho, ALLEGRO_BITMAP *fogo)
{
    for(int i = 0; i < size; i++)
        if(heats[i].live)
            al_draw_bitmap(fogo, heats[i].x-50, heats[i].y-75, 0);
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
        zombie[i].life = 100;
        zombie[i].boundx = 50;
        zombie[i].boundy = 5;
    }
}

void DrawZombie(Zombie zombie[], int size, ALLEGRO_BITMAP *zombie_bitmap)
{
    for(int i = 0; i < size; i++)
        if(zombie[i].live)
            al_draw_bitmap(zombie_bitmap, zombie[i].x, zombie[i].y-40, 0);
}

void StartZombie(Zombie zombie[], int size)
{
    for(int k = 0; k < size; k++)
        if(!zombie[k].live)
            if(rand() % dificuldade == 0)
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
    {
        if(zombie[i].live)
            zombie[i].x -= zombie[i].speed;
        if(zombie[i].x <=0)
            state = GAMEOVER;
    }

}

void InitBattery(Battery battery[], int size)
{
    for(int i = 0; i < size; i++)
    {
        battery[i].ID = ENERGY;
        battery[i].speed = 1;
        battery[i].live = false;
        battery[i].boundx = 120;
        battery[i].boundy = 40;
    }
}
void DrawBattery(Battery battery[], int size, ALLEGRO_BITMAP *bateria)
{
    for(int i = 0; i < size; i++)
    {
        if(battery[i].live)
            al_draw_bitmap(bateria, battery[i].x, battery[i].y, 0);
    }
}
void StartBattery(Battery battery[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(!battery[i].live)
        {
            battery[i].live = true;
            battery[i].x = rand() % (WIDTH);
            battery[i].y = 0;
            break;
        }
    }
}
void UpdateBattery(Battery battery[], int size)
{
    for(int i = 0; i < size; i++)
    {

        if(battery[i].live)
            battery[i].y += battery[i].speed;
        if(battery[i].y > HEIGHT)
        {
            battery[i].y = HEIGHT;
            battery[i].live = false;
        }
    }
}
void CaptureBattery(Battery battery[], int size, float mouse_x, float mouse_y, Gamer &gamer)
{
    for(int i = 0; i < size; i++)
    {
        if(battery[i].live)
        {
            if(mouse_x > (battery[i].x-battery[i].boundy)
                    && mouse_x < (battery[i].x+battery[i].boundx)
                    && mouse_y < (battery[i].y+battery[i].boundy)
                    && mouse_y > (battery[i].y-battery[i].boundy))
            {
                battery[i].live = false;
                gamer.energy += 25;
            }
        }

    }
}


