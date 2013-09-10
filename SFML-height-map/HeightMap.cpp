#include <SFML-height-map/HeightMap.hpp>

namespace height_map
{

    HeightMap::HeightMap() : sf::Drawable(), data(0), precision(1), height(0), width(0)
    {
    }

    bool HeightMap::loadFromFile(const std::string& filename)
    {
        sf::Image image;

        if ( !image.loadFromFile(filename))
            return false;

        return loadFromImage(image);
    }

    bool HeightMap::loadFromImage(const sf::Image& image)
    {
        // La taille totale du tableau renvoyé par GetPixelsPtr()
        height = image.getSize().x;
        width = image.getSize().y;

        const int size = height * width;

        // Création de notre propre tableau qui ne contiendra qu'une composante (noir et blanc)
        data = new unsigned char[size];

        // On stocke un pointeur vers le tableau renvoyé par GetPixelsPtr() pour y accéder plus
        // simplement
        const unsigned char* const px = image.getPixelsPtr();

        // On copie les pixels
        for ( int i=0; i<size;i+=4 )
            data[i] = px[i];

        compile();

        return true;
    }

    #define HEIGHTMAP_GET_PIXEL(x,y) data[y*height+x]
    template <class T>
    class Point3D
    {
        public:
            Point3D ( T mx=0, T my=0, T mz=0, float mtx=0, float mty=0 ): x ( mx  ),y ( my  ),z ( mz  ),tx( mtx ),ty( mty ){}

            void send( void ) const
            {
                glVertex3f( static_cast<float>(x),
                            static_cast<float>(y),
                            static_cast<float>(z) );
            }

            void sendWithText( void ) const
            {
                glTexCoord2f( tx, ty );
                glVertex3f( static_cast<float>(x),
                            static_cast<float>(y),
                            static_cast<float>(z) );
            }

        public:
            T x,y,z;

            float tx;
            float ty;
    };

    void HeightMap::draw(sf::RenderTarget &target,sf::RenderStates states) const
    {
        //glClear          ( GL_DEPTH_BUFFER_BIT ); // Réinitialisation z-buffer
        //camera_.focus    (                     ); // gluLookAt
        glScalef(1.f,0.15f,1.f); // Diminution du rapport de hauteur
        //glEnable         ( GL_TEXTURE_2D       ); // Activation du texturing
        //texture_main.Bind(                     ); // Sélection de la texture principale
        glCallList       (gl_list_id); // Appel de la display list
    }

    void HeightMap::compile()
    {
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
                    for ( unsigned int y=0; y<(width-precision); y+=precision)
                    {
                        // Définition des coordonnées des points
                        Point3D<GLfloat> vertex1( x,
                                                  HEIGHTMAP_GET_PIXEL( x, y ),
                                                  y,
                                                  0,
                                                  HEIGHTMAP_GET_PIXEL(x,y)
                                                  );

                        Point3D<GLfloat> vertex2( precision+x,
                                                  HEIGHTMAP_GET_PIXEL(precision+x, y ),
                                                  y,
                                                  0,
                                                  HEIGHTMAP_GET_PIXEL(precision+x,y)
                                                  );

                        Point3D<GLfloat> vertex3( precision+x,
                                                  HEIGHTMAP_GET_PIXEL(precision+x, precision+y),
                                                  precision+y,
                                                  0,
                                                  HEIGHTMAP_GET_PIXEL(precision+x,precision+y)
                                                  );

                        Point3D<GLfloat> vertex4( x,
                                                  HEIGHTMAP_GET_PIXEL( x, precision+y ),
                                                  precision+y,
                                                  0,
                                                  HEIGHTMAP_GET_PIXEL(x,precision+y)
                                                  );

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
