#ifndef PARTICLE_H
#define PARTICLE_H

#include<SFML/Graphics.hpp>

class Particle {
    public:
        sf::Vector2f position;
        sf::Vector2f previous_position;
        sf::Vector2f acceleration;
        bool force_applied;
        bool is_pinned;
        float damping;  // Added damping for more realistic movement

        Particle(float x, float y, bool pinned = false) : 
            position(x, y),
            previous_position(x, y),
            acceleration(0, 0),
            force_applied(false),
            is_pinned(pinned),
            damping(0.99f) {}  // Increased damping factor for smoother motion

        void apply_force(const sf::Vector2f& force) {
            if(!is_pinned) {
                acceleration += force;
            }
        }

        void update(float time_step) {
            if(!is_pinned) {
                sf::Vector2f velocity = position - previous_position;
                
                // Apply damping to velocity
                velocity *= damping;
                
                previous_position = position;
                position += velocity + acceleration * time_step * time_step;
                
                // Reset acceleration
                acceleration = sf::Vector2f(0, 0);
                force_applied = false;
            }
        }

        void constrain_to_bounds(float width, float height) {
            sf::Vector2f velocity = position - previous_position;
            const float bounce = 0.1f;  // Low bounce factor for realistic behavior

            if(position.x < 0) {
                position.x = 0;
                previous_position.x = position.x - velocity.x * bounce;
            }
            else if(position.x > width) {
                position.x = width;
                previous_position.x = position.x - velocity.x * bounce;
            }

            if(position.y < 0) {
                position.y = 0;
                previous_position.y = position.y - velocity.y * bounce;
            }
            else if(position.y > height) {
                position.y = height;
                previous_position.y = position.y - velocity.y * bounce;
                // Add extra damping when hitting the ground
                velocity *= 0.8f;
                previous_position = position - velocity;
            }
        }
};

#endif