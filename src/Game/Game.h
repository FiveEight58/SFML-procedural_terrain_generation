#ifndef GAME_H
#define GAME_H

#include "MenuMap/MenuMap.h"
#include "Renderer/renderer.h"
#include "Map/Map.h"


class Game
{
private:
    renderer render;
    std::vector<Chunk> chunks;
    Chunk chunk;

    int loadcounter;
    int x_counter = ((-chunkSize/2)-1);
    int y_counter = -(chunkSize/2);

    //Game Runner
    bool IsRunning = true;

    //Controls
    bool IsMovingUp = false;
    bool IsMovingDown = false;
    bool IsMovingRight = false;
    bool IsMovingLeft = false;
public:
    MenuMap menumap;

    void Run() 
    {
        menumap.MapGenerationRequested = true;
        while(IsRunning) 
        {
            Game::ProcessingInput();

            Game::Update();

            Game::Render();
        }
    }

    //Process Input
    void ProcessingInput()
    {
        sf::Event event;
        while (render.windows.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    HandlePlayerInput(event.key.code, true);
                    break;

                case sf::Event::KeyReleased:
                    HandlePlayerInput(event.key.code, false);
                    break;

                case sf::Event::Closed:
                    IsRunning = false;
                    render.windows.close(); 
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            {
                menumap.MapGenerationRequested = true;
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter)
            {
                menumap.menumapselected = true;
            }

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                render.windows.setView(sf::View(visibleArea));
            }
        }
    }

    //Handle input
    void HandlePlayerInput(sf::Keyboard::Key key, bool isPressed)
    {
        if(key == sf::Keyboard::W) 
        {
            IsMovingUp = isPressed;
        }

        if (key == sf::Keyboard::S)
        {
            IsMovingDown = isPressed;
        }

        if(key == sf::Keyboard::A) 
        {
            IsMovingRight = isPressed;
        }

        if(key == sf::Keyboard::D) 
        {
            IsMovingLeft = isPressed;
        }
    }
    //Update
    void Update()
    {
        //Up
        if(IsMovingUp)
        {
            if(menumap.view.getCenter().y >= -((chunkSize*screen_height)/2)/2.2)
            {
                menumap.view.move(0, -menumap.mov_speed);
            }
            
            menumap.displayposition();
        }

        //Down
        if(IsMovingDown) 
        {
            if(menumap.view.getCenter().y <= ((chunkSize*screen_height)/2)/2.2)
            {
                menumap.view.move(0, menumap.mov_speed);
            }
            
            menumap.displayposition();
        }

        //Right
        if(IsMovingRight) 
        {
            if(menumap.view.getCenter().x > -((chunkSize*screen_width)/2)/16)
            {
                menumap.view.move(-menumap.mov_speed, 0);
            }
            menumap.displayposition();
        }

        //Left
        if(IsMovingLeft)
        {
            if(menumap.view.getCenter().x < ((chunkSize*screen_width)/2)/16)
            {
                menumap.view.move(menumap.mov_speed, 0);
            }
            menumap.displayposition();
        }

        // Map generation
        if (menumap.MapGenerationRequested) {
            x_counter++;
            if(x_counter == -(chunkSize/2) && y_counter == -(chunkSize/2))
            {
                MenuMap();
                chunks.clear();
            }
            if(x_counter == chunkSize/2)
            {
                x_counter = -chunkSize/2;
                y_counter++;
            }
            //each call to this chunk_gen function passes it a different seed
            // std::cout<<"Seed: "<<menumap.seed1<<std::endl;
            Chunk &chunk = chunks.emplace_back();
            chunk = menumap.chunk_gen(menumap.seed1, x_counter, y_counter);
            std::cout<<menumap.seed1<<std::endl;
            if(x_counter == (chunkSize/2)-1 && y_counter == (chunkSize/2)-1)
            {
                menumap.MapGenerationRequested = false;
                x_counter = ((-chunkSize/2)-1);
                y_counter = (-chunkSize/2);
            }
        }

        if(menumap.menumapselected)
        {
            Map GameMap(menumap.seed1);
            menumap.menumapselected = false;
        }
    } 

    //Render
    void Render()
    {
        render.windows.clear();
        render.windows.setView(menumap.view);
        for(Chunk const &chunk : chunks) 
        {
            render.windows.draw(chunk.sprite);
        }
        render.windows.display();
    }
};
#endif