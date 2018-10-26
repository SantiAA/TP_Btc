#pragma once
#include "GraphicNode.h"
#include "movingItem.h"

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"


#include <vector>
#include <ctime>
#include <cmath>

#define FPS 60.0

enum class GUIEvents { NO_EVENTS , CLICK , CLOSE , TIMER};

template<typename T1>class GUI
{
public:
	GUI(int node_num) {
		myVector.clear();
		animation.clear();
		srand(time(NULL));
		basicBackground = miner = fullService = nullptr;
		eventqueue = nullptr;
		display = nullptr;
		initAlleg(node_num);
		eventType = GUIEvents::NO_EVENTS;
	};

	~GUI() {
		closeAlleg();
	};

	void addNode(T1 newNode) {  //al nodo de cosas que me pasan le agrego lo que necesito para manejar el diplay
		myVector.push_back(GraphicNode<T1>(newNode, randPos() ));
	};

	void makeDisplay() {
		al_set_target_bitmap(basicBackground);

		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_draw_filled_rectangle(DWith*0.9, 0, DWith, DHight, al_map_rgb(0, 15, 160));

		for (GraphicNode<T1> intresting : myVector)
		{
			if (rand() % 2)
			{
				al_draw_bitmap(miner, intresting.getX() - (RNodo*sqrt(2) / 2), intresting.getY() - (RNodo * sqrt(2) / 2), 0);
			}
			else
			{
				al_draw_bitmap(fullService, intresting.getX() - (RNodo*sqrt(2) / 2), intresting.getY() - (RNodo * sqrt(2) / 2), 0);
			}
		}
		al_set_target_backbuffer(display);
		al_draw_bitmap(basicBackground, 0, 0, 0);
		al_flip_display();
	};

	void refresh(void) 
	{
		int mousex = 0, mousey = 0 ;
		al_draw_bitmap(basicBackground,0,0,0);

		al_get_mouse_cursor_position(&mousex, &mousey);

		for (int i = 0; i< myVector.size();i++)
		{	
			if (myVector[i].isSelected()) // si esta seleccionado pinto circulo verde
			{
				al_draw_circle(myVector[i].getX(), myVector[i].getY(), RNodo, al_map_rgb(0, 0, 250), RNodo / 20);
			}
			unsigned int tempx = (mousex - myVector[i].getX());
			unsigned int tempy = (mousey - myVector[i].getY());
			if (((tempx * tempx) + (tempy*tempy)) < (RNodo*RNodo)) // me fijo si el mouse toca un nodo
			{
				if (!myVector[i].isSelected()) // si el nodo no esta seleccionado le hago circulo rojo
				{
					al_draw_circle(myVector[i].getX(), myVector[i].getY(), RNodo, al_map_rgb(250, 0, 0), RNodo / 20);
				}
			}
		}
		
		for (int i = 0; i < animation.size(); i++)
		{
			bool comoEstaElanimation;
			comoEstaElanimation = animation[i].move();
			
			al_draw_filled_circle(animation[i].getX(), animation[i].getY(), RNodo/4,al_map_rgb(255,100,10));
			
			if (comoEstaElanimation != false)
			{
				animation.erase(animation.begin()+i);
				i--; // medio turbio pero deberia funcionar pq estoy sacando un elemeto del vector
			}
		}
		al_flip_display();
	};

	bool anyEvent(void)
	{
		ALLEGRO_EVENT ev;
		if (al_get_next_event(eventqueue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
			{
				eventType = GUIEvents::CLICK;
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				eventType = GUIEvents::CLOSE;
			}
			else if (ev.type = ALLEGRO_EVENT_TIMER)
			{
				eventType = GUIEvents::TIMER;
				std::cout << "timer event\n" << ev.type << std::endl;
			}
			return true;
		}
		return false;
	}

	GUIEvents getEvent(void)
	{
		return eventType;
	}

	void click(void)
	{
		int mousex = 0, mousey = 0;
		al_get_mouse_cursor_position(&mousex, &mousey);
		

		for (int i = 0; i< myVector.size();i++)
		{
			unsigned int tempx = (mousex - myVector[i].getX());
			unsigned int tempy = (mousey - myVector[i].getY());
			if (((tempx * tempx) + (tempy*tempy)) < (RNodo*RNodo)) // me fijo si el mouse toca un nodo
			{
				if (myVector[i].isSelected())
				{
					myVector[i].toggelSelected();
				}
				else
				{
					
					for (int j = 0; j < myVector.size() ; j++ )
					{
						if (myVector[j].isSelected())
						{
							myVector[j].toggelSelected();
						}
					}
					myVector[i].toggelSelected();
				}
			}
		}

		if (mousex > DWith * 0.9) // para probar los movimientos
		{
			int nodo1 = rand() % myVector.size();
			int nodo2 = rand() % myVector.size();
		
			if (nodo1 == nodo2) // por si sale el mismo
			{
				nodo2--;
			}
			
			animation.push_back(movingItem(myVector[nodo1].getX(), myVector[nodo1].getY(), myVector[nodo2].getX(), myVector[nodo2].getY(), RNodo / 4));

			if (!animation.back().isOk()) // si no se inicializo bien lo elimino , tentativo para probar
			{
				animation.pop_back();
			}
		}
	}

private:
	pos randPos() {
		int xmax = (DWith*0.9 - 2 * RNodo);
		int ymax = (DHight - 2 * RNodo);

		pos tempPosition = { rand() % xmax + RNodo , rand() % ymax + RNodo }; // dejo un bordecito para la barra del costado
		bool posOk = true;

		do {

			posOk = true;
			for (GraphicNode<T1> myNode : myVector)
			{
				int xdif = (tempPosition.xCoord - myNode.getX());
				xdif = (xdif*xdif);
				int ydif = (tempPosition.yCoord - myNode.getY());
				ydif = (ydif*ydif);

				if ((xdif + ydif) <= (4*RNodo*RNodo)) // si estoy donde se encuentra otro nodo lo indico
				{
					posOk = false;
				}
			}

			if (!posOk)
			{
				tempPosition.xCoord = rand() % xmax + RNodo; // busco otra posicion al azar
				tempPosition.yCoord = rand() % ymax + RNodo;
			}

		} while (posOk != true);

		return tempPosition;
		
	};

	void initAlleg(int NumOfNode) {
		ALLEGRO_DISPLAY_MODE displayData;
		if (al_init() == true)
		{
			if (al_get_display_mode(al_get_num_display_modes()-1, &displayData) != NULL)
			{
				DWith = displayData.width * 0.98;
				DHight = displayData.height* 0.9;
				RNodo = floor(sqrt((0.9*DWith*DHight*0.15) / (NumOfNode*ALLEGRO_PI)));

				display = al_create_display(DWith,DHight); // creo un display con el tamaño de la pantalla
				if (display != NULL)
				{
					al_clear_to_color(al_map_rgb(120, 120, 120));
					al_flip_display();
					al_set_window_position(display, (displayData.width- DWith)/2 , 0);
					if (al_install_mouse() && al_install_keyboard())
					{
						if (al_init_image_addon() && al_init_primitives_addon())
						{
							int temp = floor(sqrt(2)*RNodo); // cuadrado inscripto en circunferencia
							miner = al_create_bitmap(temp, temp);
							fullService = al_create_bitmap(temp, temp);
							basicBackground = al_create_bitmap(DWith,DHight);
							if ((miner != NULL) && (fullService != NULL) && (basicBackground != NULL))
							{
								ALLEGRO_BITMAP * tempMiner = al_load_bitmap("miner.png");
								ALLEGRO_BITMAP * tempFS = al_load_bitmap("FS.png");
								if (tempMiner != NULL && tempFS != NULL)
								{
									al_set_target_bitmap(miner); // guardo las fotos del minero y del full service para despues
									al_draw_scaled_bitmap(tempMiner,
										0, 0, al_get_bitmap_width(tempMiner), al_get_bitmap_height(tempMiner),
										0, 0, al_get_bitmap_width(miner), al_get_bitmap_height(miner),
										0);
									al_set_target_bitmap(fullService);
									al_draw_scaled_bitmap(tempFS,
										0, 0, al_get_bitmap_width(tempFS), al_get_bitmap_height(tempFS),
										0, 0, al_get_bitmap_width(fullService), al_get_bitmap_height(fullService),
										0);

									al_destroy_bitmap(tempMiner);  //destruyo los bitmaps temporales de las fotos
									al_destroy_bitmap(tempFS);
									al_set_target_backbuffer(display);

									if (timer = al_create_timer(1.0 / FPS))
									{
										if ( (eventqueue = al_create_event_queue()) != NULL)
										{
											al_register_event_source(eventqueue, al_get_display_event_source(display));
											al_register_event_source(eventqueue, al_get_mouse_event_source());
											//al_register_event_source(eventqueue, al_get_timer_event_source(timer));

											//al_start_timer(timer);

											if (al_init_ttf_addon() && al_init_font_addon())
											{
												return;
											}
											al_destroy_event_queue(eventqueue);
										}
										al_destroy_timer(timer);
									}
								}

								al_destroy_bitmap(miner);
								al_destroy_bitmap(fullService);
								al_destroy_bitmap(basicBackground);
							}
						}
					}
				}
				al_destroy_display(display);
			}
		}

	};

	void closeAlleg(void) {
		al_destroy_timer(timer);
		al_destroy_event_queue(eventqueue);
		al_destroy_bitmap(basicBackground);
		al_destroy_bitmap(miner);
		al_destroy_bitmap(fullService);
		al_destroy_display(display);
		al_shutdown_image_addon();
		al_shutdown_primitives_addon();

	};
	
	std::vector< GraphicNode<T1> > myVector;
	std::vector<movingItem> animation;

	unsigned int DWith;
	unsigned int DHight;
	unsigned int RNodo; // es el radio del nodo en la imagen del display

	GUIEvents eventType;

	ALLEGRO_DISPLAY * display;
	ALLEGRO_BITMAP * basicBackground;
	ALLEGRO_BITMAP * miner;
	ALLEGRO_BITMAP * fullService;
	ALLEGRO_EVENT_QUEUE * eventqueue;
	ALLEGRO_TIMER * timer;

	
};

