#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "objects.h"

// VARIAVEIS GLOBAIS
const int WIDTH = 800; //Resolução X
const int HEIGHT = 480; //Resolução Y
enum KEYS { UP, DOWN, LEFT, RIGHT, ONE}; //Introduz as teclas primitivas do teclado
const int FPS = 60;
const int NUM_ZOMBIES = 7;
const int NUM_TIROS = 7;
const int NUM_ENERGIA = 4;
const int NUM_ELECTRONICS = 45;
const int LINHA_MAX = 5;
const int COL_MAX = 9;
const int WIDTH_PIXEL = WIDTH / LINHA_MAX; // Resoluçao X do pixel
const int HEIGHT_PIXEL = HEIGHT / COL_MAX; // Resoluçao Y do pixel
int timer_tiros;
int timer_zombie;
int energia_armazenada = 0;
int tiros_tela = 0;
int timer_energia;
int V_ELT = 0;

int mapa[5][9] = // nao esta sendo utilizado (TALVEZ ira ser utilizado no futuro)
{
    {0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,1},
};

// PROTOTIPOS
void InitZombie (Zombies *zombie, int tamanho);
void DrawZombie (Zombies *zombie, int tamanho);
void StartZombie(Zombies *zombie, int tamanho);
void UpdateZombie(Zombies *zombie, int tamanho);

void InitElectronic(Electronics *resistor, int numero_electronics);
void DrawElectronic(Electronics *resistor, int numero_electronics);
void StartElectronic(Electronics *resistor, float mouse_x, float mouse_y);

void InitBullet(Tiros *tiro, int tamanho);
void DrawBullet(Tiros *tiro, int tamanho);
void FireBullet(Tiros *tiro, Zombies *zombie, int numero_tiros, int numero_zombies);
void UpdateBullet(Tiros *tiro, int tamanho);

void ColisaoBulletZombie(Zombies *zombie, Tiros *tiro, int numero_zombies, int numero_tiros);

void InitEnergia (Energia *energia, int tamanho);
void DrawEnergia (Energia *energia, int tamanho);
void StartEnergia(Energia *energia, int tamanho);
void UpdateEnergia(Energia *energia, int tamanho);
void PegaEnergia(Energia *energia, float mouse_x, float mouse_y, int numero_energia);

int TemZombie(Zombies *zombie, int tamanho);
int DaParaComprarResistor();


