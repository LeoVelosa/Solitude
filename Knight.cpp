#include "Knight.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "Sword.hpp"
#include "SoundNode.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <chrono>
#include <thread>
#include <unistd.h>

namespace
{
	const std::vector<KnightData> Table = initializeKnightData();
}

Knight::Knight(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mFireCommand()
, mFireCountdown(sf::Time::Zero)
, mIsFiring(false)
, mPlayedExplosionSound(false)
, mIsMarkedForRemoval(false)
, mFireRateLevel(1)
, mSpreadLevel(1)
, mDropPickupCommand()
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mHealthDisplay(nullptr)
, mTargetDirection()
, mSwingCommand()
, mIsSwinging(false)
, mSwingRateLevel(1)
, mSwingCountdown(sf::Time::Zero)
, mSwingClock()
, mSword(nullptr)
, mTexture()
{;
	//Sets Clock timer for sword swing
	sf::Clock clock;
	mSwingClock = clock;

	centerOrigin(mSprite);

	//Creates command for creating bullets
	mFireCommand.category = Category::Scene;
	mFireCommand.action   = [this, &textures] (SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	//Creates command for sword swings
	mSwingCommand.category = Category::Scene;
	mSwingCommand.action = [this, &textures] (SceneNode& node, sf::Time) {
		createSwords(node, textures);
	};

	//Creates command for creating pick ups
	mDropPickupCommand.category = Category::Scene;
	mDropPickupCommand.action   = [this, &textures] (SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));
	
	//Decides what sword 
	Sword::Type types = isAllied() ? Sword::AlliedBasicSword : Sword::EnemyBasicSword;

	std::unique_ptr<Sword> sword(new Sword(types, textures));
	mSword = sword.get();
	//Sets swords only to axeknights and the player
	if(isFollower() == Knight::AxeKnight || isAllied()) {
		
		sword->setPosition(getWorldPosition().x+20.f, getWorldPosition().y);
		attachChild(std::move(sword));	
	}

	updateTexts();
}

//This command will follow a position (usually the player)
void Knight::follow(sf::Vector2f position) {
	
	mTargetDirection = unitVector(position - getWorldPosition());
}

//Draws sprite on screen
void Knight::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

//Use this command to change texture
//Currently used to change texture based on direction of motion for the player
void Knight::changeTexture(std::string c) {
	if(!mTexture.loadFromFile(c)) {
		std::cout << "test";
	}
	mSprite.setTexture(mTexture);
}

void Knight::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
		checkPickupDrop(commands);

		mIsMarkedForRemoval = true;
		return;
	}
	//Updates position of any Enemy that is a follower
	if(isFollower()) {
		const float approachRate = 200.f;

		if(approachRate * dt.asSeconds() * mTargetDirection + getVelocity() != sf::Vector2f(0.f,0.f)) {
			sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
			newVelocity *= getMaxSpeed();
			float angle = std::atan2(newVelocity.y, newVelocity.x);

			//setRotation(toDegree(angle) + 90.f);
			setVelocity(newVelocity);
		}
	}
	
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
		checkPickupDrop(commands);
		//mExplosion.update(dt);

		// Play explosion sound only once
		if (!mPlayedExplosionSound)
		{
			//SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
			//playLocalSound(commands, soundEffect);

			mPlayedExplosionSound = true;
		}
		return;
	}

	// Check if bullets are fired
	checkProjectileLaunch(dt, commands);
	checkSwordSwing(dt, commands);

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

	// Update texts
	updateTexts();
}

//Currently only AxeKnights are followers
bool Knight::isFollower() const
{
	return mType == AxeKnight;
}


unsigned int Knight::getCategory() const
{
	if (isAllied())
		return Category::PlayerKnight;
	else
		return Category::EnemyKnight;
}

sf::FloatRect Knight::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Knight::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

//Green is the type of the player
bool Knight::isAllied() const
{
	return mType == Green;
}

//Grabs speed from datatable
float Knight::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Knight::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}

void Knight::increaseSpread()
{
	if (mSpreadLevel < 3)
		++mSpreadLevel;
}

void Knight::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}

//Makes knight swing sword. If its an AI, it swings if the datatable says the interval is not 0
void Knight::swing() {
	if(Table[mType].swingInterval != sf::Time::Zero)
		mIsSwinging = true;
}

void Knight::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();
	/*
	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition] (SoundNode& node, sf::Time)
		{
			//node.playSound(effect, worldPosition);
		});

	commands.push(command);
	*/
}

void Knight::updateMovementPattern(sf::Time dt)
{
	// Enemy knight movement pattern
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		// Moved long enough in current direction: Change direction
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Knight::checkPickupDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(3) == 0)
		commands.push(mDropPickupCommand);
}

void Knight::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Enemies try to fire all the time
	if (!isAllied())
		fire();

	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		//playLocalSound(commands, isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);
		
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}
}

void Knight::checkSwordSwing(sf::Time dt, CommandQueue& commands) {
	//If its an enemy knight, swing constantly
	if(!isAllied())
		swing();

	//swing if countdown is <= 0
	if(mIsSwinging && mSwingCountdown <= sf::Time::Zero) {
		commands.push(mSwingCommand);
		//Swings 40 degrees
		mSword->rotation(40.f);
		mSwingCountdown += Table[mType].swingInterval / (mSwingRateLevel + 1.f);
		mIsSwinging = false;
	}
	//after 1 second rotate sword back to original position
	if(mSwingClock.getElapsedTime().asSeconds() > 1.f) {
		mSwingClock.restart();
		if(mSword->getRotation() == 40.f)
			mSword->rotation(-40.f);
	}
	//if countdown is > 0, lower countdown
	else if(mSwingCountdown > sf::Time::Zero) {

		mSwingCountdown -= dt;
		mIsSwinging = false;
	}
}
void Knight::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (mSpreadLevel)
	{
		case 1:
			createProjectile(node, type, 0.0f, 0.5f, textures);
			break;

		case 2:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;

		case 3:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type,  0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;
	}
}

//Creates individual bullets
void Knight::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(yOffset * mSprite.getGlobalBounds().width, xOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(projectile->getMaxSpeed(), 0);

	float sign = isAllied() ? +1.f : -1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

//Unused
void Knight::createSwords(SceneNode& node, const TextureHolder& textures) const {

	//mSword->rotation(30.f);
	//mSword->rotation(-10.f);
}

void Knight::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}

void Knight::updateTexts()
{
	mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());
}
