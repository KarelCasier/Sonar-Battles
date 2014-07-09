#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>

#include "iostream"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
//, mTileStateArray()
{
	loadTextures();
	buildScene();

	// Prepare the view
	// Move view to center over vertices
	mWorldView.setCenter(mTileMap.getCenter());
}

void World::update(sf::Time dt)
{
	//Handle player commands
	while (!mCommandQueue.isEmpty())
	{
		Command command = mCommandQueue.pop();
		for (const Ptr& object : mGameObjects)
		{
			object->onCommand(command, dt);
		}

	}

	//Collision
	sf::Vector2f subPos = mPlayerSub->getPosition();
	sf::Vector2u subTilePos(floor(subPos.x / mTileMap.getTileSize()), floor(subPos.y / mTileMap.getTileSize()));
	
	// Get tile that sub is on
	int tileNumber = subTilePos.x + subTilePos.y * mTileMap.getGridSize().x;
	//std::cout << "Tile collision: " << tileNumber << std::endl;

	//Create line object for that tile
	sf::Vector2f ptA, ptB;
	line.setSize(sf::Vector2f(2, mTileMap.getTileSize()));
	line.setFillColor(sf::Color::Red);

	if (tileNumber > 0 && tileNumber <= (mTileMap.getGridSize().x * mTileMap.getGridSize().y))
	{
		//std::cout << mTileStateArray[tileNumber] << std::endl;
		switch (mTileStateArray[tileNumber])
		{
		case 1:
			// Horizontal - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize();
			ptA.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			ptB.x = subTilePos.x * mTileMap.getTileSize() + mTileMap.getTileSize();
			ptB.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			line.setRotation(-90);
			break;
		case 2:
			// Top Left - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize();
			ptA.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			ptB.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptB.y = subTilePos.y * mTileMap.getTileSize();

			line.setSize(sf::Vector2f(2, mTileMap.getTileSize() / 1.5));
			line.setRotation(-135);
			break;
		case 3:
			// Top Right - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptA.y = subTilePos.y * mTileMap.getTileSize();

			ptB.x = subTilePos.x * mTileMap.getTileSize() + mTileMap.getTileSize();
			ptB.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			line.setSize(sf::Vector2f(2, mTileMap.getTileSize() / 1.5));
			line.setRotation(-45);
			break;
		case 4:
			// Bottom Right - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptA.y = subTilePos.y * mTileMap.getTileSize() + mTileMap.getTileSize();

			ptB.x = subTilePos.x * mTileMap.getTileSize() + mTileMap.getTileSize();
			ptB.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			line.setSize(sf::Vector2f(2, mTileMap.getTileSize() / 1.5));
			line.setRotation(-135);
			break;
		case 5:
			// Bottom Left - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize();
			ptA.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			ptB.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptB.y = subTilePos.y * mTileMap.getTileSize() + mTileMap.getTileSize();

			line.setSize(sf::Vector2f(2, mTileMap.getTileSize() / 1.5));
			line.setRotation(-45);
			break;
		case 6:
			//Vertical - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptA.y = subTilePos.y * mTileMap.getTileSize();

			ptB.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptB.y = subTilePos.x * mTileMap.getTileSize() + mTileMap.getTileSize();

			line.setRotation(0);
			break;
		default:
			break;
		}
		line.setPosition(ptA);
	}

	//std::cout << "( " <<  << ", " <<  << ")" << std::endl;

	// Apply movements
	for (const Ptr& object : mGameObjects)
	{
		object->update(dt);
	}
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mTileMap);
	for (const Ptr& object : mGameObjects)
	{
		mWindow.draw(*object);
	}
	
	mWindow.draw(line);

}


void World::loadTextures()
{
	// Tile map
	mTextures.load(TextureID::TileMap, "assets/MapTileSet.png");

	//Submarine
	mTextures.load(TextureID::Submarine, "assets/Submarine.png");

}

void World::buildScene()
{

	// Build Level - ToDo - Allow import from files
	mTileStateArray =
	{ {

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 4, 1, 1, 1, 1, 5, 0, 4, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 6, 0, 0, 0, 0, 3, 1, 2, 0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 3, 1, 5, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 5, 0, 0, 0,
		0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0,
		0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 4, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0,
		0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,
		0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0,
		0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0,
		0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
		0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
		0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0,
		0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0,
		0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 4, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,
		0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0,
		0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 5, 6, 0, 0,
		0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 2, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 3, 2, 0, 0,
		0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 5, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0,
		0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 3, 1, 1, 5, 0, 0, 6, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0,
		0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 2, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 0, 0, 0,
		0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
		0, 4, 2, 0, 0, 0, 0, 0, 4, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 2, 0, 0, 0, 0, 0,
		0, 6, 0, 0, 0, 4, 1, 1, 2, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0,
		0, 6, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 3, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	} };

	mTileMap.load<1800>(mTextures.get(TextureID::TileMap), 20, mTileStateArray, 60, 30);

	mSpawnLocations.push_back(sf::Vector2f(100.f, 300.f));

	// Build Subs
	std::unique_ptr<Submarine> playerSub(new Submarine(mTextures, Submarine::TypeID::Player));
	mPlayerSub = playerSub.get();
	mPlayerSub->setPosition(mSpawnLocations.back());
	mSpawnLocations.pop_back();
	mGameObjects.push_back(std::move(playerSub));
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}