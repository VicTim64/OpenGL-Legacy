#include "SpaceShip.h"
#include "CubeCollider.h"
#include "GameEngine.h"
float acc = 0;


SpaceShip::SpaceShip(glm::vec3 pos) :GameObject(pos) {
	SpaceShip::collider = new CubeCollider(&this->position, 1.5, 1.5, 1.5);
}

SpaceShip::~SpaceShip() {
	//delete SpaceShip::collider;
	SpaceShip::collider = NULL;
}

unsigned int SpaceShip::setupDrawing(unsigned int listBase) {
	
	this->base = listBase;
	// Draw ship.
	glNewList(this->base, GL_COMPILE);
	glPushMatrix();
	////// Light

	float specularRef[] = { 1.0,1.0,1.0 };
	float materialShine[] = { 10.00 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularRef);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShine);
	glEnable(GL_COLOR_MATERIAL);
	
	//////// Light

	//glColor3f(0, 1, 1);
	//glTranslatef(0, 0, 0);
	//glRotatef(180, 0, 1, 0);
	//glColor3f(1.0, 0.0, 1.0);
	//glutSolidSphere(1.4, 30, 30);
	//glEnd();
	//glColor3f(0, 1, 1);
	glTranslatef(0, 0, 0);
	glRotatef(180, 0, 1,0);
	glutSolidCone(1.5, 2.2, 30, 30);
	glTranslatef(0, 0, +2.7);
	glutSolidSphere(1, 30, 30);
	

//	glEnd();
	

	glPopMatrix();

	////////////////////////////////////////////


	glEndList();

	return this->base + 1;
	
	
	this->base = listBase;
	
	// Draw ship.
	/*
	glNewList(this->base, GL_COMPILE);

	glPushMatrix();
	// to enable the lights on the spaceship, more specific specular lighting

	float SpecReflection1[] = { 0.8f,0.8f,0.8f, 1.0f };
	float MatShine1[] = { 200.00 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecReflection1);
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShine1);
	glEnable(GL_COLOR_MATERIAL);

	
	
	glNewList(this->base, GL_COMPILE);
	glPushMatrix();
	//glColor3f(1, 0, 0);
	//glTranslatef(0, 0, 0);
	//glRotatef(180, 1, 0, 0);
	//glutSolidCone(0.5, 0.15, 30, 30);

	glColor3f(0, 1, 1);
	
	glTranslatef(0, 0, 0);
	glRotatef(180, 0, 1, 0);
	//glutSolidTorus(1, -1, 10, 10);
	glutSolidCone(1,1.5, 20, 20);
	glTranslatef(1, 0, 0);//Body
	//glTranslatef(1, 0, 0);//Body
	//glRotate3f(0, 1, 0, 0);
	//glutSolidCone(1, 5, 20, 20);
	
//	glutSolidCube(0.5); // Draw a solid-shaded cube, centered at origin.
	//glutSolidSphere() // Draw a shaded sphere, centered at origin.
	


										 //glColor3f(1, 0, 0);
	//glTranslatef(0, 0, 0);
	//glRotatef(180, 1, 0, 0);             //Head
	//glutSolidSphere(1.5, 2, 2);
	glPopMatrix();
	
	//glMatrixMode(GL_MODELVIEW);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glLoadIdentity();
	//glTranslatef(0.0,0.0,-5.0);
	//glColor3f(0.0, 1.0, 0.0);
	//glRotatef(0, 1.0, 0.0, 0.0);
	//glScalef(1.0, 1.0, 1.0);
	//glutSolidTorus(1, 1, 1, 1);
	//glutSolidTorus(0.5, 1, 2, 1);
	/*
	glBegin(GL_POLYGON);
	glColor3f(0.2, 0.5, 0.2);
	glVertex3f(1, 1, 1);
	glVertex3f(0.4, 0.2, 0.8);
	glVertex3f(0.3, 0.5, 0.4);
	glVertex3f(0.2, 0.1, 0.2);
	*/
	//glColor3f(1, 0, 0);
	//glTranslatef(1, 0, 0);
	//glRotatef(90, 1, 0, 0);
	//glutWireSphere(0.5, 0.15, 30);
	//
	//glColor3f(1.0,0.0,0.0);
	//glutWireSphere(0.5, 5, 5);

	//
//	glPopMatrix();
	/*
	glEndList();
	
	return this->base + 1;
	*/
}

void SpaceShip::drawScene() {
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);

	//rotate openGL object
	glRotatef(pitchAngle, 0.0, 0.0, 1.0);
	glRotatef(rotationAngle, 0.0, 1.0, 0.0);
	glCallList(this->base); // Draw SpaceShip.
	glPopMatrix(); // End draw SpaceShip.

				   //if we are in debug mode then:
	if (debugMode) {
		//display heading vector
		glPushMatrix();
		glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(position.x, position.y, position.z);
		glVertex3f(position.x + heading.x, position.y + heading.y, position.z + heading.z);
		glEnd();
		glPopMatrix();
		//display the collider bounding box
		this->collider->Draw();
	}
}

void SpaceShip::start() {
}

void SpaceShip::update(int deltaTime) {
	float moveStep = MOVE_SPEED * (deltaTime / 1000.0); //movement speed in units per second * deltaTime in sec = moveStep
	float turningSpeed = TURNING_SPEED *  (deltaTime / 1000.0); //turning speed (degrees/sec) * deltaTime in sec = turning speed over delta time

	this->position -= this->heading * moveStep * acc;

	if (specialKeys[GLUT_KEY_DOWN]) {
		acc += 0.008;
	}
	if (specialKeys[GLUT_KEY_PAGE_UP]) {
		this->pitchAngle += turningSpeed; //in degrees not radians
	}
	if (specialKeys[GLUT_KEY_PAGE_DOWN]) {
		this->pitchAngle -= turningSpeed; //in degrees not radians
	}

	if (specialKeys[GLUT_KEY_LEFT]) {
		this->rotationAngle += turningSpeed; //in degrees not radians
	}
	if (specialKeys[GLUT_KEY_UP]) {
		acc -= 0.008;
	}
	if (specialKeys[GLUT_KEY_RIGHT]) {
		this->rotationAngle -= turningSpeed; //in degrees not radians
	}
	if (keys[' '])
	{
		acc = 0;
	};
	//test = 0

	//rotate the heading
	//In the drawscene we will need to rotate by the full rotation since the start
	//Therfore we start heard from the original startingheading, so that we can use the same variables for the rotations
	this->heading = glm::rotate(this->startingHeading, glm::radians(rotationAngle), glm::vec3(0.0, 1.0, 0.0));
	this->heading = glm::rotate(this->heading, glm::radians(pitchAngle), glm::vec3(0.0, 0.0, 1.0));
}


void SpaceShip::collides(Collider* other) {
	if (debugMode) {
		std::cout << "SpaceShip collides!" << std::endl;
	
	}
	acc = -acc;
}