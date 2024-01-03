#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>  
#include <SFML/Window.hpp>

class renderer
{
    public:
        sf::RenderWindow windows = sf::RenderWindow(sf::VideoMode(screenwidth, screenheight), "Procedural generation");

        renderer()
        {
            windows.setVerticalSyncEnabled(true);
        }
};

#endif