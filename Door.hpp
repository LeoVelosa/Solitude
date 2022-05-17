#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Door : public Entity {
    public:
      enum Type
      {
              ExitDoor,
              Door2,
              TypeCount
      };

    public:
								Door(Type type, const TextureHolder& textures);

    private:
		    virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void                    apply(Knight& player) const;
        virtual unsigned int	getCategory() const;
        virtual sf::FloatRect	getBoundingRect() const;

    private:
        Type					mType;
        sf::Sprite				mSprite;

};