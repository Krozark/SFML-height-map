#include <SFML-height-map/HeightMap.hpp>

namespace height_map
{
    #define HEIGHTMAP_GET_PIXEL(x,y,height) data[y*height+x]

    HeightMap::HeightMap() : data(0), precistion(0)
    {
    }

    bool HeightMap::loadFromFile(const std::string& filename, const sf::IntRect& area)
    {
        sf::Image image;

        if ( !image.LoadFromFile(filename))
            return false;

        return loadFromImage(image,area);
    }

    bool HeightMap::loadFromImage(const sf::Image& image,const sf::IntRect& area)
    {
        // Assignations

        // Le tableau renvoyé par GetPixelsPtr() contient les 4 composantes RGBA
        // sous forme de char
        static const int elemSize = 4;

        // La taille totale du tableau renvoyé par GetPixelsPtr()
        const int size = image.GetWidth() * image.GetHeight() * 4;

        // Création de notre propre tableau qui ne contiendra qu'une composante (noir et blanc)
        data = new unsigned char[size/elemSize];

        // On stocke un pointeur vers le tableau renvoyé par GetPixelsPtr() pour y accéder plus
        // simplement
        const unsigned char* const px = image.GetPixelsPtr();

        // On copie les pixels
        for ( int i=0; i<size/elemSize; ++i )
            data[i] = px[i*elemSize];

        return true;
    }

    bool HeightMap::compile()
    {
        unsigned int width ( image.GetWidth () );
        unsigned int height( image.GetHeight() );
        
        // Génération d'un identifiant pour notre display list
        gl_list_id = glGenLists( 1 );

        // On demande à OpenGL de compiler ce qui va suivre dans
        // une display list identifiée par list_, que l'on vient de
        // générer.
        glNewList( gl_list_id, GL_COMPILE );
        {
            // Nous informons OpenGL que ce qui va suivre est une
            // énumération de triangles.
            glBegin( GL_TRIANGLES );
            {
                //  Pour chaque ligne, avec un pas dépendant de la précision souhaitée
                for ( unsigned int x=0; x<(height-precision); x+=precision)
                {
                    // Pour chaque colonne, avec un pas dépendant de la précision souhaitée
                    for ( unsigned int z=0; z<(width-precision); z+=precision)
                    {
                        // Définition des coordonnées des points
                        Point3D<GLfloat> vertex1( x,
                                                  HEIGHTMAP_GET_PIXEL( x, z ),
                                                  z,
                                                  x/width,
                                                  1.f-(z/height);

                        Point3D<GLfloat> vertex2( precision+x,
                                                  HEIGHTMAP_GET_PIXEL(precision+x, z ),
                                                  z,
                                                  (x+precision)/width,
                                                  1.f-(z/height));

                        Point3D<GLfloat> vertex3( precision+x,
                                                  HEIGHTMAP_GET_PIXEL(precision+x, precision+z ),
                                                  precision+z,
                                                  (x+precision)/width,
                                                  1.f-((z+precision)/height) );

                        Point3D<GLfloat> vertex4( x,
                                                  HEIGHTMAP_GET_PIXEL( x, precision+z ),
                                                  precision+z,
                                                  (x)/(float)width,
                                                  1.f-((z+precision)/height));

                        // Premier triangle
                        vertex3.sendWithText();
                        vertex2.sendWithText();
                        vertex1.sendWithText();

                        // Deuxième triangle
                        vertex4.sendWithText();
                        vertex3.sendWithText();
                        vertex1.sendWithText();
                    }
                }
            }
            glEnd();
        }
        glEndList();
    }



}
