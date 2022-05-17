#include "Game.hpp"
#include "Button.cpp"
#include "DataTables.cpp"
#include "Door.cpp"
#include "World.cpp"
#include "Knight.cpp"
#include "Player.cpp"
#include "Pickup.cpp"
#include "Projectile.cpp"
#include "SceneNode.cpp"
#include "SpriteNode.cpp"
#include "Entity.cpp"
#include "Container.cpp"
#include "Component.cpp"
#include "Command.cpp"
#include "CommandQueue.cpp"
#include "Utility.cpp"
#include "Utility.inl"
#include "Label.cpp"
#include "State.cpp"
#include "TitleState.cpp"
#include "TextNode.cpp"
#include "GameState.cpp"
#include "MenuState.cpp"
#include "PauseState.cpp"
#include "SettingsState.cpp"
#include "SoundPlayer.cpp"
#include "SoundNode.cpp"
#include "MusicPlayer.cpp"
#include "StateStack.cpp"
#include "Sword.cpp"
#include "GameOverState.cpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: mWindow(sf::VideoMode(1070, 620), "Solitude", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mMusic()
, mSounds()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Main, 	"Sansation.ttf");

	mTextures.load(Textures::TitleScreen,		"Images/TitleScreen.png");
	mTextures.load(Textures::ButtonNormal,		"Images/ButtonNormal.png");
	mTextures.load(Textures::ButtonSelected,	"Images/ButtonSelected.png");
	mTextures.load(Textures::ButtonPressed,		"Images/ButtonPressed.png");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);

	mMusic.setVolume(25.f);
}

Game::~Game(){}

int main()
{
    Game game;
    game.run();
}

void Game::run() 
{
    sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Game::processInput()
{
    sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Game::update(sf::Time dt) 
{
    mStateStack.update(dt);
}

void Game::render()
{
    mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Game::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
}