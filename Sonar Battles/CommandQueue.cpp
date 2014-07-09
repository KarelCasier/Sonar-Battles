#include "CommandQueue.hpp"
//#include "GameObject.hpp"

Command::Command()
: action()
, category(Category::None)
{
}

void CommandQueue::push(const Command& c)
{
	mQueue.push(c);
}

Command CommandQueue::pop()
{
	Command command = mQueue.front();
	mQueue.pop();
	return command;
}

bool CommandQueue::isEmpty() const
{
	return mQueue.empty();
}