#pragma once
#include "Entity.hpp"
#include "Projectile.hpp"
#include "TextNode.hpp"
#include "Sword.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Knight : public Entity
{
	public:
		enum Type
		{
            Green,
			AxeKnight,
			GunKnight,
			TypeCount
		};


	public:
								Knight(Type type, const TextureHolder& textures, const FontHolder& fonts);
		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		virtual bool 			isMarkedForRemoval() const;
		bool					isAllied() const;
		bool 					isFollower() const;
		void					follow(sf::Vector2f position);
		float					getMaxSpeed() const;
		void 					changeTexture(std::string s);

		void					increaseFireRate();
		void					increaseSpread();

		void 					fire();
		void					swing();
		TextureHolder&			getTextures();
		void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

	private:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
		void					updateMovementPattern(sf::Time dt);
		void					checkPickupDrop(CommandQueue& commands);
		void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
		void					checkSwordSwing(sf::Time dt, CommandQueue& commands);

		void					createBullets(SceneNode& node, const TextureHolder& textures) const;
		void					createSwords(SceneNode& node, const TextureHolder& textures) const;
		void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
		void					createPickup(SceneNode& node, const TextureHolder& textures) const;

		void					updateTexts();


	private:
		Type					mType;
		sf::Sprite				mSprite;
		sf::FloatRect			mBounds;
		Command 				mFireCommand;
		Command					mSwingCommand;
		sf::Time				mFireCountdown;
		sf::Time				mSwingCountdown;
		sf::Clock				mSwingClock;
		bool 					mIsFiring;
		bool 					mIsMarkedForRemoval;
		bool					mIsSwinging;
		bool					mPlayedExplosionSound;

		int						mFireRateLevel;
		int						mSwingRateLevel;
		int						mSpreadLevel;

		Command 				mDropPickupCommand;
		float					mTravelledDistance;
		std::size_t				mDirectionIndex;
		TextNode*				mHealthDisplay;
		sf::Vector2f			mTargetDirection;
		Sword*					mSword;
		sf::Texture				mTexture;
		
};