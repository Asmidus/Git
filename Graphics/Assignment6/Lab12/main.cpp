//FileName:		main.cpp
//Programmer:	Dan Cliburn
//Date:			4/14/2015
//Purpose:		This file defines the main() function for Lab 12.

#include "viewcontroller.h"
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char *argv[])  //main() must take these parameters when using SDL
{
	Viewcontroller vc;

	vc.run();

	system("pause");
	return 0;
}