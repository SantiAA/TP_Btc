
#include "GUI.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

int main(int argc, char ** argv)
{
	int nodos = atoi(argv[1]); // solo para probar

	GUI<int> myinterfase(nodos);
	for (int i = 0; i < nodos; i++)
	{
		myinterfase.addNode(i);
	}
	myinterfase.makeDisplay();

	bool salir = false;

	while (!salir)
	{
		if (myinterfase.anyEvent())
		{
			switch (myinterfase.getEvent())
			{
			case GUIEvents::TIMER:myinterfase.refresh(); break;
			case GUIEvents::CLICK:myinterfase.click(); break;
			case GUIEvents::CLOSE:salir = true; break;
			default: break;
			}
		}
	}
}