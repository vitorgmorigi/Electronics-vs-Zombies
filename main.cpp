#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

enum KEYS{ UP, DOWN, LEFT, RIGHT};				//Our primitive header file;

int main(void)
{
	int width = 640; //resolução y
	int height = 480; //resolução x

	bool done = false; //variavel booleana para identificar se o programa terminou de ser executado
	int pos_x = width / 2;
	int pos_y = height / 2;

	bool keys[4] ={false,false,false,false};

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(width, height);			//create our display object

	if(!display)										//test display object
		return -1;

	al_init_primitives_addon(); //introduz os comandos primitivos(figuras geometricas) do allegro
	al_install_keyboard(); //introduz o comando de teclas

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
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

        pos_y -= keys[UP] * 10;
        pos_y += keys[DOWN] * 10;
        pos_x += keys[RIGHT] * 10;
        pos_x -= keys[LEFT] * 10;



        al_draw_filled_rectangle(pos_x, pos_y-170, pos_x+30, pos_y-200, al_map_rgb(255,255,0));
		al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(0,255,0));
		al_draw_filled_rectangle(pos_x, pos_y+170, pos_x+30, pos_y+200, al_map_rgb(0,255,255));
		al_flip_display();
		al_clear_to_color(al_map_rgb(255,255,255));
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}
