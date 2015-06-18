#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "objects.h"

// VARIAVEIS GLOBAIS
const int WIDTH = 640; //Resolução X
const int HEIGHT = 480; //Resolução Y
enum KEYS{ UP, DOWN, LEFT, RIGHT}; //Introduz as teclas primitivas do teclado
const int FPS = 60;
const int NUM_ZOMBIES = 7;
const int LINHA_MAX = 5;
const int COL_MAX = 9;
const int WIDTH_PIXEL = WIDTH / LINHA_MAX; // Resoluçao X do pixel
const int HEIGHT_PIXEL = HEIGHT / COL_MAX; // Resoluçao Y do pixel


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



int main(void)
{
    // VARIAVEIS PRIMITIVAS
	bool done = false; //Variavel booleana para identificar se o programa terminou de ser executado
	bool redraw = true; //Enquanto essa variavel for verdadeira, ira ser desenhado algo na tela


	// STRUCTS DOS OBJETOS
	Zombies zombie[NUM_ZOMBIES];
	Electronics resistor;



	bool keys[4] ={false,false,false,false};

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	if(!al_init()) //Inicializa o Allegro
    {
    printf("Erro ao iniciar o Allegro");
    return -1;
    }

	display = al_create_display(WIDTH, HEIGHT);//Cria o display

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

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer); // Inicia o timer
    al_hide_mouse_cursor(display); // Faz o cursor do mouse não aparecer no display
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                redraw=true;
                StartZombie(zombie, NUM_ZOMBIES);
                UpdateZombie(zombie, NUM_ZOMBIES);
            }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {

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
					break;if(ev.type == ALLEGRO_EVENT_TIMER)
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = true;
                    break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = true;
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
    }
}


void DrawZombie (Zombies *zombie, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(zombie[i].live)
        {
            al_draw_filled_circle(zombie[i].x, zombie[i].y, 20, al_map_rgb(255, 0, 0));
            printf("desenho %d\n", i);
        }
    }
}
void StartZombie(Zombies *zombie, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(!zombie[i].live)
        {
            if(rand() % 500 == 0)
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
