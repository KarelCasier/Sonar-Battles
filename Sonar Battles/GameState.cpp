#include "GameState.hpp"

struct SubmarineMover
{
	SubmarineMover(GameObject::MoveDirection direction)
	: mDirection(direction)
	{
	}

	void operator() (Submarine& sub, sf::Time) const
	{
		sub.accelerate(mDirection);
	}

	Submarine::MoveDirection	mDirection;
};

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window)
{
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	//Real Time Events - Act while key pressed
	CommandQueue& commands = mWorld.getCommandQueue();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) //Accelerate Up
	{
		Command accelerateCommand;
		accelerateCommand.category = Category::PlayerSub;
		accelerateCommand.action = derivedAction<Submarine>(SubmarineMover(GameObject::MoveDirection::Up));
		commands.push(accelerateCommand);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Accelerate Left
	{
		Command accelerateCommand;
		accelerateCommand.category = Category::PlayerSub;
		accelerateCommand.action = derivedAction<Submarine>(SubmarineMover(GameObject::MoveDirection::Left));
		commands.push(accelerateCommand);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Accelerate Down
	{
		Command accelerateCommand;
		accelerateCommand.category = Category::PlayerSub;
		accelerateCommand.action = derivedAction<Submarine>(SubmarineMover(GameObject::MoveDirection::Down));
		commands.push(accelerateCommand);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Accelerate Right
	{
		Command accelerateCommand;
		accelerateCommand.category = Category::PlayerSub;
		accelerateCommand.action = derivedAction<Submarine>(SubmarineMover(GameObject::MoveDirection::Right));
		commands.push(accelerateCommand);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //Fire
	{
		Command fireCommand;
		fireCommand.category = Category::PlayerSub;
		//fireCommand.action = derivedAction<Submarine>([] (Submarine& sub, sf::Time) { sub.fire(); });
		//commands.push(fireCommand);
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	//mPlayer.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
		requestStackPush(StateID::Pause);

	return true;
}