#pragma once
#include "GameObject.h"
# include <glm/glm.hpp> 
struct Coloured {
	float r, g, b;
};
struct Scaling {
	double x, y, z;
};


class Sun : public GameObject
{

	
	unsigned int texture2[1];
public:
	//////////////////////////////////
	/// Constructor that sets the position and colour.
	///
	/// @param[in] pos Position in scene in 3 axis
	/// @param[in] col Colour of the object in r,g,b
	//////////////////////////////////
	Sun(glm::vec3 position, Coloured colour = { 0,0,1 }, Scaling scales = { 1, 40, 40 });
	~Sun();///<Default destructor.

	Coloured coloured; // Colour for asteroids
	Scaling scaling;    // Resize the asteroids

					/* Overridden from the GameObject class.*/
	unsigned int setupDrawing(unsigned int listBase);///<Overridden setup drawing method.
	void start();  ///Overridden start method.
	void update(int deltaTime);  ///<Overridden update method.
	void collides(Collider * other);///<Overridden collides method.
	void light();
									//void drawScene();
};
