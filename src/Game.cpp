// ============================================================================
// GAME CLASS IMPLEMENTATION
// Main game controller - handles logic, rendering, and state management
// ============================================================================

#include "Game.h"
#include "Primitives.h"
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

// ============================================================================
// CONSTRUCTOR
// ============================================================================

// Initialize the game with car and obstacles
Game::Game()
    : timeSinceLastSpawn(0.0f), collisionOccurred(false), collisionDisplayTimer(0.0f),
      windowWidth(800), windowHeight(600)
{
    // Create all 10 obstacles at starting positions
    // Spread them out along the Z axis so they don't all hit at once
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        // Calculate starting Z: each obstacle starts further back
        float spawnZ = INITIAL_SPAWN_Z - i * (SPAWN_INTERVAL * OBSTACLE_SPEED);
        
        // Distribute obstacles across 3 lanes
        int lane = i % Car::NUM_LANES;  // 0,1,2,0,1,2,...
        
        // Create obstacle at this lane and Z position
        obstacles.emplace_back(lane, spawnZ);
    }
}

// ============================================================================
// MAIN UPDATE FUNCTION - Called every frame
// ============================================================================

// Update game state: obstacles, collisions, timers
void Game::update(float deltaTime)
{
    // If collision happened, decrement the display timer
    if (collisionOccurred) {
        collisionDisplayTimer -= deltaTime;
        
        // If timer runs out, clear collision state
        if (collisionDisplayTimer <= 0.0f) {
            collisionOccurred = false;
            collisionDisplayTimer = 0.0f;
        }

        // Freeze world movement while collision message is active.
        return;
    }

    // Move all obstacles and recycle off-screen ones
    updateObstacles(deltaTime);
    
    // Check if car hit any obstacle
    checkCollisions();
}

void Game::setWindowSize(int width, int height)
{
    windowWidth = (width > 0) ? width : 1;
    windowHeight = (height > 0) ? height : 1;
}

// ============================================================================
// UPDATE OBSTACLES - Move them and recycle when off-screen
// ============================================================================

void Game::updateObstacles(float deltaTime)
{
    // Move each obstacle toward the camera (positive Z direction)
    for (auto& obs : obstacles) {
        obs.update(deltaTime, OBSTACLE_SPEED);
    }

    // Recycle off-screen obstacles (respawn them at the back)
    for (auto& obs : obstacles) {
        // Check if obstacle has passed the camera
        if (obs.isOffScreen()) {
            // Find the obstacle with the smallest (furthest back) Z value
            float minZ = obstacles[0].getZ();
            size_t minIdx = 0;
            
            for (size_t i = 1; i < obstacles.size(); i++) {
                if (obstacles[i].getZ() < minZ) {
                    minZ = obstacles[i].getZ();
                    minIdx = i;
                }
            }
            
            // Respawn the furthest obstacle in a random lane
            int randomLane = rand() % Car::NUM_LANES;
            float newSpawnZ = minZ - (SPAWN_INTERVAL * OBSTACLE_SPEED);
            obstacles[minIdx].respawn(randomLane, newSpawnZ);
        }
    }
}

// ============================================================================
// COLLISION DETECTION
// ============================================================================

void Game::checkCollisions()
{
    // Check each obstacle for collision with the car
    for (const auto& obs : obstacles) {
        
        // First check: is obstacle in the same lane as car?
        if (car.getCurrentLane() != obs.getLane())
            continue;  // Different lanes, no collision possible
        
        // Second check: is obstacle close enough to car on Z axis?
        // Calculate distance between car and obstacle
        float zDiff = std::fabs(car.getZ() - obs.getZ());
        
        // If they're within collision distance, collision detected!
        if (zDiff < 2.0f) {  // 2.0 units = collision threshold
            collisionOccurred = true;
            collisionDisplayTimer = 2.0f;  // Show message for 2 seconds
            return;  // Exit immediately (only one collision per frame)
        }
    }
}

// ============================================================================
// HANDLE KEYBOARD INPUT
// ============================================================================

void Game::handleKeyPress(unsigned char key)
{
    switch (key) {
    case 'q':   // Q key
    case 'Q':   // q key
    case 27:    // ESC key (ASCII code 27)
        exit(0);  // Quit the game
        break;
        
    case 'r':   // R key
    case 'R':   // r key
        reset();  // Reset game to initial state
        break;
    }
}

// ============================================================================
// RESET GAME
// ============================================================================

// Full reset: restore all game state to initial condition
void Game::reset()
{
    // Reset the car
    car.resetPosition();  // Move car back to center lane
    
    // Clear collision state
    collisionOccurred = false;
    collisionDisplayTimer = 0.0f;
    timeSinceLastSpawn = 0.0f;

    // Reset all obstacles to starting positions
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        float spawnZ = INITIAL_SPAWN_Z - i * (SPAWN_INTERVAL * OBSTACLE_SPEED);
        int lane = i % Car::NUM_LANES;
        obstacles[i].respawn(lane, spawnZ);
    }
}

// ============================================================================
// MAIN RENDERING FUNCTION
// ============================================================================

