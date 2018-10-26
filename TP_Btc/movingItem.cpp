#include "movingItem.h"
#include <cmath>
#include "allegro5/allegro_image.h"

#define MOVINGIMAGE "moving.png"
#define V_TICKS 120 

movingItem::movingItem(float sx, float sy, float dx, float dy, int bitRad)
{
	//imagen = NULL;
	error = false;
	ticks = 0;
	posX = origenX = sx;
	posY = origenY = sy;
	destinoX = dx;
	destinoY = dy;
	alpha = atan2(destinoY - origenY, destinoX - origenX);
	radius = bitRad;

	/****primer intento de v fachera ****/
	speed = (sqrt((dx-sx)*(dx-sx)+ (dy - sy)*(dy - sy))/V_TICKS); // en 2 segundos lo deberia recorrer

	/*if (al_is_system_installed())
	{
		imagen = al_create_bitmap(sqrt(2)*bitRad, sqrt(2)*bitRad);
		ALLEGRO_BITMAP * temporario = al_load_bitmap(MOVINGIMAGE);
		ALLEGRO_DISPLAY * current = al_get_current_display();

		if (current!=NULL && imagen != NULL && temporario != NULL)
		{
			al_set_target_bitmap(imagen);
			al_draw_scaled_bitmap(temporario,
				0, 0, al_get_bitmap_width(temporario), al_get_bitmap_height(temporario),
				0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen),
				0);
			al_set_target_backbuffer(current);
			al_destroy_bitmap(temporario);
			error = false;
		}
		else
		{
			error = true;
		}

	}
	else
	{
		error = true;
	}*/
}


movingItem::~movingItem()
{
	//al_destroy_bitmap(imagen);
}

float movingItem::getY()
{
	return posY;
}

/*ALLEGRO_BITMAP * movingItem::get_bitpam()
{
	return imagen;
}*/

float movingItem::getX()
{
	return posX;
}

bool movingItem::isOk()
{
	return !error;
}

bool movingItem::move()
{
	bool ret = false;
	
	posX = origenX + speed * cos(alpha) * ticks;
	posY = origenY + speed * sin(alpha) * ticks;
	if ( ((destinoX - posX)*(destinoX - posX) + (destinoY - posY)*(destinoY - posY)) < (radius * radius) ) //arriesgado comparar los float asi pero hay que ver si anda
	{
		ret = true;
	}
	ticks++;
	return ret;
}
