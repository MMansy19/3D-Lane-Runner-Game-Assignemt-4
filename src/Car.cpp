// ============================================================================
// CAR CLASS IMPLEMENTATION
// Represents the player's vehicle in the game
// ============================================================================

#include "Car.h"
#include <GL/glut.h>
#include <cmath>

// Constructor: Initialize the car at the center lane
Car::Car() : currentLane(LANE_CENTER), x(0.0f), y(0.0f), z(CAR_START_Z)
{
    // Convert center lane (1) to X coordinate (0.0)
    x = laneToX(currentLane);
}

// Convert lane number to X position
// Lane 0 (left)   -> X = -3.0
// Lane 1 (center) -> X =  0.0
// Lane 2 (right)  -> X =  3.0
float Car::laneToX(int lane) const
{
    float baseX = (lane - 1) * LANE_SPACING;
    return baseX;
}

// Move the car one lane to the left
void Car::moveLeft()
{
    // Check if not already in leftmost lane
    if (currentLane > LANE_LEFT) {
        currentLane--;              // Decrease lane (0,1,2)
        x = laneToX(currentLane);   // Update X position
    }
}

// Move the car one lane to the right
void Car::moveRight()
{
    // Check if not already in rightmost lane
    if (currentLane < LANE_RIGHT) {
        currentLane++;              // Increase lane (0,1,2)
        x = laneToX(currentLane);   // Update X position
    }
}

// Reset car to starting position
void Car::resetPosition()
{
    currentLane = LANE_CENTER;     // Back to middle lane
    x = laneToX(currentLane);      // Update X coordinate
    y = 0.0f;                      // Ground level
    z = CAR_START_Z;               // Back to starting Z position
}

// Draw the car as a 3D object
void Car::draw() const
{
    glPushMatrix();             // Save current position/rotation
    glTranslatef(x, y, z);      // Move to car's location

    // === DRAW RED CAR BODY ===
    glColor3f(1.0f, 0.0f, 0.0f);  // Red color (R=1, G=0, B=0)
    glBegin(GL_QUADS);              // Draw using quadrilaterals
    
        // Front face
        glVertex3f(-CAR_SIZE/2, 0.0f, CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, 0.0f, CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, CAR_SIZE, CAR_SIZE/2);
        glVertex3f(-CAR_SIZE/2, CAR_SIZE, CAR_SIZE/2);

        // Back face
        glVertex3f(-CAR_SIZE/2, 0.0f, -CAR_SIZE/2);
        glVertex3f(-CAR_SIZE/2, CAR_SIZE, -CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, CAR_SIZE, -CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, 0.0f, -CAR_SIZE/2);

        // Left side
        glVertex3f(-CAR_SIZE/2, 0.0f, -CAR_SIZE/2);
        glVertex3f(-CAR_SIZE/2, 0.0f, CAR_SIZE/2);
        glVertex3f(-CAR_SIZE/2, CAR_SIZE, CAR_SIZE/2);
        glVertex3f(-CAR_SIZE/2, CAR_SIZE, -CAR_SIZE/2);

        // Right side
        glVertex3f(CAR_SIZE/2, 0.0f, -CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, CAR_SIZE, -CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, CAR_SIZE, CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, 0.0f, CAR_SIZE/2);

        // Top face
        glVertex3f(-CAR_SIZE/2, CAR_SIZE, -CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, CAR_SIZE, -CAR_SIZE/2);
        glVertex3f(CAR_SIZE/2, CAR_SIZE, CAR_SIZE/2);
        glVertex3f(-CAR_SIZE/2, CAR_SIZE, CAR_SIZE/2);
    
    glEnd();

    // === DRAW WINDSHIELD ===
    // Light blue with 50% transparency (alpha = 0.5)
    glColor4f(0.5f, 0.7f, 1.0f, 0.5f);
    glBegin(GL_QUADS);
        glVertex3f(-CAR_SIZE/2 + 0.1f, CAR_SIZE * 0.6f, CAR_SIZE/2 + 0.1f);
        glVertex3f(CAR_SIZE/2 - 0.1f, CAR_SIZE * 0.6f, CAR_SIZE/2 + 0.1f);
        glVertex3f(CAR_SIZE/2 - 0.1f, CAR_SIZE, CAR_SIZE/2 + 0.1f);
        glVertex3f(-CAR_SIZE/2 + 0.1f, CAR_SIZE, CAR_SIZE/2 + 0.1f);
    glEnd();

    // === DRAW FOUR WHEELS ===
    glColor3f(0.2f, 0.2f, 0.2f);    // Dark grey color
    float wheelRadius = CAR_SIZE * 0.2f;  // 20% of car size
    
    // Front-left wheel (sphere)
    glPushMatrix();
    glTranslatef(-CAR_SIZE/2 + 0.1f, wheelRadius, CAR_SIZE/2 - 0.1f);
    glutSolidSphere(wheelRadius, 16, 16);   // Draw smooth sphere
    glPopMatrix();

    // Front-right wheel
    glPushMatrix();
    glTranslatef(CAR_SIZE/2 - 0.1f, wheelRadius, CAR_SIZE/2 - 0.1f);
    glutSolidSphere(wheelRadius, 16, 16);
    glPopMatrix();

    // Back-left wheel
    glPushMatrix();
    glTranslatef(-CAR_SIZE/2 + 0.1f, wheelRadius, -CAR_SIZE/2 + 0.1f);
    glutSolidSphere(wheelRadius, 16, 16);
    glPopMatrix();

    // Back-right wheel
    glPushMatrix();
    glTranslatef(CAR_SIZE/2 - 0.1f, wheelRadius, -CAR_SIZE/2 + 0.1f);
    glutSolidSphere(wheelRadius, 16, 16);
    glPopMatrix();

    glPopMatrix();  // Restore position/rotation
}
