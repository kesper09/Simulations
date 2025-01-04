#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>

#include"particle.h"

const int WIDTH = 1080;
const int HEIGHT = 640;
const float PARTICLE_RADIUS = 12.0f;
const float GRAVITY = 9.8f;
const float TIME_STEP = 0.1f;

const int ROW = 15;
const int COLUMN = 15;
const float REST_DISTANCE = 30.0f;


int main()
{
    sf:: RenderWindow window (sf:: VideoMode(WIDTH,HEIGHT),"Cloth Simulation");

    std::vector<Particle> particles;
    particles.emplace_back(WIDTH/2,HEIGHT/2);


    while(window.isOpen())
    {
        sf:: Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            window.clear(sf::Color::Black);

            // Drawing the particles as balls
            for(const auto& particle: particles)
            {
                sf::CircleShape circle(PARTICLE_RADIUS);
                circle.setFillColor(sf::Color::White);
                circle.setPosition(particle.position);
                window.draw(circle);
            }


            window.display();
        }
    }
}