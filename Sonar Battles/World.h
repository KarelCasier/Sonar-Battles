#pragma once
//#include <SFML\Graphics.hpp>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include <array>
#include <queue>

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
		Floor,
		Main,
		LayerCount
	};

private:
	sf::RenderWindow&					mWindow;
	sf::View							mWorldView;
	TextureHolder						mTextures;

};
