#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"
#include "Knight.hpp"
#include "SoundPlayer.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <array>
#include <queue>

// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
    public:
        explicit                            World(sf::RenderWindow& window, FontHolder& fonts);
                                            ~World();
        void                                update(sf::Time dt);
        void                                draw();

        CommandQueue&                       getCommandQueue();

        bool 								hasAlivePlayer() const;
        bool 								hasPlayerReachedEnd() const;
        bool                                checkWin();
        
    private:
        void                                loadTextures();
        void                                buildScene();
        void                                adaptPlayerPosition();
        void                                adaptPlayerVelocity();
        void								handleCollisions();
        void								updateSounds();
        void                                guideAI();
        void                                levelDone();
		
		void								addEnemies();
		void								addEnemy(Knight::Type type, float relX, float relY);
		void								spawnEnemies();
		void								destroyEntitiesOutsideView();
		sf::FloatRect						getViewBounds() const;
		sf::FloatRect						getBattlefieldBounds() const;

    private:
        enum Layer
        {
            Background,
            Floor,
            LayerCount
        };

        struct SpawnPoint 
		{
			SpawnPoint(Knight::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
			{
			}

			Knight::Type type;
			float x;
			float y;
		};
    
    private:
        sf::RenderWindow&                   mWindow;
        sf::View                            mWorldView;
        TextureHolder                       mTextures;
        FontHolder&							mFonts;
        //SoundPlayer&						mSounds;

        SceneNode                           mSceneGraph;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        CommandQueue                     mCommandQueue;

        sf::FloatRect                       mWorldBounds;
        sf::Vector2f                        mSpawnPosition;

        Knight*                             mPlayer;
        std::vector<SpawnPoint>				mEnemySpawnPoints;
		std::vector<Knight*>				mActiveEnemies;
        bool                                lvone;
        bool                                lvtwo;
        bool                                lvthree;
        bool                                win;

};