#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Knight.hpp"
#include "Foreach.hpp"
#include "Category.hpp"
#include "Sword.hpp"

#include <map>
#include <string>
#include <algorithm>

struct PlayerMover
{
	PlayerMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	//Moves player and depending on movement, set sprite to proper image
	void operator() (Knight& knight, sf::Time) const
	{
		knight.accelerate(velocity * knight.getMaxSpeed());
		if(velocity.x < 0) {
			knight.changeTexture("Images/knight-left.png");
		}
		else if(velocity.x > 0) {
			knight.changeTexture("Images/knight-right.png");
		}
		else if(velocity.y < 0) {
			knight.changeTexture("Images/knight-back.png");
		}
		else if(velocity.y > 0) {
			knight.changeTexture("Images/knight-forward.png");
		}
	}

	sf::Vector2f velocity;
};

Player::Player()
: mCurrentMissionStatus(MissionRunning)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	//mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::Space] = Swing;

	// Set initial action bindings
	initializeActions();	

	// Assign all categories to player's Knight
	FOREACH(auto& pair, mActionBinding)
		pair.second.category = Category::PlayerKnight;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

//Sets actions to the key presses
void Player::initializeActions()
{
	mActionBinding[MoveLeft].action	 = derivedAction<Knight>(PlayerMover(-1, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Knight>(PlayerMover(+1, 0.f));
	mActionBinding[MoveUp].action    = derivedAction<Knight>(PlayerMover(0.f, -1));
	mActionBinding[MoveDown].action  = derivedAction<Knight>(PlayerMover(0.f, +1));
	mActionBinding[Fire].action      = derivedAction<Knight>([] (Knight& a, sf::Time){ a.fire(); });
	mActionBinding[Swing].action	 = derivedAction<Knight>([] (Knight& b, sf::Time){ b.swing(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
		case Fire:
		case Swing:
			return true;

		default:
			return false;
	}
}
