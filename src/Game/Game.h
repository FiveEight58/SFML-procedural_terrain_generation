#include "../../src/MenuMap/MenuMap.h"
#include "../../src/Renderer/renderer.h"


class Game
{
private:
    renderer render;
    MenuMap map;
    std::vector<Chunk> chunks;

    //Game Runner
    bool IsRunning = true;

    //Controls
    bool IsMovingUp = false;
    bool IsMovingDown = false;
    bool IsMovingRight = false;
    bool IsMovingLeft = false;


public:

    void Run() 
    {
        map.MapGenerationRequested = true;
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
                map.MapGenerationRequested = true;
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter)
            {
                
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
            if(map.view.getCenter().y >= -((chunkSize*screen_height)/2)/2.2)
            {
                map.view.move(0, -map.mov_speed);
            }
            
            map.displayposition();
        }

        //Down
        if(IsMovingDown) 
        {
            if(map.view.getCenter().y <= ((chunkSize*screen_height)/2)/2.2)
            {
                map.view.move(0, map.mov_speed);
            }
            
            map.displayposition();
        }

        //Right
        if(IsMovingRight) 
        {
            if(map.view.getCenter().x > -((chunkSize*screen_width)/2)/16)
            {
                map.view.move(-map.mov_speed, 0);
            }
            map.displayposition();
        }

        //Left
        if(IsMovingLeft)
        {
            if(map.view.getCenter().x < ((chunkSize*screen_width)/2)/16)
            {
                map.view.move(map.mov_speed, 0);
            }
            map.displayposition();
        }

        // Map generation
        if (map.MapGenerationRequested) {
            chunks = map.regen();
            map.MapGenerationRequested = false;
        }
    } 

    //Render
    void Render()
    {
        render.windows.clear();
        render.windows.setView(map.view);
        for (Chunk const &chunk : chunks)
        {
            render.windows.draw(chunk.sprite);
        }
        render.windows.display();
        //render.windows.display();
    }
};