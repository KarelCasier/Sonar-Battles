#include "GameObject.hpp"

#include <math.h>

const double GameObject::FRICTION = .98;
const double GameObject::GRAVITY = 200;

GameObject::GameObject()
: mVelocity(0.f, 0.f)
, mAccelerationRate(3)
, mMaxSpeed(200)
, hasFriction(true)
, hasGravity(false)
{

}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Handled by derived classes
}

void GameObject::update(sf::Time dt)
{
	if (hasFriction)
	{
		mVelocity.x *= FRICTION;
		mVelocity.y *= FRICTION;
	}
	if (hasGravity)
	{
		mVelocity.y += GRAVITY * dt.asSeconds();
	}
	move(mVelocity * dt.asSeconds());

	updateSelf(dt);
}

void GameObject::updateSelf(sf::Time dt)
{
	//Overridden in derived class
}

void GameObject::setVelocity(sf::Vector2f vel)
{
	mVelocity = vel;
}

void GameObject::setVelocity(float x, float y)
{
	mVelocity.x = x;
	mVelocity.y = y;
}

void GameObject::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void GameObject::accelerate(MoveDirection direction)
{
	if (sqrt(pow(mVelocity.x, 2) + pow(mVelocity.y, 2)) <= mMaxSpeed)
	{
		// For rotation base movement
		//mVelocity.x += mAccelerationRate * (cos((this->getRotation() + 90) * 3.14159 / 180));
		//mVelocity.y += mAccelerationRate * (sin((this->getRotation() + 90) * 3.14159 / 180));

		switch (direction)
		{
		case GameObject::MoveDirection::Up:
			mVelocity.y -= mAccelerationRate;
			break;
		case GameObject::MoveDirection::Right:
			mVelocity.x += mAccelerationRate;
			break;
		case GameObject::MoveDirection::Down:
			mVelocity.y += mAccelerationRate;
			break;
		case GameObject::MoveDirection::Left:
			mVelocity.x -= mAccelerationRate;
			break;
		default:
			break;
		}
	}
}

sf::Vector2f GameObject::getVelocity() const
{
	return mVelocity;
}

unsigned int GameObject::getCategory() const
{
	return Category::None;
}

void GameObject::onCommand(Command& command, sf::Time dt)
{
	if (command.category & this->getCategory())
	{
		command.action(*this, dt);
	}
}

void GameObject::enableFriction()
{
	hasFriction = true;
}

void GameObject::disableFriction()
{
	hasFriction = false;
}

void GameObject::enableGravity()
{
	hasGravity = true;
}

void GameObject::disableGravity()
{
	hasGravity = false;
}
