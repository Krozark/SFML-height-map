#include <Camera/FreeFlyCamera.hpp>
#include <SFML-height-map/HeightMap.hpp>

#include <iostream>

using namespace height_map;
using namespace std;

int main(int argc,char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML window");
    window.setFramerateLimit(60);

    HeightMap heightMap;
    heightMap.loadFromFile("data/map.png","data/gradient.png");

    FreeFlyCamera camera(sf::Vector3<float>(-5,20,0));



    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
	// Quelques autres options OpenGL
    glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();

    //gluPerspective(70, 800.0/500.0, 0.001f, 1000.0f );
    //glScalef(0.04f,0.04f,1.f); // Diminution du rapport de hauteur

    sf::Clock clock;

    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();*/

    while (window.isOpen())
    {
         // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                camera.mouseMoved();
            }
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                camera.mouseWheelMoved(event);
            }

        }
        // Clear screen
        //window.clear(sf::Color::Red);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.animate(clock.getElapsedTime().asSeconds());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera.look();

                std::cout<<"pointer <"<<camera.getTarget().x<<" "<<camera.getTarget().y<<" "<<camera.getTarget().z<<">"<<std::endl;


        window.draw(heightMap);

        //glBegin( GL_TRIANGLES );

        //#include "map.hpp"

        /*glColor3f(0.027451,0.219608,0.603922);
        glVertex3f(1,1,0);

        glColor3f(0.466667,0.458824,0.0352941);
        glVertex3f(1,0,0);

        glColor3f(0.466667,0.694118,0.4);
        glVertex3f(0,0,0);

        glColor3f(0.0431373,0.458824,0.0352941);
        glVertex3f(0,1,0);
        glColor3f(0.027451,0.219608,0.603922);
        glVertex3f(1,1,0);
        glColor3f(0.466667,0.694118,0.4);
        glVertex3f(0,0,0);*/



        //glEnd();

        glFlush();

        window.display();
    }
    

    return 0;
}
