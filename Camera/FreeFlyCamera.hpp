#ifndef FREEFLYCAMERA_H
#define FREEFLYCAMERA_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class FreeFlyCamera
{
    public:
    FreeFlyCamera(const sf::Vector3<float> & position = sf::Vector3<float>(0,0,0));

        void mouseMoved();
        void mouseWheelMoved(const sf::Event & event);

        void animate(double timestep);
        inline void setSpeed(double speed){
            speed = speed;
        };
        inline void setSensivity(double sensivity){
            sensitivityMouse = sensivity;
        };
        inline void setPosition(const sf::Vector3<float> & pos){
            position = pos;
            target = pos + before;
        };

        inline sf::Vector3<float> getPosition()const{
            return position;
        };
        inline sf::Vector3<float> getTarget()const{
            return target;
        };

        inline void look(){
            gluLookAt(position.x,position.y,position.z,
                      target.x,target.y,target.z,
                      0,1,0);
        };

        ~FreeFlyCamera();

    protected:
        double speed;
        double sensitivityMouse;

        //vertical motion stuffs
        double _timeBeforeStoppingVerticalMotion;
        bool _verticalMotionActive;
        int _verticalMotionDirection;

        std::map<std::string,sf::Keyboard::Key> keyConf;

        sf::Vector3<float> position;
        sf::Vector3<float> target;
        sf::Vector3<float> before;
        sf::Vector3<float> right;

        double Theta,
               Phi;

        void vectorsFromAngles();
        //void Init();
};

#endif //FREEFLYCAMERA_H
