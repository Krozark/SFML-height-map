#include "FreeFlyCamera.hpp"

#include <cmath>



sf::Vector2i old_Pos;

FreeFlyCamera::FreeFlyCamera(const sf::Vector3<float> & pos)
{
    position = pos;
    old_Pos = sf::Mouse::getPosition();
    before = sf::Vector3<float>(0.0,0.0,0.0);
    //Cible_vision = sf::Vector3<float>(position[0]-1,position[1]+0.01,position[2]-0.01);
    //Cible_vision = position;
    Phi = 0;
    Theta = 0;
    vectorsFromAngles();

    speed = 50;
    sensitivityMouse = 0.8;
    _verticalMotionActive = false;
    keyConf["forward"] = sf::Keyboard::Up;
    keyConf["backward"] = sf::Keyboard::Down;
    keyConf["strafe_left"] = sf::Keyboard::Left;
    keyConf["strafe_right"] = sf::Keyboard::Right;
    keyConf["boost"] = sf::Keyboard::LShift;

    /*SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);*/
}

void FreeFlyCamera::mouseMoved()
{
    sf::Vector2i pos =sf::Mouse::getPosition();

    Theta += (pos.x - old_Pos.x) *sensitivityMouse;
    Phi -= (pos.y - old_Pos.y)*sensitivityMouse;
    //Cible_vision = Position + avant;

    old_Pos = pos;
    vectorsFromAngles();
}

void FreeFlyCamera::mouseWheelMoved(const sf::Event & event)
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

template<typename T>
sf::Vector3<T> operator*(const sf::Vector3<T>& vec,double value)
{
    return sf::Vector3<T>(vec.x*value,vec.y*value,vec.z*value);
}

template<typename T>
sf::Vector3<T> cross(const sf::Vector3<T>& v1,const sf::Vector3<T>& v2)
{
    return sf::Vector3<T>(
                     v1.y * v2.z - v1.z * v2.y,
                     v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x);
}
/**@brief Normalize this vector 
 * x^2 + y^2 + z^2 = 1 */
//TODO
/*SIMD_FORCE_INLINE btVector3& normalize() 
{
    return *this /= length();
}*/


void FreeFlyCamera::animate(double timestep)
{

    double realspeed = (sf::Keyboard::isKeyPressed(keyConf["boost"]))?2*speed:speed;
    if (sf::Keyboard::isKeyPressed(keyConf["forward"]))
    {
        position += before * (realspeed * timestep);
    }
    if (sf::Keyboard::isKeyPressed(keyConf["backward"]))
    {
        position -= before * (realspeed * timestep);
    }
    if (sf::Keyboard::isKeyPressed(keyConf["strafe_left"]))
    {
        position += right * (realspeed * timestep);
    }
    if (sf::Keyboard::isKeyPressed(keyConf["strafe_right"]))
    {
        position -= right * (realspeed * timestep);
    }

    if (_verticalMotionActive)
    {
        if (timestep > _timeBeforeStoppingVerticalMotion)
            _verticalMotionActive = false;
        else
            _timeBeforeStoppingVerticalMotion -= timestep;
        position += sf::Vector3<float>(0,_verticalMotionDirection*realspeed*timestep,0);
    }

    target = position + before;
}



void FreeFlyCamera::vectorsFromAngles()
{
    static const sf::Vector3<float> up(0,1,0);
    if (Phi > 89)
        Phi = 89;
    else if (Phi < -89)
        Phi = -89;
    double r_temp = cos(Phi*M_PI/180);
    before.x = r_temp*cos(Theta*M_PI/180);
    before.y = sin(Phi*M_PI/180);
    before.z = r_temp*sin(Theta*M_PI/180);

    right = cross(up,before);
    //TODO Droite.normalize();

    target = position + before;
}

FreeFlyCamera::~FreeFlyCamera()
{
}
