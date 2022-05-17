#include "Entity.hpp"

#include <cassert>


Entity::Entity(int hitpoints)
: mVelocity()
, mHitpoints(hitpoints)
, mRotation()
{
	//mRotation = 0.f;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::setRotation2(float r) {
	mRotation = r;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}

void Entity::rotation(float r) {
	mRotation = r;
	setRotation(r);
}

float Entity::getRotation() {
	return mRotation;
}

int Entity::getHitpoints() const
{
	return mHitpoints;
}

void Entity::repair(int points)
{
	assert(points > 0);

	mHitpoints += points;
}

void Entity::damage(int points)
{
	//assert(points > 0);

	mHitpoints -= points;
}

void Entity::destroy()
{
	mHitpoints = 0;
}

bool Entity::isDestroyed() const
{
	return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{	
	move(mVelocity * dt.asSeconds());
}
