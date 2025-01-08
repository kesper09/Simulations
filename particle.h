#ifndef PARTICLE_H
#define PARTICLE_H

#include<SFML/Graphics.hpp>

class Particle{
    public:
        sf::Vector2f position;
        sf::Vector2f previous_position;
        sf::Vector2f acceleration;
        bool force_applied;
        bool is_pinned;

        Particle(float x, float y, bool pinned = false) : position(x,y), previous_position(x,y), acceleration(0,0),force_applied(false),is_pinned(pinned) {}

        void apply_force(const sf::Vector2f& force){
            if(!force_applied)
                acceleration += force;
            force_applied = true;
            if(!is_pinned)
            {
                acceleration += force;
            }
        }

        void update(float time_step){
            if(!is_pinned){
                sf::Vector2f velocity = position - previous_position;
                previous_position = position;
                position += velocity + acceleration * time_step * time_step;
                acceleration = sf::Vector2f(0,0);
                force_applied = false;
            }
        }

        void constrain_to_bounds(float width, float height, float radius) {   
            sf::Vector2f velocity = position - previous_position;
            
            // Strong damping for floor collision to prevent floating
            const float floor_damping = 0.003f;  // Reduces bounce height significantly
            // Less damping for wall collisions to maintain horizontal movement
            const float wall_damping = 0.008f;
            
            // Only apply strong bounce damping when velocity is significant
            const float min_bounce_velocity = 0.005f;
            
            if(position.x < 2*radius) {
                position.x = 2*radius;
                velocity.x *= -wall_damping;
                previous_position.x = position.x - velocity.x;
            }
            if(position.x > width - 2*radius) {
                position.x = width - 2*radius;
                velocity.x *= -wall_damping;
                previous_position.x = position.x - velocity.x;
            }
            if(position.y < 2*radius) {
                position.y = 2*radius;
                velocity.y *= -wall_damping;
                previous_position.y = position.y - velocity.y;
            }
            if(position.y > height - 2*radius) {
                position.y = height - 2*radius;
                // Only bounce if falling with enough velocity
                if (-velocity.y > min_bounce_velocity) {
                    velocity.y *= -floor_damping;
                } else {
                    velocity.y = 0;  // Stop vertical movement if too slow
                }
                previous_position.y = position.y - velocity.y;
            }
        }
};

#endif