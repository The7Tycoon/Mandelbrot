#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <sstream>

struct Param
{
    float x1;
    float x2;
    float y1;
    float y2;

    float zoom;

    int max_iter;
};

sf::Image drawManderbolt(Param p);
Param zoomIn(Param current, int mouseX, int mouseY, int w, int h);
Param zoomOut(Param current, int mouseX, int mouseY, int w, int h);

template <typename T> std::string to_string(T const& value) ;

int main()
{


    Param currentParameter;
        currentParameter.x1 = -2.1;
        currentParameter.x2 = 0.6;
        currentParameter.y1 = -1.2;
        currentParameter.y2 = 1.2;
        currentParameter.zoom = 400;
        currentParameter.max_iter = 50;


    clock_t chrono;
    sf::Image image = drawManderbolt(currentParameter);
    chrono = clock();
    std::cout << "Initial calculation time (x" << currentParameter.zoom << ", " << currentParameter.max_iter << " iter) : " << (double) chrono/CLOCKS_PER_SEC << " s" << std::endl;

    std::string title = "Mandelbrot - Generated in " + to_string( (double) chrono/CLOCKS_PER_SEC ) + "s";
    sf::RenderWindow window(sf::VideoMode(image.getSize().x, image.getSize().y), title);

    // Chargement de l'image dans une texture
    sf::Texture texture;
    texture.loadFromImage(image);

    // Puis de la texture dans un sprite
    sf::Sprite sprite;
    sprite.setTexture(texture);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(event.type == sf::Event::MouseWheelMoved)
            {
                int delta, x, y = 0;
                delta = event.mouseWheel.delta;
                x = sf::Mouse::getPosition(window).x;
                y = sf::Mouse::getPosition(window).y;

                if(delta == 1)
                {
                    currentParameter = zoomIn(currentParameter, x, y, image.getSize().x, image.getSize().y);
                }
                else if(delta == -1)
                {
                    currentParameter = zoomOut(currentParameter, x, y, image.getSize().x, image.getSize().y);
                }
                else
                {

                }

                image = drawManderbolt(currentParameter);
                texture.loadFromImage(image);
                sprite.setTexture(texture);

            }

        }

        window.clear();

        // Affichage de l'image
        window.draw(sprite);

        window.display();
    }



    return EXIT_SUCCESS;
}

sf::Image drawManderbolt(Param p)
{
    int imageX = (p.x2 - p.x1) * p.zoom;
    int imageY = (p.y2 - p.y1) * p.zoom;

    sf::Image image;
    image.create(imageX, imageY, sf::Color::Black);

    float c_r = 0, c_i = 0,
          z_r = 0, z_i = 0;
    int i = 0;

    for(int x = 0; x < imageX; ++x)
    {
        for(int y = 0; y < imageY; ++y)
        {
            c_r = x / p.zoom + p.x1;
            c_i = y / p.zoom + p.y1;
            z_r = 0;
            z_i = 0;

            i = 0;

            do
            {
                float tmp = z_r;
                z_r = z_r * z_r - z_i * z_i + c_r;
                z_i = 2 * z_i * tmp + c_i;
                i++;
            }while(z_r * z_r + z_i * z_i < 4 && i < p.max_iter);

            if(i == p.max_iter)
            {
                image.setPixel(x, y, sf::Color::White);
            }

            else
            {
                image.setPixel(x, y, sf::Color(0, 0, i * 255 / p.max_iter));
            }

        }
    }

    return image;
}



Param zoomIn(Param current, int mouseX, int mouseY, int w, int h)
{
    Param newParameter;

    newParameter.zoom = current.zoom * 2;

    // Coordonnées de la sourie dans le "vrai" repère
    float X = current.x1 + mouseX * (current.x2 - current.x1) / w;
    float Y = current.y1 + mouseY * (current.y2 - current.y1) / h;

    newParameter.x1 = X - (current.x2 - current.x1) / 4;
    newParameter.x2 = X + (current.x2 - current.x1) / 4;
    newParameter.y1 = Y - (current.y2 - current.y1) / 4;
    newParameter.y2 = Y + (current.y2 - current.y1) / 4;

    newParameter.max_iter = current.max_iter * 1.5;

    std::cout << "Zoom level: x" << newParameter.zoom << ", " << newParameter.max_iter << " max iterations" << std::endl;

    return newParameter;
}

Param zoomOut(Param current, int mouseX, int mouseY, int w, int h)
{
    Param newParameter;

    newParameter.zoom = current.zoom / 2;

    // Coordonnées de la sourie dans le "vrai" repère
    float X = current.x1 + mouseX * (current.x2 - current.x1) / w;
    float Y = current.y1 + mouseY * (current.y2 - current.y1) / h;

    newParameter.x1 = X - (current.x2 - current.x1);
    newParameter.x2 = X + (current.x2 - current.x1);
    newParameter.y1 = Y - (current.y2 - current.y1);
    newParameter.y2 = Y + (current.y2 - current.y1);

    newParameter.max_iter = current.max_iter / 1.5;

    std::cout << "Zoom level: x" << newParameter.zoom << ", " << newParameter.max_iter << " max iterations" << std::endl;

    return newParameter;
}






template <typename T> std::string to_string(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}
