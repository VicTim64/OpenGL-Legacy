#include "SphereCollider.h"
#include <iostream>
#include <glm/geometric.hpp>
using namespace std;

SphereCollider::SphereCollider(glm::vec3*centre, float radius)
{
	this->colliderCentre = centre;
	this->radius = radius;
}
float SphereCollider::Radius()
{
	return this->radius;
}
float SphereCollider::minX()
{
	return this->colliderCentre->x - this->radius;
}
float SphereCollider::maxX()
{
	return this-> colliderCentre->x + this->radius;
}
float SphereCollider::minY()
{
	return this->colliderCentre->y - this->radius;
}
float SphereCollider::maxY()
{
	return this->colliderCentre->y + this->radius;
}
float SphereCollider::minZ()
{
	return this->colliderCentre->z - this->radius;
}
float SphereCollider::maxZ()
{
	return this->colliderCentre->z - this->radius;
}
bool SphereCollider::collidesWith(Collider* other)



SphereCollider::~SphereCollider()
{
}
