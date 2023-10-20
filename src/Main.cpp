#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cstdlib>
#include "../lib/FastNoiseLite.h"
#include <random>
#include <memory>
#include "chunk.h"
#include "player.h"
using namespace std;

FastNoiseLite noiseparams(int oct, double freq, FastNoiseLite::NoiseType type, int seed)
{
    FastNoiseLite noise;
    noise.SetSeed(seed);
    noise.SetNoiseType(type);
    noise.SetFractalLacunarity(3.0f);
    noise.SetFractalGain(0.4f);
    noise.SetFrequency(freq);
    noise.SetFractalOctaves(oct);
    noise.SetFractalType(noise.FractalType_FBm);
    return noise;
};

std::vector<Chunk> regen()
{
    std::vector<Chunk> chunks;

    // Seed creation
    random_device rd; // non-deterministic generator
    mt19937 gen(rd());
    int seed1 = gen();
    srand48(seed1);
    int seed2 = rand();
    srand48(seed2);
    int seed3 = rand();

    // Noise maps
    FastNoiseLite moisture1 = noiseparams(1, 0.0001, FastNoiseLite::NoiseType::NoiseType_Perlin, seed1);
    FastNoiseLite moisture2 = noiseparams(2, 0.001, FastNoiseLite::NoiseType::NoiseType_Perlin, seed1);
    FastNoiseLite moisture3 = noiseparams(3, 0.01, FastNoiseLite::NoiseType::NoiseType_Perlin, seed1);

    const int screen_width = 128;
    const int screen_height = 128;
    const int chunkSize = 32;
    // Chunk_Y
    for (float chunk_y = -chunkSize / 2; chunk_y < chunkSize / 2; chunk_y++)
    {
        // Chunk_X
        for (float chunk_x = -chunkSize / 2; chunk_x < chunkSize / 2; chunk_x++)
        {
            Chunk &chunk = chunks.emplace_back();

            // pixel_Y
            for (int y = -screen_height / 2; y < screen_height / 2; ++y)
            {

                // pixel_X
                for (int x = -screen_width / 2; x < screen_width / 2; ++x)
                {
                    double moist1 = moisture1.GetNoise(float((screen_height * chunk_y) + x), float((screen_width * chunk_x) + y));
                    double moist2 = moisture2.GetNoise(float((screen_height * chunk_y) + x), float((screen_width * chunk_x) + y));
                    double moist3 = moisture3.GetNoise(float((screen_height * chunk_y) + x), float((screen_width * chunk_x) + y));
                    double moist = moist1 + moist2 + moist3;
                    moist = (moist + 1.0) / 2.0;
                    moist = int(moist * 255);

                    // Convert 2d representation of each index to 1d
                    int unsigned_y = y + ((screen_width / 2));
                    int unsigned_x = x + ((screen_height / 2));
                    // cout<<"Unsigned_Y is : "<<unsigned_y<<endl;

                    int CurrentPixelIndex2 = ((unsigned_y * screen_width) + unsigned_x) * 4;
                    // Deep Water
                    if (moist < 100)
                    {
                        chunk.pixels[CurrentPixelIndex2] = {0};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {66};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {137};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                    // Deep water
                    else if (moist < 120)
                    {
                        chunk.pixels[CurrentPixelIndex2] = {55};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {102};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {200};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                    // Beaches
                    else if (moist < 122)
                    {
                        chunk.pixels[CurrentPixelIndex2] = {209};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {189};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {111};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                    // Plain
                    else if (moist < 160)
                    {
                        chunk.pixels[CurrentPixelIndex2] = {71};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {133};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {47};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                    // Jungle
                    else if (moist < 180)
                    {
                        chunk.pixels[CurrentPixelIndex2] = {28};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {101};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {1};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                    else if (moist < 200)
                    {
                        chunk.pixels[CurrentPixelIndex2] = {96};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {61};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {0};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                    else
                    {
                        chunk.pixels[CurrentPixelIndex2] = {63};
                        chunk.pixels[CurrentPixelIndex2 + 1] = {37};
                        chunk.pixels[CurrentPixelIndex2 + 2] = {0};
                        chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                    }
                }
            }

            chunk.chunk_position.x = chunk_x;
            chunk.chunk_position.y = chunk_y;
            chunk.sprite.setPosition(screen_height * chunk_y, screen_width * chunk_x);
            chunk.sprite.setTexture(*chunk.texture);
            chunk.texture->update(chunk.pixels.get());
            chunks.push_back(chunk);
        }
    }
    return chunks;
}

int main()
{
    // Camera View
    sf::View view(sf::FloatRect(200.f, 200.f, 1920.f, 1080.f));
    view.setCenter(0, 0);
    const int mov_speed = 64;

    // Center of screen indicator
    player Player;
    Player.player.setRadius(2);
    Player.player.setFillColor(sf::Color::Red);
    Player.player.setPosition(0, 0);

    // Window renderer
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Procedural Generation");
    window.setVerticalSyncEnabled(true);

    std::vector<Chunk> chunks = regen();

    while (window.isOpen()) // Main Game Loop
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
                chunks = regen();

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter)
                chunks = regen();

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
            {
                view.move(-mov_speed, 0);
                Player.player.move(-mov_speed, 0);
                cout <<  Player.getPlayerChunkPos(Player.player.getPosition()).x << endl;
                cout <<  Player.getPlayerChunkPos(Player.player.getPosition()).y << endl;
                cout << " " << endl;
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
            {
                view.move(mov_speed, 0);
                Player.player.move(mov_speed, 0);
                cout << Player.getPlayerChunkPos(Player.player.getPosition()).x << endl;
                cout << Player.getPlayerChunkPos(Player.player.getPosition()).y << endl;
                cout << " " << endl;
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
            {
                view.move(0, -mov_speed);
                Player.player.move(0, -mov_speed);
                cout << Player.getPlayerChunkPos(Player.player.getPosition()).x << endl;
                cout << Player.getPlayerChunkPos(Player.player.getPosition()).y << endl;
                cout << " " << endl;
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            {
                view.move(0, mov_speed);
                Player.player.move(0, mov_speed);
                cout << Player.getPlayerChunkPos(Player.player.getPosition()).x << endl;
                cout << Player.getPlayerChunkPos(Player.player.getPosition()).y << endl;
                cout << " " << endl;
            }
            window.draw(Player.player);
        }

        window.clear();
        window.setView(view);
        for (Chunk const &chunk : chunks)
        {
            window.draw(chunk.sprite);
        }
        window.draw(Player.player);
        window.display();
    }

    return 0;
}