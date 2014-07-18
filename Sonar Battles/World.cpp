#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>

#include "iostream"

bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
	float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = p1_x - p0_x;
	s10_y = p1_y - p0_y;
	s32_x = p3_x - p2_x;
	s32_y = p3_y - p2_y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
		return false; // Collinear
	bool denomPositive = denom > 0;

	s02_x = p0_x - p2_x;
	s02_y = p0_y - p2_y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
		return false; // No collision

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
		return false; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
		return false; // No collision
	// Collision detected
	t = t_numer / denom;
	if (i_x != NULL)
		*i_x = p0_x + (t * s10_x);
	if (i_y != NULL)
		*i_y = p0_y + (t * s10_y);

	return true;
}


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
	sf::Vector2f playerPosBefore = mPlayerSub->getPosition();

	//Handle player commands
	while (!mCommandQueue.isEmpty())
	{
		Command command = mCommandQueue.pop();
		for (const Ptr& object : mGameObjects)
		{
			object->onCommand(command, dt);
		}

	}

	// Apply movements
	for (const Ptr& object : mGameObjects)
	{
		object->update(dt);
	}

	sf::Vector2f playerPosAfter = mPlayerSub->getPosition();

	//Collision
	sf::Vector2f subPos = mPlayerSub->getPosition();
	sf::Vector2u subTilePos(floor(subPos.x / mTileMap.getTileSize()), floor(subPos.y / mTileMap.getTileSize()));
	
	// Get tile that sub is on
	int tileNumber = subTilePos.x + subTilePos.y * mTileMap.getGridSize().x;
	//std::cout << "Tile collision: " << tileNumber << std::endl;

	//Create line object for that tile
	sf::Vector2f ptA, ptB;

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
			break;
		case 2:
			// Top Left - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize();
			ptA.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			ptB.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptB.y = subTilePos.y * mTileMap.getTileSize();
			break;
		case 3:
			// Top Right - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptA.y = subTilePos.y * mTileMap.getTileSize();

			ptB.x = subTilePos.x * mTileMap.getTileSize() + mTileMap.getTileSize();
			ptB.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			break;
		case 4:
			// Bottom Right - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptA.y = subTilePos.y * mTileMap.getTileSize() + mTileMap.getTileSize();

			ptB.x = subTilePos.x * mTileMap.getTileSize() + mTileMap.getTileSize();
			ptB.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			break;
		case 5:
			// Bottom Left - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize();
			ptA.y = subTilePos.y * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);

			ptB.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptB.y = subTilePos.y * mTileMap.getTileSize() + mTileMap.getTileSize();
			break;
		case 6:
			//Vertical - Check
			ptA.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptA.y = subTilePos.y * mTileMap.getTileSize();

			ptB.x = subTilePos.x * mTileMap.getTileSize() + (mTileMap.getTileSize() / 2);
			ptB.y = subTilePos.y * mTileMap.getTileSize() + mTileMap.getTileSize();
			break;
		default:
			break;
		}
	}

	sf::Vector2f newPosition;
	//Get current movement vector
	if (get_line_intersection(ptA.x, ptA.y, ptB.x, ptB.y, playerPosBefore.x, playerPosBefore.y, playerPosAfter.x, playerPosAfter.y, &newPosition.x, &newPosition.y))
	{
		std::cout << "Collision" << std::endl;
		mPlayerSub->setPosition(newPosition);
		mPlayerSub->setVelocity(0, 0);
	}

	//std::cout << "( " <<  << ", " <<  << ")" << std::endl;
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mTileMap);
	for (const Ptr& object : mGameObjects)
	{
		mWindow.draw(*object);
	}

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