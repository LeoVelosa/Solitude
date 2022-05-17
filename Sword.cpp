#include "Sword.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>


namespace
{
	const std::vector<SwordData> SwordTable = initializeSwordData();
}

Sword::Sword(Type type, const TextureHolder& textures)
: Entity(10)
, mType(type)
, mSprite(textures.get(SwordTable[type].texture))
, mTargetDirection()
{
	centerOrigin(mSprite);
}

void Sword::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);
}

void Sword::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Sword::getCategory() const
{
	if (mType == EnemyBasicSword)
		return Category::EnemySword;
	else
		return Category::AlliedSword;
}

sf::FloatRect Sword::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Sword::getMaxSpeed() const
{
	return SwordTable[mType].speed;
}

int Sword::getDamage() const
{
	return SwordTable[mType].damage;
}
