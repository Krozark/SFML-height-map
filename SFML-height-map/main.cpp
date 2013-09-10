#include <SFML-height-map/HeightMap.hpp>
#include <iostream>

using namespace height_map;
//using namespace std;

int main(int argc,char* argv[])
{
    HeightMap heightMap;
    heightMap.loadFromFile("data/map.png");

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML window");
    while (window.isOpen())
    {
         // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        
        window.draw(heightMap);

        window.display();
    }
    

    return 0;
}
