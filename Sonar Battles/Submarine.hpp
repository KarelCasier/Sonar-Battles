#pragma once
#include "GameObject.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

class Submarine : public GameObject
{
public:
	Submarine(const TextureHolder& textures);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	sf::Sprite		mSprite;
};