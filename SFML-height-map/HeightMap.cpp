#include <SFML-height-map/HeightMap.hpp>

#include <iostream>

namespace height_map
{

    HeightMap::HeightMap() : sf::Drawable(), precision(1), height(0), width(0), gl_list_id(0)
    {
    }

    HeightMap::~HeightMap()
    {
        if(gl_list_id != 0)
            glDeleteLists(gl_list_id,1);
    }

    bool HeightMap::loadFromFile(const std::string& filename,const std::string& gradient_name)
    {
        sf::Image image;
        if ( !image.loadFromFile(filename))
            return false;

        sf::Image gradient;
        if ( !gradient.loadFromFile(gradient_name))
            return false;


        return loadFromImage(image,gradient);
    }

    bool HeightMap::loadFromImage(const sf::Image& image,const sf::Image& gradient)
    {
        if(gradient.getSize().y < 256)
            return false;


        height = image.getSize().x;
        width = image.getSize().y;

        const int size = height * width;

        // Création de notre propre tableau qui ne contiendra qu'une composante (noir et blanc)
        unsigned char data[size];

        {
            const unsigned char* const px = image.getPixelsPtr();

            // On copie les pixels
            for ( int i=0; i<size;++i )
                data[i] = px[i*4];
        }

        // Création de notre tableau de couleurs
        unsigned char gradient_data[256][3];
        {
            const unsigned char* const px = gradient.getPixelsPtr();

            for ( int i=0; i<256;++i )
            {
                gradient_data[i][0] = px[i*4];//R
                gradient_data[i][1] = px[i*4+1];//G
                gradient_data[i][2] = px[i*4+2];//B
                //gradient_data[i][3] = px[i*4+3];//A
            }
        }

        compile(data,gradient_data);

        return true;
    }

    #define HEIGHTMAP_GET_PIXEL(x,y) data[y*height+x]
    template <class T>
    class Point3D
    {
        public:
            Point3D ( T mx=0, T my=0, T mz=0):
                x(mx ),y(my),z(mz)
        {
            //std::cout<<"<"<<x<<","<<y<<","<<z<<": "<<R<<","<<G<<","<<B<<">"<<std::endl;
        }

            void toGl() const
            {
                glVertex3f(static_cast<float>(x),static_cast<float>(y),static_cast<float>(z));
            }
        public:
            T x,y,z;
    };

    void HeightMap::draw(sf::RenderTarget &target,sf::RenderStates states) const
    {
        glClear(GL_DEPTH_BUFFER_BIT); // Réinitialisation z-buffer
        //camera_.focus    (                     ); // gluLookAt
        //glLoadIdentity();                                   // Réinitialisation de la matrice Current Modelview
        //glScalef(1.f,0.15f,1.f); // Diminution du rapport de hauteur
        //texture_main.Bind(                     ); // Sélection de la texture principale

        glCallList(gl_list_id); // Appel de la display list
    }

    void HeightMap::compile(unsigned char* data,unsigned char gradient[255][3])
    {
        gl_list_id = glGenLists(1);

        glNewList( gl_list_id,GL_COMPILE);
        {
            glBegin( GL_TRIANGLES );
            {
                for (int x=0; x<(height-precision); x+=precision)
                {
                    for (int z=0; z<(width-precision); z+=precision)
                    {
                        unsigned int y1 = HEIGHTMAP_GET_PIXEL(x,z);
                        Point3D<GLfloat> vertex1(x,-1.f*z,0);

                        unsigned int y2 = HEIGHTMAP_GET_PIXEL(precision+x,z);
                        Point3D<GLfloat> vertex2(precision+x,-1.f*z,0);

                        unsigned int y3 = HEIGHTMAP_GET_PIXEL(precision+x,precision+z);
                        Point3D<GLfloat> vertex3(precision+x,-1.f*(precision+z),0);

                        unsigned int y4 = HEIGHTMAP_GET_PIXEL(x,precision+z);
                        Point3D<GLfloat> vertex4(x,-1.f*(precision+z),0);

                        // Premier triangle
                        glColor3f(gradient[y1][0],gradient[y1][1],gradient[y1][2]);
                        std::cout<<y1<<" "<<int(gradient[y1][0])<<" "<<int(gradient[y1][1])<<" "<<int(gradient[y1][2])<<std::endl;
                        vertex3.toGl();
                        vertex2.toGl();
                        vertex1.toGl();

                        vertex4.toGl();
                        vertex3.toGl();
                        vertex1.toGl();
                    }
                }
            }
            glEnd();
        }
        glEndList();
    }
}
