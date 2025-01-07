#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>

// Adding the custom headers

#include"particle.h"
#include"constraint.h"

// Setting some constants needed in the main code

const int WIDTH = 1080;
const int HEIGHT = 640;
const float PARTICLE_RADIUS = 12.0f;
const float GRAVITY = 0.35f;
#define TIME_STEP 1.0f / 120.0f

const int ROW = 15;
const int COLUMN = 15;
const float REST_DISTANCE = 30.0f;


int main()
{
    sf:: RenderWindow window (sf:: VideoMode(WIDTH,HEIGHT),"Cloth Simulation");

    std::vector<Particle> particles;
    particles.emplace_back(WIDTH/2 - 50,HEIGHT/2 - 50);
    particles.emplace_back(WIDTH/2 + 50,HEIGHT/2 + 50);
    particles.emplace_back(WIDTH/2 + 50,HEIGHT/2 - 50);
    particles.emplace_back(WIDTH/2 - 50,HEIGHT/2 + 50);

    std::vector<Constraint> constraints;
    constraints.emplace_back(&particles[0], &particles[1]);
    constraints.emplace_back(&particles[0], &particles[2]);
    constraints.emplace_back(&particles[0], &particles[3]);
    constraints.emplace_back(&particles[1], &particles[2]);
    constraints.emplace_back(&particles[1], &particles[3]);
    constraints.emplace_back(&particles[2], &particles[3]);
    
    while(window.isOpen())
    {
        sf:: Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
            // Applying Gravity

        // Apply the constraint
        for(size_t i = 0; i < 4; i++)
        {
            for(auto& constraint : constraints)
            {
                constraint.satisfy();
            }
        }

        for(auto& particle : particles)
        {
                particle.constrain_to_bounds(WIDTH,HEIGHT,PARTICLE_RADIUS);
                particle.apply_force(sf::Vector2f(0,GRAVITY));
                particle.update(TIME_STEP);
        }

        window.clear(sf::Color::Black);

            // Drawing the particles as balls
        for(const auto& particle: particles)
        {
            sf::CircleShape circle(PARTICLE_RADIUS);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(particle.position.x - PARTICLE_RADIUS, particle.position.y - PARTICLE_RADIUS);
            window.draw(circle);
        }

        // Draw constraints as lines
        for(auto& constraint: constraints)
        {
            sf::Vertex lines[] = {
                sf::Vertex(constraint.p1->position,sf::Color::White),
                sf::Vertex(constraint.p2->position,sf::Color::White),
            };
            window.draw(lines,2,sf::Lines);
        }

        window.display();
    }
}
