#ifndef PARTICLE_H
#define PARTICLE_H


#include<SFML/Graphics.hpp>

class Particle{
    public:
        sf:: Vector2f position;
        sf:: Vector2f previous_position;
        sf:: Vector2f acceleration;

        Particle(float x, float y) : position (x,y), previous_position(x,y), acceleration(0,0){}

        void apply_force(const sf::Vector2f& force){
            // This is to apply the force to the particle
            acceleration += force;
        }

        void update(float time_step){
            // Verlet integration
            sf::Vector2f velocity = position - previous_position;
            previous_position = position;
            position += velocity + acceleration * time_step * time_step;
            acceleration = sf::Vector2f(0,0); // reset after the update
        }

        void constrain_to_bounds(float width, float height, float radius)
        {   
            
            if(position.x < 2*radius) // Ensures the particle's x-coordinate does not go below the specified radius
            {
                position.x = 2*radius;
            }
            if(position.x > width - 2*radius) // Ensures the particle's x-coordinate does not exceed the screen width minus the specified radius
            {
                position.x = width - 2*radius;
            } 
            if(position.y < 2*radius) //Ensures the particle's y-coordinate does not go below the specified radius
            {
                position.y = 2*radius;
            }
            if(position.y > height - 2*radius) // Ensures the particle's y-coordinate does not exceed the screen width minus the specified radius
            {
                position.y = height - 2*radius;
            }
        }
};

#endif