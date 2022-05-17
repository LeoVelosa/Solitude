#pragma once
#include "SceneNode.hpp"


class Entity : public SceneNode
{
	public:
		explicit			Entity(int hitpoints);

		void				setVelocity(sf::Vector2f velocity);
		void				setVelocity(float vx, float vy);
		void				setRotation2(float r);
		void				accelerate(sf::Vector2f velocity);
		void				accelerate(float vx, float vy);
		void 				rotation(float r);
		float				getRotation();
		sf::Vector2f		getVelocity() const;

		int					getHitpoints() const;
		void				repair(int points);
		void				damage(int points);
		void				destroy();
		virtual bool		isDestroyed() const;


	protected:
		virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);


	private:
		sf::Vector2f		mVelocity;
		int					mHitpoints;
		float				mRotation;
};
