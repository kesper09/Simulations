#ifndef PARTICLE_H
#define PARTICLE_H

#include<SFML/Graphics.hpp>

class Particle{
    public:
        sf::Vector2f position;
        sf::Vector2f previous_position;
        sf::Vector2f acceleration;
        bool force_applied;
        bool is_grounded;

        Particle(float x, float y) : position(x,y), previous_position(x,y), acceleration(0,0), force_applied(false), is_grounded(false) {}

        void apply_force(const sf::Vector2f& force){
            if(!force_applied && !is_grounded)
                acceleration += force;
            force_applied = true;
        }

        void update(float time_step){
            if(!is_grounded) {
                sf::Vector2f velocity = position - previous_position;
                previous_position = position;
                position += velocity + acceleration * time_step * time_step;
                acceleration = sf::Vector2f(0,0);
            }
            force_applied = false;
        }

        void constrain_to_bounds(float width, float height, float radius) {   
            const float bounce_damping = 0.8f;  // Natural bounce for side collisions
            
            sf::Vector2f velocity = position - previous_position;
            
            // Side collisions - maintain natural movement
            if(position.x < 2*radius) {
                position.x = 2*radius;
                velocity.x *= -bounce_damping;  // Reverse and slightly dampen horizontal velocity
                previous_position = position - velocity;  // Update previous position to maintain momentum
            }
            if(position.x > width - 2*radius) {
                position.x = width - 2*radius;
                velocity.x *= -bounce_damping;
                previous_position = position - velocity;
            }
            
            // Ceiling collision - natural bounce
            if(position.y < 2*radius) {
                position.y = 2*radius;
                velocity.y *= -bounce_damping;
                previous_position = position - velocity;
            }
            
            // Ground collision - complete stop
            if(position.y > height - 2*radius) {
                position.y = height - 2*radius;
                previous_position = position;  // Stop all movement
                is_grounded = true;
            }
        }
};

#endif