#include<bits/stdc++.h>
#include"particle.h"

class Constraint
{
    public:
        Particle *p1;
        Particle *p2;
        float initial_length;
        float stiffness;
        bool active;

        Constraint(Particle *p1, Particle *p2) : 
            p1(p1), 
            p2(p2),
            initial_length(std::hypot(p2->position.x - p1->position.x, p2->position.y - p1->position.y)),
            stiffness(0.9f),  // Slightly increased stiffness for better stability
            active(true) {}

        void satisfy() {
            if(!active) return;

            sf::Vector2f delta = p2->position - p1->position;
            float current_length = std::hypot(delta.x, delta.y);
            
            if (current_length > 0.0001f) {
                float difference = (current_length - initial_length) / current_length;
                sf::Vector2f correction = delta * difference * 0.5f * stiffness;
                
                // Apply corrections only to unpinned particles
                if(!p1->is_pinned) {
                    p1->position += correction;
                    p1->previous_position += correction * 0.95f;  // Increased damping for smoother motion
                }
                if(!p2->is_pinned) {
                    p2->position -= correction;
                    p2->previous_position -= correction * 0.95f;  // Increased damping for smoother motion
                }
            }
        }

        void deactivate()
        {
            active = false;
        }
};