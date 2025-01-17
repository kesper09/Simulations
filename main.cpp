#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include"particle.h"
#include"input_handler.h"

const int WIDTH = 1080;
const int HEIGHT = 640;
const float PARTICLE_RADIUS = 5.0f;
const float GRAVITY = 0.8f;
const float TIME_STEP = 1.0f / 8.0f;

const int ROW = 15;
const int COLUMN = 15;
const float REST_DISTANCE = 25.0f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Simulation");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;
    
    float startX = (WIDTH - (COLUMN - 1) * REST_DISTANCE) / 2;
    float startY = 50.0f;

    // Create particles
    for(int row = 0; row < ROW; row++) {
        for(int col = 0; col < COLUMN; col++) {
            float x = startX + col * REST_DISTANCE;
            float y = startY + row * REST_DISTANCE;
            bool pinned = (row == 0 && (col == 0 || col == COLUMN-1));
            particles.emplace_back(x, y, pinned);
        }
    }

    // Create constraints
    for(int row = 0; row < ROW; row++) {
        for(int col = 0; col < COLUMN; col++) {
            if(col < COLUMN - 1) {
                constraints.emplace_back(&particles[row * COLUMN + col], 
                                      &particles[row * COLUMN + col + 1]);
            }
            if(row < ROW - 1) {
                constraints.emplace_back(&particles[row * COLUMN + col],
                                      &particles[(row + 1) * COLUMN + col]);
            }
        }
    }

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            InputHandler::handle_mouse_click(event, particles, constraints);
        }

        // Solve constraints multiple times for stability
        for(size_t i = 0; i < 10; i++) {  // Increased iterations for better stability
            for(auto& constraint : constraints) {
                if(constraint.active) {
                    constraint.satisfy();
                }
            }
        }

        // Check if particles are connected to any active constraints
        std::vector<bool> has_active_connection(particles.size(), false);
        for(const auto& constraint : constraints) {
            if(constraint.active) {
                int idx1 = constraint.p1 - &particles[0];
                int idx2 = constraint.p2 - &particles[0];
                has_active_connection[idx1] = true;
                has_active_connection[idx2] = true;
            }
        }

        // Update particles
        for(size_t i = 0; i < particles.size(); i++) {
            auto& particle = particles[i];
            if(!particle.is_pinned) {
                // Apply stronger gravity to disconnected particles
                float gravity_multiplier = has_active_connection[i] ? 1.0f : 1.5f;
                particle.apply_force(sf::Vector2f(0, GRAVITY * gravity_multiplier));
                particle.update(TIME_STEP);
                particle.constrain_to_bounds(WIDTH, HEIGHT);
            }
        }

        window.clear(sf::Color::Black);

        // Draw constraints
        for(const auto& constraint : constraints) {
            if(constraint.active) {
                sf::Vertex lines[] = {
                    sf::Vertex(constraint.p1->position, sf::Color::White),
                    sf::Vertex(constraint.p2->position, sf::Color::White)
                };
                window.draw(lines, 2, sf::Lines);
            }
        }

        // Draw particles
        for(size_t i = 0; i < particles.size(); i++) {
            sf::Color color = has_active_connection[i] ? 
                             sf::Color::White : 
                             sf::Color(255, 100, 100); // Reddish for disconnected particles
            sf::Vertex point(particles[i].position, color);
            window.draw(&point, 1, sf::Points);
        }

        window.display();
    }
    return 0;
}