#pragma once


#include "GameObject.h"
#include <vector>

class GameEngine {
private:
	static std::vector<GameObject*> gameobjects; ///<Vector of (pointers to) game objects: all the objects in the game.
	static void updateGame(); ///<Function that updates the game state, by updating all game objects and checking for collision.
	static int oldTimeSinceStart; ///<The old time since the start of the game (from previous frame) for delta tikme calculation.
	static int newTimeSinceStart; ///<The time since the start of the game  for delta tikme calculation.
	static GameObject* cameraFollow; ///<Pointer to an object that the camera will follow.
	static bool debugMode;
	static unsigned int base; // Generate display list base. 
public:
	//////////////////////////////////
	/// Initialises the engine as well as initilising glut and OpenGL.
	///
	/// @param[in] argc Number of arguments pointed to by argv
	/// @param[in] **argv Argument vector (How command line arguements are passed to main())
	/// @param[in] windowTitle Title deplayed on the window created by glut. (Has a default value)
	/// @param[in] debugMode If debug mode is active for the game (Has a default value)
	/// @param[in] width Width in pixels for the created window (Has a default value)
	/// @param[in] height Height in pixels for the created window (Has a default value)
	//////////////////////////////////
	static void initEngine(int argc, char **argv, const char* windowTitle = "New Game", bool debugMode = false, int width = 500, int height = 500); ///<Initialises the engine, initilising glut and OpenGL.
	static void displayFunc();
	static void reshapeFunc(int w, int h);
	static void addGameObject(GameObject* gameobject, bool camFollow = false); ///<Adds a GameObject to the game.
	static void startEngine();///<Start the engine (and the main game loop).
	static void initialize();
   //static void drawRotation();
   // void animate(60);
	static void skybox();
	static void initsky();
	static void cleanup(); ///<Function to clean up and delete any pointers and anything else when the game exists to prevent memory leaks.
};