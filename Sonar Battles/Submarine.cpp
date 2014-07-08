#include "Submarine.hpp"

#include "Utility.hpp"

Submarine::Submarine(const TextureHolder& textures)
: mSprite(textures.get(TextureID::Submarine))
{
	centerOrigin(mSprite);
}

void Submarine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mSprite, states);
}