// Render the entire game frame
void Game::render()
{
    // Clear the screen (set to background color)
    glClearColor(0.15f, 0.45f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Render two different views side-by-side
    renderRearView();        // Left 2/3 of screen - 3D rear view
    renderTopDownView();     // Right 1/3 of screen - top-down view
    
    // Render HUD overlay (controls, info)
    renderHUD();
    
    // If collision occurred, show collision message
    if (collisionOccurred) {
        renderCollisionMessage();
    }

    glutSwapBuffers();  // Swap front and back buffers (double buffering)
}

// ============================================================================
// REAR VIEW - 3D perspective camera view from behind the car
// ============================================================================

void Game::renderRearView() const
{
    int rearWidth = windowWidth * 2 / 3;

    // Set viewport to left 2/3 of screen
    glViewport(0, 0, rearWidth, windowHeight);
    
    // Set up perspective projection (3D view)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f,                    // 60 degree field of view
                   static_cast<float>(rearWidth) / static_cast<float>(windowHeight),
                   0.1f,                     // Near clipping plane
                   200.0f);                  // Far clipping plane

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position camera behind and above the car
    float camX = car.getX();
    float camY = car.getY() + 3.0f;  // 3 units above car
    float camZ = car.getZ() + 5.0f;  // 5 units behind car
    
    // Look at the car slightly ahead
    float lookX = car.getX();
    float lookY = car.getY();
    float lookZ = car.getZ() - 10.0f;

    // Set camera position and direction
    gluLookAt(camX, camY, camZ,     // Camera position
              lookX, lookY, lookZ,   // Look-at position
              0.0f, 1.0f, 0.0f);     // Up vector (Y axis)

    // Draw the scene
    // Draw road
    glColor3f(0.5f, 0.5f, 0.5f);
    drawRoad(100.0f, 15.0f, 20);
    
    // Draw the car
    car.draw();
    
    // Draw all obstacles
    for (const auto& obs : obstacles) {
        obs.draw();
    }
}

// ============================================================================
// TOP-DOWN VIEW - Orthographic (2D-like) camera looking straight down
// ============================================================================

void Game::renderTopDownView() const
{
    int rearWidth = windowWidth * 2 / 3;
    int miniMapWidth = windowWidth - rearWidth;

    // Set viewport to right 1/3 of screen
    glViewport(rearWidth, 0, miniMapWidth, windowHeight);
    
    // Set up orthographic projection (no perspective)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0f,  // Left boundary
            10.0f,   // Right boundary
            -50.0f,  // Bottom boundary
            50.0f,   // Top boundary
            -100.0f, // Near plane
            100.0f); // Far plane

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position camera directly above car
    gluLookAt(0.0f,              // Camera X (center)
              30.0f,             // Camera Y (high above)
              car.getZ(),        // Camera Z (track with car)
              0.0f,              // Look at X (center)
              0.0f,              // Look at Y (ground)
              car.getZ(),        // Look at Z (track with car)
              0.0f, 0.0f, -1.0f); // Up vector

    // Draw the scene (do not clear here, or rear viewport gets erased)
    // Draw road
    glColor3f(0.5f, 0.5f, 0.5f);
    drawRoad(100.0f, 15.0f, 20);
    
    // Draw the car
    car.draw();
    
    // Draw all obstacles
    for (const auto& obs : obstacles) {
        obs.draw();
    }
}

// ============================================================================
// HUD OVERLAY - Display game controls and info
// ============================================================================

void Game::renderHUD() const
{
    const float hudW = static_cast<float>(windowWidth);
    const float hudH = static_cast<float>(windowHeight);

    // Save 3D projection and switch to 2D screen space
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, hudW, 0.0f, hudH, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw semi-transparent black background for text readability
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);  // Black with 50% opacity
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawRect2D(12.0f, 12.0f, 360.0f, 138.0f);  // Semi-transparent box
    glDisable(GL_BLEND);

    // Draw text on top of background
    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    drawText2D(24.0f, 126.0f, "3D Lane Runner", true);
    drawText2D(24.0f, 104.0f, "LEFT/RIGHT or A/D - Move", false);
    drawText2D(24.0f, 84.0f, "R - Reset", false);
    drawText2D(24.0f, 64.0f, "Q/ESC - Quit", false);
    drawText2D(24.0f, 44.0f, "Avoid yellow obstacles", false);

    // Restore 3D projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ============================================================================
// COLLISION MESSAGE - Display "COLLISION!" when crash occurs
// ============================================================================

void Game::renderCollisionMessage() const
{
    const float hudW = static_cast<float>(windowWidth);
    const float hudH = static_cast<float>(windowHeight);
    const float boxW = 420.0f;
    const float boxH = 150.0f;
    const float boxX = (hudW - boxW) * 0.5f;
    const float boxY = (hudH - boxH) * 0.5f;

    // Switch to 2D screen space
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, hudW, 0.0f, hudH, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw semi-transparent red background (warning/error color)
    glColor4f(1.0f, 0.0f, 0.0f, 0.7f);  // Red with 70% opacity
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawRect2D(boxX, boxY, boxW, boxH);  // Centered box
    glDisable(GL_BLEND);

    // Draw collision message text
    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    drawText2D(boxX + 130.0f, boxY + 92.0f, "COLLISION!", true);
    drawText2D(boxX + 95.0f, boxY + 55.0f, "Press R to reset", true);

    // Restore 3D projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
