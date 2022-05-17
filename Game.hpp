#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "State.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Game : private sf::NonCopyable 
{
    public:
                            Game();
                            ~Game();
        void                run();

    private:
        void                processInput();
        void                update(sf::Time dt);
        void                render();

        void					updateStatistics(sf::Time dt);
		void					registerStates();

    private:
        static const sf::Time	TimePerFrame;

        sf::RenderWindow		mWindow;
		TextureHolder			mTextures;
	  	FontHolder				mFonts;
		Player					mPlayer;

		StateStack				mStateStack;
        MusicPlayer				mMusic;
		SoundPlayer				mSounds;

		sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;
		std::size_t				mStatisticsNumFrames;
};