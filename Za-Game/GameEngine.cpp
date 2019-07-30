
#include "GameEngine.h"
#include "getbmp.h"

static unsigned int texture[6];
BitMapFile *image[6];

bool GameEngine::debugMode;
std::vector<GameObject*> GameEngine::gameobjects; //The definition of static variables needs to be repeated in the cpp file.
GameObject* GameEngine::cameraFollow;
int GameEngine::oldTimeSinceStart;
int GameEngine::newTimeSinceStart;
std::map <int, bool> GameObject::specialKeys;
std::map <char, bool> GameObject::keys;
unsigned int GameEngine::base; // Generate display list base. 
							   ///
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

bool cam1;
bool cam2;

/*								  ///
void animate(int value)
{
	if (isAnimate)
	{
		latAngle += 5.0;
		if (latAngle > 360.0) latAngle -= 360.0;
		longAngle += 1.0;
		if (longAngle > 360.0) longAngle -= 360.0;

		glutPostRedisplay();
		glutTimerFunc(animationPeriod, animate, 1);
	}
}*/
/*
void drawRotation(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -25.0);

	// Rotate scene.
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	// Fixed torus.
	glColor3f(0.0, 1.0, 0.0);
	glutWireTorus(2.0, 12.0, 20, 20);

	// Begin revolving ball.
	glRotatef(longAngle, 0.0, 0.0, 1.0);

	glTranslatef(12.0, 0.0, 0.0);
	glRotatef(latAngle, 0.0, 1.0, 0.0);
	glTranslatef(-12.0, 0.0, 0.0);

	glTranslatef(20.0, 0.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glutWireSphere(2.0, 10, 10);
	// End revolving ball.

	glutSwapBuffers();
}
///
*/
//Light
void GameEngine::initialize() //Setting up the light in game engine
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GLfloat violet[] = { 0.1, 0.1, 0.9, 1.0 }; 
	GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };          
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, violet);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);

	// Set lighting intensity and color
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };  
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Set the light position
	GLfloat position[] = { 30, 10, -85.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position); 

}
void GameEngine::initEngine(int argc, char **argv, const char* windowTitle, bool debug, int width, int height) {
	GameEngine::debugMode = debug;
	GameObject::debugMode = GameEngine::debugMode;
	//Init glut.
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowTitle);

	glewExperimental = GL_TRUE;
	glewInit();

	GameEngine::initsky();

	//Lambda function defined inside the glutDisplayFunc call to call all relevant drawscenes.
	glutDisplayFunc(displayFunc);
	

	//Lambda function defined inside glutReshapeFunc to do resize logic.
	glutReshapeFunc(reshapeFunc);
	
	//Lambda functions to link our code to glut's keydown andkeyup. Our function deals with both regular and special keys in one.
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = true;
		//if we press escape, exit the game
		if (key == 27) {
			cleanup();
			exit(0);
		}
		if (key == 'v')
		{
			cam1 = true;
			cam2 = false;
		}
		if (key == 'b')
		{
			cam2 = true;
			cam1 = false;
		}
	});

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = false;
	});

	glutSpecialFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = true;
	});

	glutSpecialUpFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = false;
	});

	glutIdleFunc(updateGame);
	
	
}

void GameEngine::displayFunc() {
	//glEnable(GL_DEPTH_TEST);
	//Prepare for drawing all objects.1
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//cout << gameobjects.size() << endl;
	//If we have no object to follow just put the cam in a static position.
	if (cameraFollow != NULL) {
		//Hardcoded camera variables for the distance bewteen camera and object and y axis distance between camera and object.
		float distance = 8;
		float yAm = 2;
		
		if (cam1 == true)
		{
			gluLookAt(cameraFollow->position.x - (cameraFollow->heading.x * distance), cameraFollow->position.y - (cameraFollow->heading.y * distance) + yAm, cameraFollow->position.z - (cameraFollow->heading.z * distance),
				cameraFollow->position.x, cameraFollow->position.y, cameraFollow->position.z,
				0.0, 1.0, 0.0);
		}

		if (cam2 == true)
		{
			gluLookAt(cameraFollow->position.x - (cameraFollow->heading.x * distance), 40, cameraFollow->position.z - (cameraFollow->heading.z * distance),
				cameraFollow->position.x, cameraFollow->position.y, cameraFollow->position.z,
				0.0, 1.0, 0.0);
		}

		//if (cam3 == true)
		//{
		//	gluLookAt(cameraFollow->position.x - (cameraFollow->heading.x * distance), cameraFollow->position.y - (cameraFollow->heading.y * distance) + yAm, cameraFollow->position.z - (cameraFollow->heading.z * distance),
		//		cameraFollow->position.x, cameraFollow->position.y, cameraFollow->position.z,
		//		0.0, 1.0, 0.0);
		//}
	}
	else
	{
		float distance = 8;
		float yAm = 2;
		gluLookAt(cameraFollow->position.x - (cameraFollow->heading.x * distance), cameraFollow->position.y - (cameraFollow->heading.y * distance) + yAm, cameraFollow->position.z - (cameraFollow->heading.z * distance),
			cameraFollow->position.x, cameraFollow->position.y, cameraFollow->position.z,
			0.0, 1.0, 0.0);
	}


	
	glEnable(GL_TEXTURE_2D);
	GameEngine::skybox();
	glDisable(GL_TEXTURE_2D);	//Call drawscene for all gameobjects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->drawScene();
	//	
	}
