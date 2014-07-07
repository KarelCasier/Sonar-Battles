#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "StateStack.hpp"

#include <sstream>

class Application
{
public:
							Application();
	void					run();

private:
	void					processEvents();
	void					update(sf::Time dtTime);
	void					render();

	void					updateStatistics(sf::Time elapsedTime);
	void					registerStates();

private:
	//For FPS calculations and consistent movement
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;

	StateStack				mStateStack;

	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;

};
