#include "Submarine.hpp"

#include "Utility.hpp"

const sf::Texture& Submarine::getSpriteTexture(TypeID type)
{
	switch (type)
	{
	case TypeID::Player:
		return mTextures.get(TextureID::Submarine);
	case TypeID::Enemy:
		return mTextures.get(TextureID::Submarine);
	}
}

Submarine::Submarine(const TextureHolder& textures, TypeID type)
: mTextures(textures)
, mType(type)
{
	mSprite.setTexture(getSpriteTexture(type));
	centerOrigin(mSprite);

}

void Submarine::updateSelf(sf::Time dt)
{

}

void Submarine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mSprite, states);
}

unsigned int Submarine::getCategory() const
{
	switch (mType)
	{
	case Submarine::TypeID::Player:
		return Category::PlayerSub;
	case Submarine::TypeID::Enemy:
		return Category::EnemySub;
	default:
		break;
	}
}