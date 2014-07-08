#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class GameObject : public sf::Drawable, public sf::Transformable
{
public:
	GameObject();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	
};