int main(void)
{
    // VARIAVEIS PRIMITIVAS
    bool done = false; //Variavel booleana para identificar se o programa terminou de ser executado
    bool redraw = true; //Enquanto essa variavel for verdadeira, ira ser desenhado algo na tela



    // STRUCTS DOS OBJETOS
    Zombies zombie[NUM_ZOMBIES];
    Electronics resistor[NUM_ELECTRONICS];
    Tiros tiro[NUM_TIROS];
    Energia energia[NUM_ENERGIA];


    bool keys[4] = {false,false,false,false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;

    if(!al_init()) // Inicializa o Allegro
    {
        printf("Erro ao iniciar o Allegro");
        return -1;
    }

    display = al_create_display(WIDTH, HEIGHT);// Cria o display

    if(!display)// Testa o display
    {
        printf("Erro ao iniciar o display");
        return -1;
    }
    al_init_primitives_addon(); // Introduz os comandos primitivos(figuras geometricas) do Allegro
    al_install_mouse(); // Introduz o comando do mouse
    al_install_keyboard(); // Introduz o comando de teclas
    al_init_font_addon();
    al_init_ttf_addon();

    event_queue = al_create_event_queue();
    timer = al_create_timer(0.20 / FPS); // Define o tempo de atualizaçao do timer

    srand(time(NULL));
    InitZombie(zombie, NUM_ZOMBIES);
    InitElectronic(resistor, NUM_ELECTRONICS);
    InitEnergia(energia, NUM_ENERGIA);
    InitBullet(tiro, NUM_TIROS);

    font18 = al_load_font("arial.ttf", 18, 0);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer); // Inicia o timer

    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw=true;

            StartZombie(zombie, NUM_ZOMBIES);
            StartEnergia(energia, NUM_ENERGIA);
            timer_tiros++; // subtimer tiros
            if(timer_tiros >= 200) // faz os Electronics atirarem numa velocidade constante
            {
                FireBullet(tiro, zombie, NUM_TIROS, NUM_ZOMBIES);
                timer_tiros = 0;
            }
            UpdateBullet(tiro, NUM_ZOMBIES);
            ColisaoBulletZombie(zombie, tiro, NUM_ZOMBIES, NUM_TIROS);

            timer_zombie++; // subtimer zombies
            timer_energia++; // subtimer energias
            if(timer_zombie >= 15) // faz os Zombies se movimentarem mais lentamente, pois a velocidade minima (1) ainda eh rapida
            {
                UpdateZombie(zombie, NUM_ZOMBIES);
                timer_zombie = 0;
            }


            if(timer_energia >= 10) // faz as energias cairem mais lentamente
            {
                UpdateEnergia(energia, NUM_ENERGIA);
                timer_energia = 0;
            }

        }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            PegaEnergia(energia, ev.mouse.x, ev.mouse.y, NUM_ENERGIA);
        }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if(ev.mouse.button & 1)
            {
                StartElectronic(resistor, ev.mouse.x, ev.mouse.y);
            }

        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {

            case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
                break;
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_1:
                keys[ONE] = true;
                break;
            case ALLEGRO_KEY_PAD_1:
                keys[ONE] = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {

            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
            case ALLEGRO_KEY_1:
                keys[ONE] = false;
                break;
            case ALLEGRO_KEY_PAD_1:
                keys[ONE] = false;
                break;
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;

            }
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }

        if(redraw && al_is_event_queue_empty(event_queue)) // Se nenhum evento de teclado ou mouse ocorrer acontecerá isso
        {
            redraw=false;
            DrawZombie(zombie, NUM_ZOMBIES);
            DrawEnergia(energia, NUM_ENERGIA);
            DrawElectronic(resistor, NUM_ELECTRONICS);
            DrawBullet(tiro, NUM_ZOMBIES);
            al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "Energia: %i", energia_armazenada);
            al_draw_textf(font18, al_map_rgb(255, 0, 255), 500, 5, 0, "Posicao x: %d", ev.mouse.x);
            al_draw_textf(font18, al_map_rgb(255, 0, 255), 300, 5, 0, "Tiros disparados: %i", tiros_tela);
            for (int i=0; i<NUM_ZOMBIES; i++)
            {
                al_draw_textf(font18, al_map_rgb(255, 0, 255), 5+i*20, 450 , 0, "%i", zombie[i].live);
            }
            for (int i=0; i<NUM_TIROS; i++)
            {
                al_draw_textf(font18, al_map_rgb(255, 255, 255), 150+i*20, 450 , 0, "%i", tiro[i].live);
            }


            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_display(display); //Fecha o display

    return 0;
}

void InitZombie (Zombies *zombie, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        zombie[i].ID = ZOMBIES;
        zombie[i].live = false;
        zombie[i].speed = 1;
        zombie[i].boundx = 10;
        zombie[i].boundy = 10;
        zombie[i].life = 100;
    }
}


void DrawZombie (Zombies *zombie, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(zombie[i].live)
        {
            al_draw_filled_circle(zombie[i].x, zombie[i].y, 20, al_map_rgb(255, 0, 0));

        }
    }
}
void StartZombie(Zombies *zombie, int tamanho)
{


    for(int i = 0; i < tamanho; i++)
    {
        if(!zombie[i].live)
        {

            if(rand() % 1000 == 0)
            {
                zombie[i].live = true;
                printf("\nzombie %d\n", i);
                zombie[i].x = WIDTH;
                zombie[i].y = 45 + 96 * rand() % (HEIGHT);
            }
        }


    }

}

void UpdateZombie(Zombies *zombie, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(zombie[i].live)
        {
            zombie[i].x -= zombie[i].speed;

            if(zombie[i].x < 0)
                zombie[i].live = false;
        }
    }
}

int TemZombie(Zombies *zombie, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(zombie[i].live)
            return 1;
    }
    return 0;

}

void InitBullet(Tiros *tiro, int tamanho)
{
    for(int k = 0; k < tamanho; k++)
    {
        tiro[k].ID = BULLETS;
        tiro[k].live = false;
        tiro[k].PODER = CALOR;
        tiro[k].forca_tiro = 10;
        tiro[k].speed = 1;

    }
}

void DrawBullet(Tiros *tiro, int tamanho)
{
    int k = 0;
    if(tiro[k].live)
    {
        for(k; k < tamanho; k++)
        {
            al_draw_filled_circle(tiro[k].x, tiro[k].y, 5, al_map_rgb(255, 255, 0));
        }
    }
}

