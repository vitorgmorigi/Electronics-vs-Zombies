#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "objects.h"

// VARIAVEIS GLOBAIS
const int WIDTH = 640; //Resolução X
const int HEIGHT = 480; //Resolução Y
enum KEYS { UP, DOWN, LEFT, RIGHT, ONE}; //Introduz as teclas primitivas do teclado
const int FPS = 60;
const int NUM_ZOMBIES = 7;
const int NUM_TIROS = 30;
const int NUM_ENERGIA = 4;
const int LINHA_MAX = 5;
const int COL_MAX = 9;
const int WIDTH_PIXEL = WIDTH / LINHA_MAX; // Resoluçao X do pixel
const int HEIGHT_PIXEL = HEIGHT / COL_MAX; // Resoluçao Y do pixel
int timer2;

int mapa[5][9] =
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

void InitElectronic(Electronics &resistor);
void DrawElectronic(Electronics &resistor);
void StartElectronic(Electronics &resistor);

void InitBullet(Tiros *tiro, int tamanho);
void DrawBullet(Tiros *tiro, int tamanho);
void FireBullet(Tiros *tiro, Zombies *zombie, int numero_tiros);
void UpdateBullet(Tiros *tiro, int tamanho);

void ColisaoBulletZombie(Zombies *zombie, Tiros *tiro, int numero_zombies);


void InitEnergia (Energia *energia, int tamanho);
void DrawEnergia (Energia *energia, int tamanho);
void StartEnergia(Energia *energia, int tamanho);
void UpdateEnergia(Energia *energia, int tamanho);
void PegaEnergia(Energia *energia, float mouse_x, float mouse_y, int numero_energia);

int TemZombie(Zombies *zombie, int tamanho);



int main(void)
{
    // VARIAVEIS PRIMITIVAS
    bool done = false; //Variavel booleana para identificar se o programa terminou de ser executado
    bool redraw = true; //Enquanto essa variavel for verdadeira, ira ser desenhado algo na tela



    // STRUCTS DOS OBJETOS
    Zombies zombie[NUM_ZOMBIES];
    Electronics resistor;
    Tiros tiro[NUM_TIROS];
    Energia energia[NUM_ENERGIA];


    bool keys[4] = {false,false,false,false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    if(!al_init()) //Inicializa o Allegro
    {
        printf("Erro ao iniciar o Allegro");
        return -1;
    }

    display = al_create_display(WIDTH, HEIGHT);//Cria o display -

    if(!display)//Testa o display
    {
        printf("Erro ao iniciar o display");
        return -1;
    }
    al_init_primitives_addon(); //Introduz os comandos primitivos(figuras geometricas) do Allegro
    al_install_mouse(); // Introduz o comando do mouse
    al_install_keyboard(); //Introduz o comando de teclas

    event_queue = al_create_event_queue();
    timer = al_create_timer(0.20 / FPS); // Define o tempo de atualizaçao do timer

    srand(time(NULL));
    InitZombie(zombie, NUM_ZOMBIES);
    InitElectronic(resistor);
    InitEnergia(energia, NUM_ENERGIA);
    InitBullet(tiro, NUM_TIROS);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer); // Inicia o timer
    // Faz o cursor do mouse não aparecer no display
    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw=true;
            StartZombie(zombie, NUM_ZOMBIES);
            UpdateZombie(zombie, NUM_ZOMBIES);
            StartEnergia(energia, NUM_ENERGIA);
            UpdateEnergia(energia, NUM_ENERGIA);
            UpdateBullet(tiro, NUM_ZOMBIES);
            ColisaoBulletZombie(zombie, tiro, NUM_ZOMBIES);
            timer2++;

                if(timer2 >= 120) // faz os Electronics atirarem numa velocidade constante
                {
                    FireBullet(tiro, zombie, NUM_TIROS);
                    timer2 = 0;
                }

        }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            PegaEnergia(energia, ev.mouse.x, ev.mouse.y, NUM_ENERGIA);
        }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {

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
                if(ev.type == ALLEGRO_EVENT_TIMER)
                case ALLEGRO_KEY_RIGHT:
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
            DrawElectronic(resistor);
            DrawBullet(tiro, NUM_ZOMBIES);
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
        zombie[i].speed = 0.10;
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
                printf("vivo %d\n", i);
                zombie[i].x = WIDTH;
                zombie[i].y = 45 + 96 * rand() % (HEIGHT);
            }
            break;
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

        if(zombie[i].live == false)
            return 0;

    }
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

void FireBullet(Tiros *tiro, Zombies *zombie, int numero_tiros)
{

    if (TemZombie(zombie, NUM_ZOMBIES))
    {
        for (int k = 0; k < numero_tiros; k++)
        {
            if(!tiro[k].live)
            {
                tiro[k].live = true;
                tiro[k].x = 65;
                tiro[k].y = zombie[k].y;
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

void ColisaoBulletZombie(Zombies *zombie, Tiros *tiro, int numero_zombies)
{
    for(int i = 0; i < numero_zombies; i++)
    {
        if(zombie[i].live)
        {
            if(tiro[i].live)
            {
                if(tiro[i].x > zombie[i].x - zombie[i].boundx
                   && tiro[i].x < zombie[i].x + zombie[i].boundx
                   && tiro[i].y > zombie[i].y - zombie[i].boundy
                   && tiro[i].y < zombie[i].y + zombie[i].boundy
                   )
                {
                    zombie[i].live = false;
                    tiro[i].live = false;
                }
            }
        }
    }
}

void InitElectronic(Electronics &resistor)
{
    resistor.ID = ELECTRONICS;
    resistor.life = 100;
    resistor.boundx = 50;
    resistor.boundy = 50;
}

void DrawElectronic(Electronics &resistor)
{
    al_draw_filled_circle(65, 96-51, 20, al_map_rgb(0, 255, 0));
    al_draw_filled_circle(65, 2*96-51, 20, al_map_rgb(0, 255, 0));
    al_draw_filled_circle(65, 3*96-51, 20, al_map_rgb(0, 255, 0));
    al_draw_filled_circle(65, 4*96-51, 20, al_map_rgb(0, 255, 0));
    al_draw_filled_circle(65, 5*96-51, 20, al_map_rgb(0, 255, 0));
}

void StartElectronic(Electronics &resistor)
{

}

void InitEnergia (Energia *energia, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        energia[i].ID = ENERGIA;
        energia[i].live = false;
        energia[i].speed = 0.1;
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
        printf("desenho %d\n", i);
    }
}

void StartEnergia(Energia *energia, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(!energia[i].live)
        {
            if(rand() % 10000 == 0) // Gera um atraso pra nascer uma nova energia
            {
                energia[i].live = true;
                energia[i].x = rand() % (WIDTH);
                energia[i].y = 0;
                printf("criou %d\n",i);
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
            printf("atualizou %d %.2f\n",i, energia[i].y);
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
        }
        }

    }
}
