#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Knight;

class Pickup : public Entity
{
	public:
		enum Type
		{
			HealthRefill,
			FireSpread,
			FireRate,
			TypeCount
		};


	public:
								Pickup(Type type, const TextureHolder& textures);

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;

		void 					apply(Knight& player) const;


	protected:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type 					mType;
		sf::Sprite				mSprite;
};