#pragma once
#include "GameObject.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class Submarine : public GameObject
{
public:
	Submarine(const TextureHolder& textures);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	sf::Sprite		mSprite;
};