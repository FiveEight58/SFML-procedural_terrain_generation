#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>
#include <random>
#include <memory>
#include "chunk.h"
#include "../../lib/FastNoiseLite.h"
using namespace std;
const float screenwidth = 1920.f;
const float screenheight = 1080.f;
class MenuMap
{
private:

public:
    std::vector<Chunk> chunks;
    int mov_speed = 16;
    bool MapGenerationRequested = false;

    sf::View view = sf::View(sf::FloatRect(200.f, 200.f, screenwidth, screenheight));


    MenuMap() 
    {
        view.setCenter(0, 0);
    }

    void displayposition() 
    {
        cout<<"X:"<<view.getCenter().x<<"  Y:"<<view.getCenter().y<<endl;
        
    }


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

        // Seed creation
        random_device rd; // non-deterministic generator
        mt19937 gen(rd());
        int seed1 = gen();
        srand(seed1);
        int seed2 = rand();
        srand(seed2);
        int seed3 = rand();
        srand(seed3);
        int seed4 = rand();
        srand(seed4);
        //NoiseMaps


        FastNoiseLite Layout1 = noiseparams(1, 0.0001, FastNoiseLite::NoiseType::NoiseType_Perlin, seed1);   //Layouts
        FastNoiseLite Layout2= noiseparams(2, 0.001, FastNoiseLite::NoiseType::NoiseType_Perlin, seed2);
        FastNoiseLite Layout3= noiseparams(3, 0.01, FastNoiseLite::NoiseType::NoiseType_Perlin, seed3);
        FastNoiseLite Layout4= noiseparams(4, 0.01, FastNoiseLite::NoiseType::NoiseType_Perlin, seed4);

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
                        double Map1 = Layout1.GetNoise(float((screen_height * chunk_x) + x), float((screen_width * chunk_y) + y));
                        double Map2 = Layout2.GetNoise(float((screen_height * chunk_x) + x), float((screen_width * chunk_y) + y));
                        double Map3 = Layout3.GetNoise(float((screen_height * chunk_x) + x), float((screen_width * chunk_y) + y));
                        double Map4 = Layout4.GetNoise(float((screen_height * chunk_x) + x), float((screen_width * chunk_y) + y));
                        double Layout = Map1 + Map2 + Map3 + Map4;

                        Layout = (Layout + 1.0) / 2.0;
                        Layout = int(Layout* 255);

                        // Convert 2d representation of each index to 1d
                        int unsigned_y = y + ((screen_width / 2));
                        int unsigned_x = x + ((screen_height / 2));
                        // cout<<"Unsigned_Y is : "<<unsigned_y<<endl;

                        int CurrentPixelIndex2 = ((unsigned_y * screen_width) + unsigned_x) * 4;

                        //Medium level water
                        if (Layout < 100)
                        {
                            chunk.pixels[CurrentPixelIndex2] = {13}; //55
                            chunk.pixels[CurrentPixelIndex2 + 1] = {13}; //102 
                            chunk.pixels[CurrentPixelIndex2 + 2] = {224}; //200
                            chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                        }

                        // Shallow water
                        else if (Layout < 120)
                        {
                            chunk.pixels[CurrentPixelIndex2] = {65}; //55
                            chunk.pixels[CurrentPixelIndex2 + 1] = {105}; //102 
                            chunk.pixels[CurrentPixelIndex2 + 2] = {225}; //200
                            chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                        }

                        // Rest
                        else
                        {
                            if(Layout < 160)
                            {
                                chunk.pixels[CurrentPixelIndex2] = {223};
                                chunk.pixels[CurrentPixelIndex2 + 1] = {206};
                                chunk.pixels[CurrentPixelIndex2 + 2] = {159};
                                chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                            }
                            else
                            {
                                chunk.pixels[CurrentPixelIndex2] = {28}; //55
                                chunk.pixels[CurrentPixelIndex2 + 1] = {100}; //102 
                                chunk.pixels[CurrentPixelIndex2 + 2] = {0}; //200
                                chunk.pixels[CurrentPixelIndex2 + 3] = {255};
                            }
                        }
                    }
                }

                chunk.chunk_position.x = chunk_x;
                chunk.chunk_position.y = chunk_y;
                chunk.sprite.setPosition(screen_height * chunk_x, screen_width * chunk_y);
                chunk.sprite.setTexture(*chunk.texture);
                chunk.texture->update(chunk.pixels.data());
            }
        }
        std::cout<<chunks.size()<<endl;
        return chunks;
    }
};