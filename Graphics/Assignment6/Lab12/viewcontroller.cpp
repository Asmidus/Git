//FileName:		Viewcontroller.cpp
//Programmer:	Dan Cliburn
//Date:			4/14/2015
//Purpose:		This file defines the methods for the Viewcontroller class.
//The Viewcontroller uses SDL to set up our graphics window (the "view") and handle events (the "controller").
//Thus, all SDL functions are called in this class.
//
//See:  http://www.sdltutorials.com/sdl-tutorial-basics/
//		http://www.sdltutorials.com/sdl-opengl-tutorial-basics/
//		http://stackoverflow.com/questions/13826150/sdl2-opengl3-how-to-initialize-sdl-inside-a-function
//for many more details on how to write an OpenGL program using SDL.  You might also want to go to these 
//pages which will link you to other tutorials on how to do stuff with SDL.
//Be warned, however, that a lot of the tutorials describe SDL 1.2, but we will be using SDL 2 in this course.
//
//ACKNOWLEDGEMENT:  I found some helpful code examples at the bottom of this page: 
//	http://www.gamedev.net/topic/658486-glsl-send-array-receive-struct/
//that showed how to pass the array of LightProperties to the shaders.

#include <SDL.h>
#include "viewcontroller.h"
#include <iostream>
using namespace std;

const int WINDOWWIDTH = 600;
const int WINDOWHEIGHT = 600;

Viewcontroller::Viewcontroller()
{
	quit = false;
	window = 0;
	ogl4context = 0;

	ROTATING = false;
	xAngle = yAngle = 0.0;
	lastOffsetX = lastOffsetY = 0;

	lightToggles[0] = lightToggles[1] = lightToggles[2] = 1;
}

//Initializes SDL
bool Viewcontroller::init()
{
	//First initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Failed to initialize SDL." << endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	if ((window = SDL_CreateWindow("Lab 12 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGHT, SDL_WINDOW_OPENGL)) == NULL)
	{
		cout << "Failed to create window." << endl;
		return false;
	}
	ogl4context = SDL_GL_CreateContext(window);

	if (m.init() == false)
	{
		cout << "Could not initialize Model object" << endl;
		return false;
	}
	
	return true;  //Everything got initialized
}

//Display what we want to see in the graphics window
void Viewcontroller::display()
{
	m.draw();

	SDL_GL_SwapWindow(window);
}


bool Viewcontroller::handleEvents(SDL_Event *theEvent)
{
	switch(theEvent->type)
	{
		case SDL_QUIT:  //user clicked on the 'X' in the window
		{
			return true;  //force program to quit
		}
		case SDL_KEYDOWN:
		{
			if (theEvent->key.keysym.sym == SDLK_0)  //the <Esc> key
			{
				lightToggles[0] = (lightToggles[0] + 1) % 2;
			}
			else if (theEvent->key.keysym.sym == SDLK_1)
			{
				lightToggles[1] = (lightToggles[1] + 1) % 2;
			}
			else if (theEvent->key.keysym.sym == SDLK_2)
			{
				lightToggles[2] = (lightToggles[2] + 1) % 2;
			}
			m.updateLights(lightToggles);

			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(1))  //Attach rotation to the left mouse button
			{
				// save position where button down event occurred. This 
				// is the "zero" position for subsequent mouseMotion callbacks. 
				baseX = theEvent->button.x;
				baseY = theEvent->button.y;
				ROTATING = true;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (ROTATING)  //are we finishing a rotation?
			{
				//Remember where the motion ended, so we can pick up from here next time. 
				lastOffsetX += (theEvent->button.x - baseX);
				lastOffsetY += (theEvent->button.y - baseY);
				ROTATING = false;
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			//Is the left mouse button also down?
			if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(1))
			{
				float x, y;

				//Calculating the conversion => window size to angle in degrees 
				float scaleX = 360.0 / WINDOWWIDTH;
				float scaleY = 360.0 / WINDOWHEIGHT;

				x = (theEvent->button.x - baseX) + lastOffsetX;
				y = (theEvent->button.y - baseY) + lastOffsetY;

				// map "x" to a rotation about the y-axis. 
				x *= scaleX;
				yAngle = x;

				// map "y" to a rotation about the x-axis. 
				y *= scaleY;
				xAngle = y;

				m.update(xAngle, yAngle); //send the new angles to the Model object
			}
			break;
		}
	} //end the switch
	return false;  //the program should not end
}

void Viewcontroller::run()
{
	if (init() == false)  //This method (defined above) sets up OpenGL, SDL, and GLEW
	{
		cout << "Program failed to initialize ... exiting." << endl;
		return;
	}

	SDL_Event events;  //Makes an SDL_Events object that we can use to handle events

	//The following is the main program loop.  Essentially it keeps running until the
	//user clicks on the 'X' in the graphics window.
	do
	{
		display();  //This method (defined above) draws whatever we have defined
		while (SDL_PollEvent(&events)) //keep processing the events as long as there are events to process
		{
			quit = handleEvents(&events);
		}

	} while (!quit); //run until "quit" is true

	SDL_GL_DeleteContext(ogl4context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}