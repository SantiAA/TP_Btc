#pragma once

#include "allegro5/allegro.h"
 


class movingItem
{
public:
	movingItem(float sx, float sy, float dx, float dy, int bitRad);
	~movingItem();

	//ALLEGRO_BITMAP * get_bitpam();
	float getX();
	float getY();

	bool isOk();

	bool move();
	
	
private:

	//ALLEGRO_BITMAP * imagen;
	float posX;
	float posY;

	float origenX;
	float origenY;

	float destinoX;
	float destinoY;

	float ticks;
	float alpha;

	float speed;

	bool finished;
	bool error;

	int radius;
};

