// ============================================================================
// OBSTACLE CLASS IMPLEMENTATION  
// Represents an enemy obstacle moving toward the player
// ============================================================================

#include "Obstacle.h"
#include <GL/glut.h>
#include <cmath>

// Constructor: Create an obstacle in a specific lane at a starting distance
Obstacle::Obstacle(int lane, float startZ)
    : lane(lane), x(laneToX(lane)), y(0.0f), z(startZ), startZ(startZ)
{
}

// Convert lane number to X position  
// Lane 0 (left)   -> X = -3.0
// Lane 1 (center) -> X =  0.0
// Lane 2 (right)  -> X =  3.0
float Obstacle::laneToX(int lane) const
{
    float baseX = (lane - 1) * LANE_SPACING;
    return baseX;
}

// Update: Move obstacle toward the player
// deltaTime: time since last frame (in seconds)
// speed: how fast obstacle moves (units per second)
void Obstacle::update(float deltaTime, float speed)
{
    // Move obstacle closer to camera (increase Z toward positive)
    z += speed * deltaTime;
}

// Check if obstacle has passed the camera (off-screen)
// Returns true if obstacle should be recycled
bool Obstacle::isOffScreen() const
{
    // If Z > 10, obstacle is past camera - recycle it
    return z > 10.0f;
}

// Recycle obstacle: Move it to a new lane and reset to starting position
void Obstacle::respawn(int newLane, float newStartZ)
{
    lane = newLane;
    x = laneToX(newLane);   // Update X for new lane
    z = newStartZ;          // Put back at far distance
    startZ = newStartZ;
    y = 0.0f;              // Ground level
}

// Draw the obstacle as a yellow cube
void Obstacle::draw() const
{
    glPushMatrix();             // Save current position
    glTranslatef(x, y, z);      // Move to obstacle's location

    // === DRAW OBSTACLE BODY (YELLOW CUBE) ===
    glColor3f(1.0f, 0.84f, 0.0f);  // Gold/yellow color
    glBegin(GL_QUADS);              // Draw using quadrilaterals
    
        // Front face
        glVertex3f(-OBSTACLE_SIZE/2, 0.0f, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, 0.0f, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);

        // Back face
        glVertex3f(-OBSTACLE_SIZE/2, 0.0f, -OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, 0.0f, -OBSTACLE_SIZE/2);

        // Left side
        glVertex3f(-OBSTACLE_SIZE/2, 0.0f, -OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, 0.0f, OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);

        // Right side
        glVertex3f(OBSTACLE_SIZE/2, 0.0f, -OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, 0.0f, OBSTACLE_SIZE/2);

        // Top face
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);

        // Bottom face
        glVertex3f(-OBSTACLE_SIZE/2, 0.0f, -OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, 0.0f, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, 0.0f, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, 0.0f, -OBSTACLE_SIZE/2);
    
    glEnd();

    // === DRAW OUTLINE ON TOP (DARKER YELLOW) ===
    // This makes the obstacle easier to see in 3D
    glColor3f(0.5f, 0.42f, 0.0f);  // Dark gold
    glLineWidth(2.0f);              // Thicker lines for visibility
    glBegin(GL_LINE_LOOP);          // Draw a wireframe loop
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, OBSTACLE_SIZE/2);
        glVertex3f(OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
        glVertex3f(-OBSTACLE_SIZE/2, OBSTACLE_SIZE, -OBSTACLE_SIZE/2);
    glEnd();
    glLineWidth(1.0f);  // Reset line width to normal

    glPopMatrix();  // Restore position
}
