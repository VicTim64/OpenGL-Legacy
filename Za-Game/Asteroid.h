#pragma once
#include "GameObject.h"
# include <glm/glm.hpp> 
struct Colour {
	float r, g, b;
};
struct Scale {
	double x, y, z;
};


class Asteroid : public GameObject
{

	static int debugCounter;
	unsigned int texture3[1];
public:
	//////////////////////////////////
	/// Constructor that sets the position and colour.
	///
	/// @param[in] pos Position in scene in 3 axis
	/// @param[in] col Colour of the object in r,g,b
	//////////////////////////////////
	Asteroid(glm::vec3 pos, Colour col = { 0,0,1 }, Scale scale = { 1, 40, 40 });
	~Asteroid();///<Default destructor.

	Colour colour; // Colour for asteroids
	Scale scale;    // Resize the asteroids

				  /* Overridden from the GameObject class.*/
	unsigned int setupDrawing(unsigned int listBase);///<Overridden setup drawing method.
	void start();  ///Overridden start method.
	void update(int deltaTime);  ///<Overridden update method.
	void collides(Collider * other);///<Overridden collides method.
	void light();
	//void drawScene();
};
