//FileName:		Viewcontroller.cpp
//Programmer:	Dan Cliburn
//Date:			4/20/2015
//Purpose:		This file defines the methods for the Viewcontroller class
//See:  http://www.sdltutorials.com/sdl-tutorial-basics/
//		http://www.sdltutorials.com/sdl-opengl-tutorial-basics/
//		http://stackoverflow.com/questions/13826150/sdl2-opengl3-how-to-initialize-sdl-inside-a-function
//for many more details on how to write an OpenGL program using SDL.  You might also want to go to these 
//pages which will link you to other tutorials on how to do stuff with SDL.
//Be warned, however, that a lot of the tutorials describe SDL 1.2, but we will be using SDL 2 in this course.
//
//Specific to this lab, I found some helpful information on the following pages for centering the mouse in SDL 2 
//and trapping it in the window (i.e. you can't move the mouse outside the window)
//	http://stackoverflow.com/questions/10492143/sdl2-mouse-grab-is-not-working
//	http://gamedev.stackexchange.com/questions/33519/trap-mouse-in-sdl
//	http://www.ginkgobitter.org/sdl/?SDL_ShowCursor
//
//A big change in this class is that the user now moves around the scene using a traditional
//first person controller. Movement is controlled with wasd and view direction is changed with the mouse.
//The program now ends when the user presses the <Esc> key.

#include <SDL.h>
#include "viewcontroller.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

const int WINDOWWIDTH = 1600;
const int WINDOWHEIGHT = 800;
const double PI = 3.14159;
const double MOVESPEED = 0.75;

Viewcontroller::Viewcontroller()
{
	quit = false;
	window = 0;
	ogl4context = 0;

	view_matrix = mat4(1.0);

	moveForward = 0.0;
	moveSideways = 0.0;
	moveUp = 0.0;
	MOVEANGLE = PI/2.0;
	LOOKANGLE = 0.0;
	baseX = WINDOWWIDTH / 2.0;
	baseY = WINDOWHEIGHT / 2.0;
	eye = vec3(0.0, 0.0, 0.0);
	up = vec3(0.0, 1.0, 0.0);
	updateLookAt();  //aim will be calculated from the initial values of eye and MOVEANGLE
}

//Initializes SDL, GLEW, and OpenGL
bool Viewcontroller::init()
{
	//First initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Failed to initialize SDL." << endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	if ((window = SDL_CreateWindow("Lab 15 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGHT, SDL_WINDOW_OPENGL)) == NULL)
	{
		cout << "Failed to create window." << endl;
		return false;
	}
	ogl4context = SDL_GL_CreateContext(window);
	SDL_ShowCursor(0);  //Hide the mouse cursor

	//Initialize the Model that you want to render
	if (theWorld.init() == false)  //OpenGL initialization is done in the Model class
	{
		cout << "Failed to initialize theWorld." << endl;
		return false;
	}
	return true;  //Everything got initialized
}

//Display what we want to see in the graphics window
void Viewcontroller::display()
{
	theWorld.draw();

	SDL_GL_SwapWindow(window);
}


