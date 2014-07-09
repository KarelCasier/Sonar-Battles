#pragma once

#include "StateStack.hpp"
#include "State.hpp"

#include <SFML/Graphics.hpp>
class PauseState : public State
{
public:
							PauseState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:

	sf::RectangleShape		menuBackground;
	sf::Text				pausedText;


};