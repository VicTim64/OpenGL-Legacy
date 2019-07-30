#include "GameEngine.h"
#include "SpaceShip.h"
#include "Asteroid.h"
#include "Sun.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "getbmp.h"
//static unsigned int texture[1]; // Array of texture indices.
static float d = 0.0; // Distance parameter in gluLookAt().

int main(int argc, char **argv) {

	
	//Initialise the engine.
	GameEngine::initEngine(argc, argv, "OpenGL Racing Game", false);

	GameEngine::initialize(); //Light
	
	
	//GameEngine::addGameObject(new SpaceShip(glm::vec3(3, 0.0, -20.0)), true); //SpaceShip

//	GameEngine::addGameObject(new Sun(glm::vec3(80, 0.0, -85.0), { 1,2,4 }, { 1.4,80,80 })); //Sun
  //// TRACKKK
	for (int i = 0; i < 40; i++) // 1st Square
	{
		GameEngine::addGameObject(new Asteroid(glm::vec3(i * 4, 0, -25.00 ), { 1,0,0 }, { 1.4, 30, 30 }));
		GameEngine::addGameObject(new Asteroid(glm::vec3(160, 0, -25.00 - i * 4), { 1,0,0 }, { 1.4, 30, 30 }));
		GameEngine::addGameObject(new Asteroid(glm::vec3(160 - i * 4, 0 , -185), { 1,0,0 }, { 1.4,30,30 }));
		GameEngine::addGameObject(new Asteroid(glm::vec3(0,	0, -185 + i * 4), { 1,0,0 }, { 1.4,30,30 }));

	}
	for (int i = 0; i < 60; i++) // 2nd Square
	{
		
		GameEngine::addGameObject(new Asteroid(glm::vec3(-20 + i*4,0, -5.00), { 1,0,0 }, { 1.4, 30, 30 }));
		GameEngine::addGameObject(new Asteroid(glm::vec3(220, 0, -5.00 - i * 4), { 1,0,0 }, { 1.4, 30, 30 }));
		GameEngine::addGameObject(new Asteroid(glm::vec3(220 - i * 4, 0, -245), { 1,0,0 }, { 1.4,30,30 }));
		GameEngine::addGameObject(new Asteroid(glm::vec3(-20, 0, -245 + i * 4), { 1,0,0 }, { 1.4,30,30 }));
	}
	
	


	// Load Images
	//image[0] = getbmp();
	
	
		
		
		//add rotation thingy
//	GameEngine::drawRotation();
//		GameEngine::animate();
		//Start the game engine.
	//	GameEngine::skybox();
	GameEngine::addGameObject(new SpaceShip(glm::vec3(3, 0.0, -20.0)), true); //SpaceShip
		GameEngine::addGameObject(new Sun(glm::vec3(80, 0.0, -85.0), { 1,2,4 }, { 30,30,30 })); //Sun
		GameEngine::startEngine();

		return 0;
	}
