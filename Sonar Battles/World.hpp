#pragma once
//#include <SFML\Graphics.hpp>
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include <array>
#include <memory>

#include "CommandQueue.hpp"
#include "TileMap.hpp"
#include "GameObject.hpp"
#include "Submarine.hpp"

// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	typedef std::unique_ptr<GameObject> Ptr;

public:
	explicit		World(sf::RenderWindow& window);
	void			update(sf::Time dt);
	void			draw();

	CommandQueue&	getCommandQueue();

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

	CommandQueue						mCommandQueue;

	TileMap								mTileMap;

	std::vector<sf::Vector2f>			mSpawnLocations;

	std::vector<Ptr>					mGameObjects;

	Submarine*							mPlayerSub;
};
