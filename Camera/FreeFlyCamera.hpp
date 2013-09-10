#ifndef FREEFLYCAMERA_H
#define FREEFLYCAMERA_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class FreeFlyCamera
{
    public:
    FreeFlyCamera(const sf::vector3<float> & position = sf::vector3<float>(0,0,0));

        void MouseMoved();
        void MouseWheelMoved(const sf::Event & event);

        void animate(double timestep);
        inline void setSpeed(double speed){
            Speed = speed;
        };
        inline void setSensivity(double sensivity){
            Sensibilitee_souris = sensivity;
        };
        inline void setPosition(const sf::vector3<float> & position){
            Position = position;Cible_vision = Position + Avent;
        };

        inline sf::vector3<float> GetPosition()const{
            return Position;
        };
        inline sf::vector3<float> GetCible()const{
            return Cible_vision;
        };

        inline void look(){
            gluLookAt(Position[0],Position[1],Position[2],
                      Cible_vision[0],Cible_vision[1],Cible_vision[2],
                      0,1,0);
        };

        ~FreeFlyCamera();
    protected:
        double Speed;
        double Sensibilitee_souris;

        //vertical motion stuffs
        double _timeBeforeStoppingVerticalMotion;
        bool _verticalMotionActive;
        int _verticalMotionDirection;

        std::map<std::string,sf::Keyboard::Key> KeyConf;

        sf::vector3<float> Position;
        sf::vector3<float> Cible_vision;
        sf::vector3<float> Avent;
        sf::vector3<float> Droite;
        double Theta;
        double Phi;

        void VectorsFromAngles();
        //void Init();
};

#endif //FREEFLYCAMERA_H
