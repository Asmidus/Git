//FileName:		main.cpp
//Programmer:	Dan Cliburn
//Date:			4/1/2015
//Purpose:		This file defines the main() function for Lab 7.

#include "viewcontroller.h"
#include <stdlib.h>

int main(int argc, char *argv[])  //main() must take these parameters when using SDL
{
	Viewcontroller vc;

	vc.run();

	system("pause");
	return 0;
}