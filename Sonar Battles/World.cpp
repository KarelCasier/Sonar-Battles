#include "World.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>

#include "iostream"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
{
	loadTextures();
	buildScene();

	// Prepare the view
	//mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	//Center view on player
	//mWorldView.setCenter(mPlayerMech->getWorldPosition());

	// Apply movements
	//mSceneGraph.update(dt);
}

void World::draw()
{
	mWindow.setView(mWorldView);
	//mWindow.draw(mSceneGraph);
}


void World::loadTextures()
{
	//Player
	//mTextures.load(TextureID::PlayerMech, "assets/textures/PlayerMech.png");

}

void World::buildScene()
{

}