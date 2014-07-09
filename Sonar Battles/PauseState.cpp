#include "PauseState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
{
	menuBackground.setSize(sf::Vector2f(200, 400));
	centerOrigin(menuBackground);
	menuBackground.setFillColor(sf::Color(0, 0, 0, 175));
	menuBackground.setPosition(context.window->getView().getSize() / 2.f);

	pausedText.setFont(context.fonts->get(FontID::Main));
	pausedText.setCharacterSize(40);
	pausedText.setColor(sf::Color::White);
	pausedText.setPosition(menuBackground.getPosition().x, menuBackground.getPosition().y - menuBackground.getGlobalBounds().height / 2 + 20);
	pausedText.setString("Paused");
	centerOrigin(pausedText);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(menuBackground);
	window.draw(pausedText);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
		requestStackPop();

	return false;
}