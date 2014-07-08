#pragma once
//#include <SFML\Graphics.hpp>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include <array>

#include "TileMap.hpp"

// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World
	: private sf::NonCopyable
{
public:
	explicit		World(sf::RenderWindow& window);
	void			update(sf::Time dt);
	void			draw();


private:
	void			loadTextures();
	void			buildScene();

private:
	enum Layer
	{
		Main,
		LayerCount
	};

	enum TileState
	{
		BLANK,		// 0
		TOPLEFT,	// 1
		TOPRIGHT,	// 2
		BOTTOMRIGHT,// 3
		BOTTOMLEFT, // 4
		HORIZONTAL,	// 5
		VERTICAL,	// 6
	};

private:
	sf::RenderWindow&					mWindow;
	sf::View							mWorldView;
	TextureHolder						mTextures;

	TileMap								mTileMap;
};
