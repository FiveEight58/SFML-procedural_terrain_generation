#ifndef CHUNK_H
#define CHUNK_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <memory>
#include <vector>
#include <cstdint>

const int chunkSize = 16;
const int screen = 128;
const int screen_width = screen;
const int screen_height = screen;
const int pixel_channel = 4;

class Chunk
{
    public:
        std::shared_ptr<sf::Texture> texture;
        sf::Sprite sprite;
        std::vector<uint8_t> pixels;
        sf::Vector2f chunk_position;  //Cordinate of each chunk.
        sf::Vector2i player_pos;       //Cordinate of the chunk the player is currently in.

        Chunk()
        {
            //Dont know why i assigned texture and pixels to make_unique even tho i defined them as smart pointers
            //but when i change them the code breaks, offcoarse i wont try to figure out why.
            //if it works it works
            texture = std::make_unique<sf::Texture>();
            texture->create(screen_width, screen_height);
            pixels.resize(screen_height * screen_width * pixel_channel);
        }
        //Retrivies the chunk cordinates that player is currently in.
        sf::Vector2i getPlayer_Chunkpos(sf::Vector2i player_pos)
        {
            int pos_chunkX = player_pos.x / screen_width;
            int pos_chunkY = player_pos.y / screen_height;
            return sf::Vector2i(pos_chunkX, pos_chunkY);
        }
};
#endif