//	
	glutSwapBuffers(); //Finish Draw Scene.
	
}

void GameEngine::reshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, (float)w / (float)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
};

void GameEngine::startEngine() {
	//Explain main interaction with scene.
	std::cout << "Press escape to exit." << std::endl;
	
	
	//Start game loop.
	glutMainLoop();
}

//Add a new gameobject to the scene.
void GameEngine::addGameObject(GameObject* gameobject, bool camFollow) {
	
	
	
	gameobjects.push_back(gameobject);
	if (camFollow) {
		cameraFollow = gameobject;
	}

	base = glGenLists(1);
	base = gameobject->setupDrawing(base);
	gameobject->start();


}

//Update the game state.
void GameEngine::updateGame() 
{
	//Calculate delta time (in mili seconds).
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = newTimeSinceStart - oldTimeSinceStart;

	//If the last frame was rendered less than 1 ms ago, the detalaTime will be 0 ms. This causes problems in calculations, so sleep for 1ms to adjust.
	if (deltaTime == 0) {
		Sleep(1);
		newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = newTimeSinceStart - oldTimeSinceStart;
	}

	//Remove any inactive objects first.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		if (!gameobjects[i]->active) {
			delete gameobjects[i];
			gameobjects.erase(gameobjects.begin() + i);
		}
	}

	//Run update for all game objects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->update(deltaTime);
	}

	//Test collisions in a naive way for every pair of gameobjects (note calculations will explode when you increase the amount of gameobjects. Can you think of a better way?
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		for (std::vector<GameObject*>::size_type j = i + 1; j != gameobjects.size(); j++) {
			if (gameobjects[i]->collider != NULL) {
				if (gameobjects[i]->collider->collidesWith(gameobjects[j]->collider)) {
					gameobjects[i]->collides(gameobjects[j]->collider);
					gameobjects[j]->collides(gameobjects[i]->collider);
				}
			}
		}
	}

	//Done so indicate that it's time to redraw.
	glutPostRedisplay();
}



//SkyBox 
// longAngle -=1.0
// if(longAngle<-300.0) {longAngle-=360.0:} // rotate planets around star
// glutPostRedisplay();
void GameEngine::initsky()
{
	// Load IMG. for SkyBox
	image[0] = getbmp("Textures/space.bmp");
	image[1] = getbmp("Textures/space.bmp");
	image[2] = getbmp("Textures/space.bmp");
	image[3] = getbmp("Textures/space.bmp");
	image[4] = getbmp("Textures/space.bmp");
	image[5] = getbmp("Textures/space.bmp");

	glGenTextures(6, texture);
	

	// Bind  image to texture object texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[5]->sizeX, image[5]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[5]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
void GameEngine::skybox()
{
glEnable(GL_TEXTURE_2D);

// Specify how texture values combine with current surface color values.
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //GL_REPLACE

glPushMatrix();

glScalef(4.0, 4.0, 4.0);

// Bind  image to texture object texture[0]. 
/// planes

glDisable(GL_CULL_FACE);

glBindTexture(GL_TEXTURE_2D, texture[0]);
glEnable(GL_TEXTURE_2D);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -100, -100.0);
glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -100, -100.0);
glTexCoord2f(1.0, 1.0); glVertex3f(100.0, -100, 100.0);
glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, -100, 100.0);
glEnd();
glDisable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D, texture[1]);
glEnable(GL_TEXTURE_2D);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -100, 100.0);
glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -100, 100.0);
glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 100, 100.0);
glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 100, 100.0);
glEnd();
glDisable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D, texture[2]);
glEnable(GL_TEXTURE_2D);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -100, -100.0);
glTexCoord2f(1.0, 0.0); glVertex3f(-100.0, -100, 100.0);
glTexCoord2f(1.0, 1.0); glVertex3f(-100.0, 100, 100.0);
glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 100, -100.0);
glEnd();
glDisable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D, texture[3]);
glEnable(GL_TEXTURE_2D);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 100, -100.0);
glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 100, -100.0);
glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 100, 100.0);
glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 100, 100.0);
glEnd();
glDisable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D, texture[4]);
glEnable(GL_TEXTURE_2D);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -100, -100.0);
glTexCoord2f(1.0, 0.0); glVertex3f(-100.0, 100, -100.0);
glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 100, -100.0);
glTexCoord2f(0.0, 1.0); glVertex3f(100.0, -100, -100.0);
glEnd();
glDisable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D, texture[5]);
glEnable(GL_TEXTURE_2D);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(100.0, -100, -100.0);
glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 100, -100.0);
glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 100, 100.0);
glTexCoord2f(0.0, 1.0); glVertex3f(100.0, -100, 100.0);
glEnd();
glDisable(GL_TEXTURE_2D);



glPopMatrix();
//glDisable(GL_CULL_FACE);
//glDisable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
} 

//Here we delete pointer objects for when we exit, to prevent memory leaks.
void GameEngine::cleanup()
{
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it) {
		delete *it;
	}
	gameobjects.clear();
}