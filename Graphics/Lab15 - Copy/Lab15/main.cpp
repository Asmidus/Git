//FileName:		main.cpp
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		This file defines the main() function for Lab 15.
//Movement is controller with wasd and view direction is changed with the mouse.
//The program ends when the user presses the <Esc> key

#include "viewcontroller.h"
#include <stdlib.h>

int main(int argc, char *argv[])  //main() must take these parameters when using SDL
{
	Viewcontroller vc;

	vc.run();

	system("pause");
	return 0;
}