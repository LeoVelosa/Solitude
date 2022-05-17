#include "Door.hpp"
#include "DataTables.hpp"
#include "Category.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<DoorData> DoorTable = initializeDoorData();
}

Door::Door(Type type, const TextureHolder& textures)
: Entity(DoorTable[type].hitpoints)
, mType(type)
, mSprite(textures.get(DoorTable[type].texture))
{

}

void Door::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Door::apply(Knight& player) const
{
	DoorTable[mType].action(player);
}

sf::FloatRect Door::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

unsigned int Door::getCategory() const
{
	return Category::Door;
}