//FileName:		viewcontroller.h
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		This file defines the header for the Viewcontroller class
//This class is responsible for setting up SDL and handing user input.

#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <SDL.h>  //Include SDL.h before glm.hpp
#include "model.h"
#include <glm.hpp>
using namespace glm;

class Viewcontroller
{
	private:
		bool quit;

		//Variables to control user movement
		vec3 eye;
		vec3 aim;
		vec3 up;
		mat4 view_matrix;
		double MOVEANGLE;
		double LOOKANGLE;
		double moveForward;
		double moveSideways;
		double baseX;
		double baseY;

		Model theWorld;  //Model we want to render

		SDL_Window *window;
		SDL_GLContext ogl4context;

	public:
		Viewcontroller();

		bool init();  //initializes SDL
		void display();
		void run();
		bool handleEvents(SDL_Event *theEvent);
		void updateLookAt();
};

#endif