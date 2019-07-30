#include "Asteroid.h"
#include "CubeCollider.h"
#include "getbmp.h"

int Asteroid::debugCounter = 0;
//unsigned int texture[1];

Asteroid::Asteroid(glm::vec3 pos, Colour col, Scale scale) : GameObject(pos)
{
	this->colour = col;
	this->scale = scale;
	Asteroid::collider = new CubeCollider(&this->position, 2, 2, 2);

}

Asteroid::~Asteroid()
{
	//delete Asteroid::collider;
	Asteroid::collider = NULL;

	
}
void Asteroid::light()
{
	glPushMatrix();
	float specularRef[] = { 1.0,1.0,1.0 };
	float materialShine[] = { 20.00 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularRef);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShine);
	glPopMatrix();
}
unsigned int  Asteroid::setupDrawing(unsigned int listBase) {
	////// Light
//	glPushMatrix();
//	float specularRef[] = { 1.0,1.0,1.0 };
//	float materialShine[] = { 20.00 };
//	glMaterialfv(GL_FRONT, GL_SPECULAR, specularRef);
//	glMaterialfv(GL_FRONT, GL_SHININESS, materialShine);
//	glPopMatrix();
	//////// Light
	//glEnable(GL_DEPTH_TEST);
	this->base = listBase;
	glNewList(this->base, GL_COMPILE);
	glPushMatrix();

	GLUquadricObj *pQuadric = gluNewQuadric();
	gluQuadricDrawStyle(pQuadric, GLU_FILL);
	glBindTexture(GL_TEXTURE_2D, texture3[0]);
	gluQuadricTexture(pQuadric, true);
	gluQuadricNormals(pQuadric, GLU_SMOOTH);
	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //GL_REPLACE //GL_MODULATE			
																// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);
	//glutSolidSphere(scale.x, scale.y, scale.z); // Error (scale without textures)
	gluSphere(pQuadric, scale.x, scale.y, scale.z); // Resize the asteroids separately.
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f(colour.r, colour.g, colour.b);
	glTranslatef(0, 0, 0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();

	return this->base + 1;
}

void Asteroid::start() {

	// Local storage for bmp image data.
	BitMapFile *image[1];

	image[0] = getbmp("Textures/asteroid.bmp");

	// Bind image to texture object texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture3[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void Asteroid::update(int deltaTime) {
}

void Asteroid::collides(Collider* other) {
	if (debugMode) {
		//std::cout << "Asteroid collides, asteroid will be removed!" << std::endl;
	}
	this->active = true;
}
