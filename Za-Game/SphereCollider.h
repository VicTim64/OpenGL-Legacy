#pragma once
#include "Collider.h"
#ifdef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

class SphereCollider :public Collider
{
private:
public:
	float radius = 0.0;
	SphereCollider(glm:vec3*centre, float radius);
	~Sphere

};



