#include "Application.h"

//#include "Utility.hpp"
#include "State.h"
#include "StateIdentifiers.hpp"
// #include "TitleState.h"
#include "GameState.h"
// #include "MenuState.h"
// #include "PauseState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Application::Application()
: mWindow(sf::VideoMode(1280, 720), "Sonar Battles", sf::Style::Close)
, mTextures()
, mFonts()
, mStateStack(State::Context(mWindow, mTextures, mFonts))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(FontID::Main, "assets/MYRIADPRO-BOLD.OTF");

	mStatisticsText.setFont(mFonts.get(FontID::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	registerStates();
	mStateStack.pushState(StateID::Game);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart(); //Note restart returns the time on the clock then restarts the clock
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) //Ensures that the logic is caught up before rendering again
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Application::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Key::Escape)
				mWindow.close();
		}
	}
}

void Application::update(sf::Time dtTime)
{
	mStateStack.update(dtTime);
}

void Application::updateStatistics(sf::Time elapsedTime)
{
	std::stringstream strStream;

	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		strStream << "FPS = " << mStatisticsNumFrames;

		mStatisticsText.setString(strStream.str());

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::registerStates()
{
	//mStateStack.registerState<TitleState>(StateID::Title);
	//mStateStack.registerState<MenuState>(StateID::Menu);
	mStateStack.registerState<GameState>(StateID::Game);
	//mStateStack.registerState<PauseState>(StateID::Pause);
}