#include "StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories()
{
}

State::Ptr StateStack::createState(StateID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());
    
    return found->second();
}

void StateStack::handleEvent(const sf::Event& event)
{
    // iterate top to bottom, stop as soon as handleEvent() returns false. i.e whether or not the background states handle event.
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->handleEvent(event))
            break;
    }
    
    applyPendingChanges();
}

void StateStack::update(sf::Time dt)
{
    // iterate top to bottom, stop as soon as update() returns false. i.e whether or not the background states update.
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->update(dt))
            break;
    }
    
    applyPendingChanges();
}

void StateStack::draw()
{
    // iterate bottom to top drawing all
    for (State::Ptr& state : mStack)
    {
        state->draw();
    }
}

void StateStack::pushState(StateID stateID)
{
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

void StateStack::applyPendingChanges()
{
    for (PendingChange change : mPendingList)
    {
        switch (change.action) {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;
                
            case Pop:
                mStack.pop_back();
                break;
                
            case Clear:
                mStack.clear();
                break;
        }
    }
    
    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, StateID stateID)
: action(action)
, stateID(stateID)
{
}