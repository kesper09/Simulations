#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>

// Adding the custom headers

#include"particle.h"
#include"constraint.h"

// Setting some constants needed in the main code

const int WIDTH = 1080;
const int HEIGHT = 640;
const float PARTICLE_RADIUS = 5.0f;
const float GRAVITY = 0.35f;
#define TIME_STEP 1.0f / 120.0f

const int ROW = 15;
const int COLUMN = 15;
const float REST_DISTANCE = 15.0f;


int main()
{
    sf:: RenderWindow window (sf:: VideoMode(WIDTH,HEIGHT),"Cloth Simulation");

    //Creating multiple points using the ROW and COLUMN and using the preset distance with REST_DISTANCE
    std::vector<Particle> particles;
    std::vector<Constraint> constraints;
    
    // Calculate starting position to center the grid
    float startX = (WIDTH - (COLUMN - 1) * REST_DISTANCE) / 2;
    float startY = (HEIGHT - (ROW - 1) * REST_DISTANCE) / 2;

    for(int row = 0; row < ROW; row++)
    {
        for(int col = 0; col < COLUMN; col++)
        {
            float x = startX + col * REST_DISTANCE;
            float y = startY + row * REST_DISTANCE;
            bool pinned = (row == 0);
            particles.emplace_back(x,y,pinned);
        }
    }

    // Setting the constraints
    for(int row = 0; row < ROW; row++)
    {
        for(int col = 0; col < COLUMN; col++)
        {
            if(col < COLUMN - 1)
            {
                // Horizontal Constraint
                constraints.emplace_back(&particles[row * COLUMN + col],&particles[row * COLUMN + col + 1]);
            }
            if(row < ROW - 1)
            {
                // Verticle constraint
                constraints.emplace_back(&particles[row * COLUMN + col],&particles[(row + 1) * COLUMN + col]);
            }
        }
    }

   /* std::vector<Particle> particles; // Creating the particles
    particles.emplace_back(WIDTH/2 - 50,HEIGHT/2 - 50);
    particles.emplace_back(WIDTH/2 + 50,HEIGHT/2 + 50);
    particles.emplace_back(WIDTH/2 + 50,HEIGHT/2 - 50);
    particles.emplace_back(WIDTH/2 - 50,HEIGHT/2 + 50);

    std::vector<Constraint> constraints; // Creating connections between the points
    constraints.emplace_back(&particles[0], &particles[1]);
    constraints.emplace_back(&particles[0], &particles[2]);
    constraints.emplace_back(&particles[0], &particles[3]);
    constraints.emplace_back(&particles[1], &particles[2]);
    constraints.emplace_back(&particles[1], &particles[3]);
    constraints.emplace_back(&particles[2], &particles[3]);*/
    
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