void FireBullet(Tiros *tiro, Zombies *zombie, int numero_tiros, int numero_zombies)
{

    if (TemZombie(zombie, NUM_ZOMBIES))
    {
        for(int k = 0; k < numero_zombies; k++)
        {
            if(zombie[k].live)
            {

                for (int i = 0; i < numero_tiros; i++)
                {
                    if(!tiro[i].live)
                    {
                        tiro[i].live = true;
                        tiro[i].x = 65;
                        tiro[i].y = zombie[k].y;
                        printf("atirou %d no zombie %d na pos(%d, %d)\n", i, k, tiro[i].x, tiro[i].y);
                        tiros_tela++;
                        break;
                    }
                    // impede que varios tiros sejam disparados ao mesmo tempo
                }
            }
        }
    }
}



void UpdateBullet(Tiros *tiro, int tamanho)
{
    for(int k = 0; k < tamanho; k++)
    {


        if(tiro[k].live)
        {
            tiro[k].x += tiro[k].speed;

        }
        if(tiro[k].x > WIDTH)
        {
            tiro[k].live = false;
            tiro[k].x = 65;
        }
    }
}

void ColisaoBulletZombie(Zombies *zombie, Tiros *tiro, int numero_zombies, int numero_tiros)
{
    for(int i = 0; i < numero_zombies; i++)
    {
        if(zombie[i].live)
        {
            for (int k = 0; k < numero_tiros; k++)
            {


                if(tiro[k].live)
                {
                    if(tiro[k].x > zombie[i].x - zombie[i].boundx
                            && tiro[k].x < zombie[i].x + zombie[i].boundx
                            && tiro[k].y > zombie[i].y - zombie[i].boundy
                            && tiro[k].y < zombie[i].y + zombie[i].boundy
                      )
                    {
                        zombie[i].live = false;
                        tiro[k].live = false;
                    }
                }
            }
        }
    }
}

void InitElectronic(Electronics *resistor, int numero_electronics)
{
    for(int i = 0; i < numero_electronics; i++)
    {
        resistor[i].ID = ELECTRONICS;
        resistor[i].life = 100;
        resistor[i].boundx = 50;
        resistor[i].boundy = 50;
        resistor[i].live = false;
    }

}

void DrawElectronic(Electronics *resistor, int numero_electronics)
{
    for(int i = 0; i < numero_electronics; i++)
    {
        if(resistor[i].live)
            al_draw_filled_circle(resistor[i].x, resistor[i].y, 20, al_map_rgb(0, 255, 0));
    }
    //al_draw_filled_circle(65, 96-51, 20, al_map_rgb(0, 255, 0));
    //al_draw_filled_circle(65, 2*96-51, 20, al_map_rgb(0, 255, 0));
    //al_draw_filled_circle(65, 3*96-51, 20, al_map_rgb(0, 255, 0));
    //al_draw_filled_circle(65, 4*96-51, 20, al_map_rgb(0, 255, 0));
    //al_draw_filled_circle(65, 5*96-51, 20, al_map_rgb(0, 255, 0));
}

