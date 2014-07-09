#pragma once
#include "GameObject.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class Submarine : public GameObject
{
public:
	enum class TypeID
	{
		Player,
		Enemy,
	};

							Submarine(const TextureHolder& textures, TypeID type);
	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			updateSelf(sf::Time dt);
	virtual unsigned int	getCategory() const;

private:

	const sf::Texture&			getSpriteTexture(TypeID type);
	const sf::Texture&			getShadowTexture(TypeID type);

private:

	sf::Sprite									mSprite;
	const TextureHolder&						mTextures;
	TypeID										mType;
};