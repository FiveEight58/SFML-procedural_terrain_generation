#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


class player
{
    public:
        sf::CircleShape player;
        sf::Vector2i player_chunkpos;
        
        sf::Vector2i getPlayerChunkPos(sf::Vector2f playerpos) 
        {
            int Pos_chunkX = playerpos.x / 128;
            int Pos_chunkY = playerpos.y / 128;
            return sf::Vector2i(Pos_chunkX, Pos_chunkY);
        };
};