void StartElectronic(Electronics *resistor, float mouse_x, float mouse_y)
{

    if(DaParaComprarResistor())
    {
        // [LINHA] [COLUNA]

        if(mouse_x < 100 && mouse_y < (2*96-51-40)) // [1] [1]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 65;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x < 100 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [1]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 65;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x < 100 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [1]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 65;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x < 100 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [1]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 65;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x < 100 && mouse_y > (5*96-51-40)) // [5] [1]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 65;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 115 && mouse_x < 185 && mouse_y < (2*96-51-40)) // [1] [2]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 150;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 115 && mouse_x < 185 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [2]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 150;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 115 && mouse_x < 185 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [2]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 150;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 115 && mouse_x < 185 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [2]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 150;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 115 && mouse_x < 185 && mouse_y > (5*96-51-40)) // [5] [2]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 150;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 200 && mouse_x < 270 && mouse_y < (2*96-51-40)) // [1] [3]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 235;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 200 && mouse_x < 270 && mouse_y > (2*96-51) && mouse_y < (3*96-51-40)) // [2] [3]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 235;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 200 && mouse_x < 270 && mouse_y > (3*96-51) && mouse_y < (4*96-51-40)) // [3] [3]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 235;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 200 && mouse_x < 270 && mouse_y > (4*96-51) && mouse_y < (5*96-51-40)) // [4] [3]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 235;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 200 && mouse_x < 270 && mouse_y > (5*96-51-40)) // [5] [3]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 235;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 285 && mouse_x < 350 && mouse_y < (2*96-51-40)) // [1] [4]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 315;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 285 && mouse_x < 350 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [4]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 315;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 285 && mouse_x < 350 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [4]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 315;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 285 && mouse_x < 350 && mouse_y > (2*96-51-40)) // [4] [4]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 315;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 285 && mouse_x < 350 && mouse_y > (2*96-51-40)) // [5] [4]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 315;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 365 && mouse_x < 435 && mouse_y < (2*96-51-40)) // [1] [5]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 400;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 365 && mouse_x < 435 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [5]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 400;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 365 && mouse_x < 435 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [5]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 400;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 365 && mouse_x < 435 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [5]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 400;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 365 && mouse_x < 435 && mouse_y > (5*96-51-40)) // [5] [5]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 400;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 450 && mouse_x < 515 && mouse_y < (2*96-51-40)) // [1] [6]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 485;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 450 && mouse_x < 515 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [6]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 485;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 450 && mouse_x < 515 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [6]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 485;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 450 && mouse_x < 515 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [6]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 485;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 450 && mouse_x < 515 && mouse_y > (5*96-51-40)) // [5] [6]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 485;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 535 && mouse_x < 605 && mouse_y < (2*96-51-40)) // [1] [7]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 570;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 535 && mouse_x < 605 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [7]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 570;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 535 && mouse_x < 605 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [7]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 570;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 535 && mouse_x < 605 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [7]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 570;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 535 && mouse_x < 605 && mouse_y > (5*96-51-40)) // [5] [7]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 570;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 535 && mouse_x < 605 && mouse_y > (5*96-51-40)) // [5] [7]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 570;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 615 && mouse_x < 690 && mouse_y < (2*96-51-40)) // [1] [8]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 655;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 615 && mouse_x < 690 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [8]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 655;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 615 && mouse_x < 690 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [8]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 655;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 615 && mouse_x < 690 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [8]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 655;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 615 && mouse_x < 690 && mouse_y > (5*96-51-40)) // [5] [8]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 655;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 705 && mouse_x < 775 && mouse_y < (2*96-51-40)) // [1] [9]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 740;
            resistor[V_ELT].y = 96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 705 && mouse_x < 775 && mouse_y > (2*96-51-40) && mouse_y < (3*96-51-40)) // [2] [9]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 740;
            resistor[V_ELT].y = 2*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 705 && mouse_x < 775 && mouse_y > (3*96-51-40) && mouse_y < (4*96-51-40)) // [3] [9]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 740;
            resistor[V_ELT].y = 3*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 705 && mouse_x < 775 && mouse_y > (4*96-51-40) && mouse_y < (5*96-51-40)) // [4] [9]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 740;
            resistor[V_ELT].y = 4*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
        if(mouse_x > 705 && mouse_x < 775 && mouse_y > (5*96-51-40)) // [5] [9]
        {
            resistor[V_ELT].live = true;
            resistor[V_ELT].x = 740;
            resistor[V_ELT].y = 5*96-51;
            energia_armazenada -= 100;
            V_ELT++;
        }
    }
}

    void InitEnergia (Energia *energia, int tamanho)
    {
        for(int i = 0; i < tamanho; i++)
        {
            energia[i].ID = ENERGIA;
            energia[i].live = false;
            energia[i].speed = 1;
        }
    }

    void DrawEnergia (Energia *energia, int tamanho)
    {
        for(int i = 0; i < tamanho; i++)
        {
            if(energia[i].live)
            {
                al_draw_filled_circle(energia[i].x, energia[i].y, 5, al_map_rgb(255, 255, 255));
            }
        }
    }

    void StartEnergia(Energia *energia, int tamanho)
    {
        for(int i = 0; i < tamanho; i++)
        {
            if(!energia[i].live)
            {
                if(rand() % 3000 == 0) // Gera um atraso pra nascer uma nova energia
                {
                    energia[i].live = true;
                    energia[i].x = rand() % (WIDTH);
                    energia[i].y = 0;
                }
            }
        }
    }

    void UpdateEnergia(Energia *energia, int tamanho)
    {
        for(int i = 0; i < tamanho; i++)
        {
            if(energia[i].live)
            {
                energia[i].y += energia[i].speed;
            }

            if(energia[i].y > HEIGHT)
            {
                energia[i].y = HEIGHT;
                energia[i].live = false;
            }
        }
    }
    void PegaEnergia(Energia *energia, float mouse_x, float mouse_y, int numero_energia)
    {

        for(int i = 0; i < numero_energia; i++)
        {
            if (energia[i].live)
            {
                if(mouse_x > (energia[i].x-20)
                        && mouse_x < (energia[i].x+20)
                        && mouse_y < (energia[i].y+20)
                        && mouse_y > (energia[i].y-20))
                {
                    energia[i].live = false;
                    energia_armazenada += 25;
                }
            }

        }
    }

    int DaParaComprarResistor()
    {
        if(energia_armazenada >= 100)
        {
            return 1;
        }
        return 0;

    }


