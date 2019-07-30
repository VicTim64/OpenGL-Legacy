#include "GameObject.h"

bool GameObject::debugMode;
std::map <int, bool> specialKeys;
std::map <char, bool> keys;


GameObject::GameObject(glm::vec3 pos) {
	this->position = pos;
}


GameObject::~GameObject() {
}

void GameObject::drawScene() {
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	glCallList(this->base);
	glPopMatrix();

	//If we are in debug mode then:
	if (debugMode) {
		//Display the collider bounding box.
		this->collider->Draw();
	}
}


void GameObject::collides(Collider* other) {
}