#pragma once
#include "GameObject.h"
#include <cmath>
#include<glm/gtx/rotate_vector.hpp>
#define TURNING_SPEED 90.0 //<turning speed in degrees per second
#define MOVE_SPEED 40.0 //<move speed in coordinate units per second

class SpaceShip : public GameObject {
	//unsigned int texture[1]; 
private:
	glm::vec3 startingHeading = heading; ///<Starting facing direction of the spaceship.
	float rotationAngle, pitchAngle = 0.0; ///<Angle at which the ship is rotated.

public:
	//////////////////////////////////
	/// Constructor that sets the position.
	///
	/// @param[in] pos Position in scene in 3 axis
	//////////////////////////////////
	SpaceShip(glm::vec3 pos);
	~SpaceShip(); ///<default deconstructor

	unsigned int setupDrawing(unsigned int listBase); ///<Override setup method.
	void drawScene(); ///<Override drawscene method.
	void start();  ///<Override start method.
	void update(int deltaTime);  ///<Override update method.
	void collides(Collider * other); ///<Override method that reacts to a collision.
};
