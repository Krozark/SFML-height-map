#ifndef HEIGHT_MAP__HEIGHTMAP_HPP
#define HEIGHT_MAP__HEIGHTMAP_HPP

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


namespace height_map
{
    class HeightMap : public sf::Drawable
    {
        public:
            HeightMap();
            HeightMap(const HeightMap&) = delete;
            HeightMap& operator=(const HeightMap&) = delete;


            bool loadFromFile(const std::string& filename, const sf::IntRect& area=sf::IntRect());
            bool loadFromImage(const sf::Image& image, const sf::IntRect& area=sf::IntRect());

            void setPrecision(const unsigned int& precistion=1);

            virtual void draw(sf::RenderTarget &target,sf::RenderStates states=sf::RenderStates::Default) const;
        

        private:
            void compile();

            unsigned char* data;
            unsigned char gradient[256];
            unsigned int precision;
            unsigned int height,
                         width;

            GLuint gl_list_id;


    };
}

#endif
