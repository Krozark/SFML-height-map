#include <SFML-height-map/HeightMap.hpp>
#include <iostream>

using namespace height_map;
using namespace std;

int main(int argc,char* argv[])
{
    HeightMap heightMap;
    cout<<"loading"<<endl;
    heightMap.loadFromFile("data/map.png");

    cout<<"end loading"<<endl;

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML window");
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
