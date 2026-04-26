#pragma once
#include "Car.h"
#include "Obstacle.h"
#include <vector>

// ─── Game state management and collision detection ────────────────────────

class Game
{
public:
    // Lifecycle
    Game();
    ~Game() = default;

    // Update and render
    void update(float deltaTime);
    void render();

    // Input handling
    void handleKeyPress(unsigned char key);
    void setWindowSize(int width, int height);

    // Game state queries
    bool isCollided() const { return collisionOccurred; }
    void reset();

    // Car and obstacle access
    Car& getCar() { return car; }
    const Car& getCar() const { return car; }
    std::vector<Obstacle>& getObstacles() { return obstacles; }

    // Constants
    static constexpr int NUM_OBSTACLES = 10;
    static constexpr float OBSTACLE_SPEED = 14.0f; // units/sec
    static constexpr float INITIAL_SPAWN_Z = -50.0f;
    static constexpr float SPAWN_INTERVAL = 2.4f;  // spacing factor for initial layout

private:
    Car car;
    std::vector<Obstacle> obstacles;
    
    float timeSinceLastSpawn;
    bool collisionOccurred;
    float collisionDisplayTimer;
    int windowWidth;
    int windowHeight;
    
    static constexpr float COLLISION_DISPLAY_DURATION = 2.0f; // seconds

    // Helper methods
    void checkCollisions();
    void spawnObstacle();
    void updateObstacles(float deltaTime);
    void renderRearView() const;
    void renderTopDownView() const;
    void renderHUD() const;
    void renderCollisionMessage() const;
};
