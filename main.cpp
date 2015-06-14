#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "objects.h"
// VARIAVEIS GLOBAIS
const int WIDTH = 640; //Resolução Y
const int HEIGHT = 480; //Resolução X
enum KEYS{ UP, DOWN, LEFT, RIGHT}; //Introduz as teclas primitivas do teclado;


int main(void)
{

	bool done = false; //Variavel booleana para identificar se o programa terminou de ser executado
	bool redraw = true; //Enquanto essa variavel for verdadeira, ira ser desenhado algo na tela

	int pos_x = 0;
	int pos_y = HEIGHT / 2;

	int FPS = 60;

	bool keys[4] ={false,false,false,false};

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	if(!al_init()) //Inicializa o Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);//Cria o display

	if(!display)//Testa o display
		return -1;

	al_init_primitives_addon(); //Introduz os comandos primitivos(figuras geometricas) do Allegro
	al_install_mouse(); // Introduz o comando do mouse
	al_install_keyboard(); //Introduz o comando de teclas

	event_queue = al_create_event_queue();
	timer = al_create_timer(0.20 / FPS); // Define o tempo de atualizaçao do timer

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
                pos_x += 1;
                redraw=true;
            }

        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            pos_x = ev.mouse.x;
            pos_y = ev.mouse.y;
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

        // Quadrados que representam os tiros
        al_draw_filled_rectangle(pos_x, pos_y-170, pos_x+30, pos_y-200, al_map_rgb(255,255,0));
        al_draw_filled_rectangle(pos_x, pos_y-70, pos_x+30, pos_y-100, al_map_rgb(160,255,0));
		al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(0,255,0));
		al_draw_filled_rectangle(pos_x, pos_y+95, pos_x+30, pos_y+125, al_map_rgb(160,255,120));
		al_draw_filled_rectangle(pos_x, pos_y+185, pos_x+30, pos_y+215, al_map_rgb(0,255,255));
		al_flip_display();
		al_clear_to_color(al_map_rgb(255,255,255));
        }
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display); //Fecha o display

	return 0;
}
