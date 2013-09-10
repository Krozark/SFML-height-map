#include "FreeFlyCamera.hpp"

#include <cmath>
//#include <GL/glu.h>



sf::Vector2i old_Pos;

FreeFlyCamera::FreeFlyCamera(const sf::vector3<float> & position)
{
    Position = position;
    old_Pos = sf::Mouse::getPosition();
    Avent = sf::vector3<float>(0.0,0.0,0.0);
    //Cible_vision = sf::vector3<float>(position[0]-1,position[1]+0.01,position[2]-0.01);
    //Cible_vision = position;
    Phi = 0;
    Theta = 0;
    VectorsFromAngles();

    Speed = 50;
    Sensibilitee_souris = 0.8;
    _verticalMotionActive = false;
    KeyConf["forward"] = sf::Keyboard::Up;
    KeyConf["backward"] = sf::Keyboard::Down;
    KeyConf["strafe_left"] = sf::Keyboard::Left;
    KeyConf["strafe_right"] = sf::Keyboard::Right;
    KeyConf["boost"] = sf::Keyboard::LShift;


    /*SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);*/
}

void FreeFlyCamera::MouseMoved()
{
    sf::Vector2i Pos =sf::Mouse::getPosition();

    Theta += (Pos.x - old_Pos.x) *Sensibilitee_souris;
    Phi -= (Pos.y - old_Pos.y)*Sensibilitee_souris;
    //Cible_vision = Position + Avent;

    old_Pos = Pos;
    VectorsFromAngles();
}

void FreeFlyCamera::MouseWheelMoved(const sf::Event & event)
{
    if (event.mouseWheel.delta >0) //coup de molette vers le haut
    {
        _verticalMotionActive = true;
        _timeBeforeStoppingVerticalMotion = 0.100;
        _verticalMotionDirection = 1;
    }
    else if (event.mouseWheel.delta < 0) //coup de molette vers le bas
    {
        _verticalMotionActive = true;
        _timeBeforeStoppingVerticalMotion = 0.100;
        _verticalMotionDirection = -1;
    }
}

void FreeFlyCamera::animate(double timestep)
{

    double realspeed = (sf::Keyboard::isKeyPressed(KeyConf["boost"]))?2*Speed:Speed;
    if (sf::Keyboard::isKeyPressed(KeyConf["forward"]))
    {
        Position += Avent * (realspeed * timestep);
    }
    if (sf::Keyboard::isKeyPressed(KeyConf["backward"]))
    {
        Position -= Avent * (realspeed * timestep);
    }
    if (sf::Keyboard::isKeyPressed(KeyConf["strafe_left"]))
    {
        Position += Droite * (realspeed * timestep);
    }
    if (sf::Keyboard::isKeyPressed(KeyConf["strafe_right"]))
    {
        Position -= Droite * (realspeed * timestep);
    }

    if (_verticalMotionActive)
    {
        if (timestep > _timeBeforeStoppingVerticalMotion)
            _verticalMotionActive = false;
        else
            _timeBeforeStoppingVerticalMotion -= timestep;
        Position += sf::vector3<float>(0,_verticalMotionDirection*realspeed*timestep,0);
    }

    Cible_vision = Position + Avent;

}

void FreeFlyCamera::VectorsFromAngles()
{
    static const sf::vector3<float> up(0,1,0);
    if (Phi > 89)
        Phi = 89;
    else if (Phi < -89)
        Phi = -89;
    double r_temp = cos(Phi*M_PI/180);
    Avent[0] = r_temp*cos(Theta*M_PI/180);
    Avent[1] = sin(Phi*M_PI/180);
    Avent[2] = r_temp*sin(Theta*M_PI/180);

    Droite = up.cross(Avent);
    Droite.normalize();

    Cible_vision = Position + Avent;
}

FreeFlyCamera::~FreeFlyCamera()
{
}
