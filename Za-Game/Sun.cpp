#include "Sun.h"
#include "CubeCollider.h"
#include "getbmp.h"

//unsigned int texture[1];


Sun::Sun(glm::vec3 pos, Coloured col, Scaling scale) : GameObject(pos)
{
	this->coloured = col;
	this->scaling = scale;
	Sun::collider = new CubeCollider(&this->position, 2, 2, 2);

}

Sun::~Sun()
{
	//delete Sun::collider;
	Sun::collider = NULL;


}

void Sun::light()
{
	glPushMatrix();
	float specularRef[] = { 1.0,1.0,1.0 };
	float materialShine[] = { 20.00 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularRef);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShine);
	glPopMatrix();
}
unsigned int  Sun::setupDrawing(unsigned int listBase) {
	
	this->base = listBase;
	glNewList(this->base, GL_COMPILE);
	glPushMatrix();

	GLUquadricObj *pQuadric = gluNewQuadric();
	gluQuadricDrawStyle(pQuadric, GLU_FILL);
	glBindTexture(GL_TEXTURE_2D, texture2[0]);
	gluQuadricTexture(pQuadric, true);
	gluQuadricNormals(pQuadric, GLU_SMOOTH);
	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //GL_REPLACE //GL_MODULATE			
																// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);
	//glutSolidSphere(scale.x, scale.y, scale.z); // Error (scale without textures)
	gluSphere(pQuadric, scaling.x, scaling.y, scaling.z); // Resize the asteroids separately.
	//glDisable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f(coloured.r, coloured.g, coloured.b);
	glTranslatef(0, 0, 0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();

	return this->base + 1;
}

void Sun::start() {

	// Local storage for bmp image data.
	BitMapFile *img[1];

	img[0] = getbmp("Textures/sun.bmp");

	// Bind image to texture object texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture2[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[0]->sizeX, img[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, img[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Sun::update(int deltaTime)
{
}

void Sun::collides(Collider* other) {
	if (debugMode) {
		//std::cout << "Sun collides, asteroid will be removed!" << std::endl;
	}
	this->active = true;
}
