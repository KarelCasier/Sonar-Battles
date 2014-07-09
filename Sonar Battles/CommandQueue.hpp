#pragma once

#include "Category.hpp"

class GameObject;

#include <SFML/System/Time.hpp>
#include <functional>
#include <queue>

struct Command
{
															Command();
	std::function < void(GameObject& object, sf::Time dt) >		action;
	unsigned int											category;
};

class CommandQueue
{
public:
	void					push(const Command& c);
	Command					pop();
	bool					isEmpty() const;

private:
	std::queue<Command>		mQueue;
};

template <typename Object, typename Function>
std::function<void(GameObject&, sf::Time)> derivedAction(Function fn)
{
	return [=](GameObject& node, sf::Time dt)
	{
		// Check if cast is safe
		assert(dynamic_cast<Object*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<Object&>(node), dt);
	};
}