bool Viewcontroller::handleEvents(SDL_Event *theEvent)
{
	switch(theEvent->type)
	{
		case SDL_QUIT:  //User pressed the escape key
		{
			return true;  //force program to quit
		}
		case SDL_KEYDOWN:
		{
			switch (theEvent->key.keysym.sym) {
			case SDLK_ESCAPE:
				return true;
			case SDLK_w:
			case SDLK_UP:
				forward = true;
				break;
			case SDLK_s:
			case SDLK_DOWN:
				backward = true;
				break;
			case SDLK_a:
			case SDLK_LEFT:
				left = true;
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				right = true;
				break;
			case SDLK_SPACE:
				upward = true;
				break;
			case SDLK_LSHIFT:
				downward = true;
				break;
			case SDLK_1:
				theWorld.usePhong();
				break;
			case SDLK_2:
				theWorld.useToon();
				break;
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (theEvent->key.keysym.sym) {
			case SDLK_w:
			case SDLK_UP:
				forward = false;
				break;
			case SDLK_s:
			case SDLK_DOWN:
				backward = false;
				break;
			case SDLK_a:
			case SDLK_LEFT:
				left = false;
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				right = false;
				break;
			case SDLK_SPACE:
				upward = false;
				break;
			case SDLK_LSHIFT:
				downward = false;
				break;
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			const double MOUSE_SENSITIVITY_X = .002;
			const double MOUSE_SENSITIVITY_Y = .002;
			MOVEANGLE += (theEvent->button.x - baseX) * MOUSE_SENSITIVITY_X;
			LOOKANGLE += -(theEvent->button.y - baseY) * MOUSE_SENSITIVITY_Y * (WINDOWWIDTH/WINDOWHEIGHT);
			if (LOOKANGLE > PI/2) {
				LOOKANGLE = PI/2;
			} else if (LOOKANGLE < -PI/2) {
				LOOKANGLE = -PI/2;
			}
			SDL_WarpMouseInWindow(window, baseX, baseY);  //re-center the mouse cursor
			break;
		}
	} //end the switch
	moveForward = (forward - backward) * 0.075 * MOVESPEED;
	moveSideways = (right - left) * 0.075 * MOVESPEED;
	moveUp = (upward - downward) * 0.075 * MOVESPEED;
	return false;  //the program should not end
}

void Viewcontroller::updateLookAt()
{
	////Add movement forward and backward
	eye[0] += cos(MOVEANGLE + 3*PI / 2.0)*moveForward;
	eye[2] += sin(MOVEANGLE + 3*PI / 2.0)*moveForward;

	////Add movement to the left and right
	eye[0] += cos(MOVEANGLE)*moveSideways;
	eye[2] += sin(MOVEANGLE)*moveSideways;
	eye[1] += moveUp;

	////Adjust the aim position from the new eye position
	//aim[0] = 0;
	////aim[1] = eye[1] + LOOKANGLE;
	//aim[1] = 0;
	//aim[2] = 0;

	//view_matrix = lookAt(eye, aim, up);  //calculate the view orientation matrix
	float yaw = -MOVEANGLE;
	float pitch = LOOKANGLE;
	float cosPitch = cos(pitch);
	float sinPitch = sin(pitch);
	float cosYaw = cos(yaw);
	float sinYaw = sin(yaw);

	vec3 xaxis = { cosYaw, 0, -sinYaw };
	vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	mat4 viewMatrix = {
		vec4(xaxis.x,            yaxis.x,            zaxis.x,      0),
		vec4(xaxis.y,            yaxis.y,            zaxis.y,      0),
		vec4(xaxis.z,            yaxis.z,            zaxis.z,      0),
		vec4(-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1)
	};
	//view_matrix = viewMatrix;
	theWorld.setViewMatrix(viewMatrix);
}

void Viewcontroller::run()
{
	if (init() == false)  //This method (defined above) sets up OpenGL, SDL, and GLEW
	{
		cout << "Program failed to initialize ... exiting." << endl;
		return;
	}

	SDL_Event events;  //Makes an SDL_Events object that we can use to handle events

	const int UPDATE_FREQUENCY = 10; //update the frame every 10 milliseconds
	long currentTime, startTime = clock();
	SDL_WarpMouseInWindow(window, baseX, baseY);  //Center the mouse cursor
	do
	{
		display();  //This method (defined above) draws whatever we have defined
		while (SDL_PollEvent(&events)) //keep processing the events as long as there are events to process
		{
			quit = handleEvents(&events);
		}

		currentTime = clock();
		if (currentTime - startTime > UPDATE_FREQUENCY)
		{
			updateLookAt();
			theWorld.updateWorld();

			startTime = currentTime;
		}

	} while (!quit); //run until "quit" is true (i.e. user presses the <Esc> key

	SDL_GL_DeleteContext(ogl4context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}