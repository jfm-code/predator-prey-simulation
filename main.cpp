#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include "ant_doodlebug.hpp"

using namespace std;

int main()
{
    //create the display window
    sf::RenderWindow window(sf::VideoMode(500, 500), "Ants & Doodlebugs");
    Grid board;
    //start looping the game 
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        //call some functions here to start the game 
        //Grid board; // Don't do this, it will create a new Grid object in each iteration
        
        board.simulate_turn();
        board.display(window);
        window.display();
        while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            
        }
        //sf::sleep(sf::milliseconds(500)); //slow it down
    }

    return 0;
}