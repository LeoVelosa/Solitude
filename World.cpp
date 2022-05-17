#include "World.hpp"
#include "Pickup.hpp"
#include "Projectile.hpp"
#include "Foreach.hpp"
#include "TextNode.hpp"
#include "Door.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

World::World(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mFonts(fonts)
, mSounds(sounds)
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 620.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mPlayer(nullptr)
, mEnemySpawnPoints()
, mActiveEnemies()
, lvone()
, lvtwo()
, lvthree()
{
	lvone = true;
    loadTextures();
    buildScene();
    mWorldView.setCenter(mSpawnPosition);
}

World::~World(){}

void World::update(sf::Time dt) {
    mPlayer->setVelocity(0.f,0.f);

    destroyEntitiesOutsideView();

    while(!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    adaptPlayerVelocity();

	guideAI();
	levelDone();

    handleCollisions();

    mSceneGraph.removeWrecks();
	spawnEnemies();

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();

	updateSounds();
}

void World::guideAI() {
	Command enemyAI;
	enemyAI.category = Category::EnemyKnight;
	enemyAI.action = derivedAction<Knight>([this] (Knight& enemy, sf::Time)
	{
		enemy.follow(mPlayer->getWorldPosition());
		if (enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});
	mCommandQueue.push(enemyAI);
}

void World::levelDone() {
	bool doorSpawned = false;
	if (mActiveEnemies.size() == 8 && lvone && !doorSpawned) {
		doorSpawned = true;
		std::unique_ptr<Door> door(new Door(Door::ExitDoor, mTextures));
		door->setPosition(500.0f, 500.0f);
		mSceneLayers[Background]->attachChild(std::move(door));
		mActiveEnemies.clear();
	}
	if (mActiveEnemies.size() == 8 && lvtwo && !doorSpawned) {
		doorSpawned = true;
		std::unique_ptr<Door> door(new Door(Door::ExitDoor, mTextures));
		door->setPosition(100.0f, 100.0f);
		mSceneLayers[Background]->attachChild(std::move(door));
		mActiveEnemies.clear();
	}
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
	return !mPlayer->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayer->getPosition());
}

void World::loadTextures()
{
    mTextures.load(Textures::GreenForward, "Images/knight-forward.png");
	mTextures.load(Textures::GreenBack, "Images/knight-back.png");
	mTextures.load(Textures::GreenLeft, "Images/knight-left.png");
	mTextures.load(Textures::GreenRight, "Images/knight-right.png");
	mTextures.load(Textures::Floor, "Images/room.jpg");
    mTextures.load(Textures::AxeKnight, "Images/axeknight.png");
	mTextures.load(Textures::GunKnight, "Images/gunknight.png");
	mTextures.load(Textures::Door, "Images/door.png");

	mTextures.load(Textures::Bullet, "Images/Bullet.png");
	mTextures.load(Textures::Sword, "Images/w1.png");

    mTextures.load(Textures::HealthRefill, "Images/HealthRefill.png");
	mTextures.load(Textures::FireSpread, "Images/FireSpread.png");
	mTextures.load(Textures::FireRate, "Images/FireRate.png");
}

void World::buildScene()
{
    for(std::size_t i=0;i<LayerCount;++i)
    {
        Category::Type category = (i == Floor) ? Category::Scene : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& texture = mTextures.get(Textures::Floor);
    sf::IntRect textureRect(mWorldBounds);
    //texture.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

    std::unique_ptr<Knight> player(new Knight(Knight::Green, mTextures, mFonts));
	mPlayer = player.get();
	mPlayer->setPosition(mSpawnPosition);
	mSceneLayers[Floor]->attachChild(std::move(player));

    addEnemies();
}

void World::addEnemies()
{
	bool lvonedone = false;
	bool lvtwodone = false;
	// Add enemies to the spawn point container
	if(lvone && lvonedone == false) {
		lvonedone = true;
		addEnemy(Knight::AxeKnight,    -200.f,  200.f);
		addEnemy(Knight::AxeKnight,    -200.f,  100.f);
		addEnemy(Knight::AxeKnight,    -200.f,  0.f);
		addEnemy(Knight::AxeKnight,    -200.f,  -100.f);
		addEnemy(Knight::AxeKnight,    -200.f,  -200.f);
		addEnemy(Knight::GunKnight,    250.f, 100.f);
		addEnemy(Knight::GunKnight,    250.f, 0.f);
		addEnemy(Knight::GunKnight,    250.f, -100.f);
	}
	if(lvtwo && lvtwodone == false) {
		lvtwodone = true;
		addEnemy(Knight::AxeKnight,    -200.f,  200.f);
		addEnemy(Knight::AxeKnight,    -200.f,  100.f);
		addEnemy(Knight::AxeKnight,    -200.f,  0.f);
		addEnemy(Knight::AxeKnight,    -200.f,  -100.f);
		addEnemy(Knight::AxeKnight,    -200.f,  -200.f);
		addEnemy(Knight::GunKnight,    -250.f, 100.f);
		addEnemy(Knight::GunKnight,    -250.f, 0.f);
		addEnemy(Knight::GunKnight,    -250.f, -100.f);
	}

	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [] (SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addEnemy(Knight::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();
		
		std::unique_ptr<Knight> enemy(new Knight(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(0.f);
		
		mSceneLayers[Floor]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyKnight;
	command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.destroy();
	});

	mCommandQueue.push(command);
}

void World::adaptPlayerPosition()
{
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());

    const float borderDistance = 40.f;

    sf::Vector2f position = mPlayer->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayer->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayer->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayer->setVelocity(velocity / std::sqrt(2.f));

}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerKnight, Category::EnemyKnight))
		{
			auto& player = static_cast<Knight&>(*pair.first);
			auto& enemy = static_cast<Knight&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}

		else if (matchesCategories(pair, Category::PlayerKnight, Category::Pickup))
		{
			auto& player = static_cast<Knight&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			// Apply pickup effect to player, then destroy it
			pickup.apply(player);
			pickup.destroy();
			player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
		}

		else if (matchesCategories(pair, Category::PlayerKnight, Category::Door))
		{
			auto& player = static_cast<Knight&>(*pair.first);
			auto& door = static_cast<Door&>(*pair.second);

			if(lvone) {
				lvone = false;
				lvtwo = true;
				addEnemies();
			}
			if(lvtwo) {
				lvtwo = false;
				lvthree = true;
				addEnemies();
			}
			door.destroy();
		}

		else if (matchesCategories(pair, Category::EnemyKnight, Category::AlliedProjectile)
			  || matchesCategories(pair, Category::PlayerKnight, Category::EnemyProjectile))
		{
			auto& knight = static_cast<Knight&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			// Apply projectile damage to Knight, destroy projectile
			knight.damage(projectile.getDamage());
			projectile.destroy();
		}
		else if (matchesCategories(pair, Category::EnemyKnight, Category::AlliedSword)
			  || matchesCategories(pair, Category::PlayerKnight, Category::EnemySword))
		{
			auto& knight = static_cast<Knight&>(*pair.first);
			auto& sword = static_cast<Sword&>(*pair.second);

			// Apply projectile damage to Knight, destroy projectile
			knight.damage(sword.getDamage());
			//sword.destroy();
		}
	}
}

void World::updateSounds()
{
	// Set listener's position to player position
	mSounds.setListenerPosition(mPlayer->getWorldPosition());

	// Remove unused sounds
	mSounds.removeStoppedSounds();
}


sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}