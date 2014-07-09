#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include "Category.hpp"
#include "CommandQueue.hpp"


class GameObject : public sf::Drawable, public sf::Transformable
{
public:
	enum class MoveDirection
	{
		Up,
		Right,
		Down,
		Left,
	};

public:
	GameObject();

	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			update(sf::Time dt);
	virtual void			updateSelf(sf::Time dt);

	virtual unsigned int	getCategory() const;

	void					onCommand(Command& command, sf::Time dt);

	void				setVelocity(sf::Vector2f vel);
	void				setVelocity(float x, float y);
	void				accelerate(sf::Vector2f velocity);
	void				accelerate(MoveDirection direction);
	void				enableFriction();
	void				disableFriction();
	void				enableGravity();
	void				disableGravity();

	sf::Vector2f		getVelocity() const;

private:

	static const double	FRICTION;
	static const double	GRAVITY;

	float				mAccelerationRate;
	float				mMaxSpeed;
	sf::Vector2f		mVelocity;
	bool				hasFriction;
	bool				hasGravity;
};