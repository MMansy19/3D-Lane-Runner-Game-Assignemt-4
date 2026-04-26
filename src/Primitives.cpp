// ============================================================================
// PRIMITIVES - REUSABLE DRAWING FUNCTIONS
// Simple utility functions for drawing 3D and 2D shapes
// ============================================================================

#include "Primitives.h"
#include <GL/glut.h>
#include <cstdio>

// Draw a filled 3D box (cube) at the current position
// Used for drawing rectangles with width, height, and depth
void drawBox(float width, float height, float depth)
{
    glBegin(GL_QUADS);  // Draw using quadrilaterals
    
        // Front face
        glVertex3f(-width/2, 0.0f, depth/2);
        glVertex3f(width/2, 0.0f, depth/2);
        glVertex3f(width/2, height, depth/2);
        glVertex3f(-width/2, height, depth/2);

        // Back face
        glVertex3f(-width/2, 0.0f, -depth/2);
        glVertex3f(-width/2, height, -depth/2);
        glVertex3f(width/2, height, -depth/2);
        glVertex3f(width/2, 0.0f, -depth/2);

        // Left side
        glVertex3f(-width/2, 0.0f, -depth/2);
        glVertex3f(-width/2, 0.0f, depth/2);
        glVertex3f(-width/2, height, depth/2);
        glVertex3f(-width/2, height, -depth/2);

        // Right side
        glVertex3f(width/2, 0.0f, -depth/2);
        glVertex3f(width/2, height, -depth/2);
        glVertex3f(width/2, height, depth/2);
        glVertex3f(width/2, 0.0f, depth/2);

        // Top
        glVertex3f(-width/2, height, -depth/2);
        glVertex3f(width/2, height, -depth/2);
        glVertex3f(width/2, height, depth/2);
        glVertex3f(-width/2, height, depth/2);

        // Bottom
        glVertex3f(-width/2, 0.0f, -depth/2);
        glVertex3f(-width/2, 0.0f, depth/2);
        glVertex3f(width/2, 0.0f, depth/2);
        glVertex3f(width/2, 0.0f, -depth/2);
    
    glEnd();
}

// Draw a filled 3D sphere (ball)
// radius: size of the sphere
// segments: number of divisions (more = smoother, but slower)
void drawSphere(float radius, int segments)
{
    glutSolidSphere(radius, segments, segments);
}

// Draw a filled 2D rectangle in screen space
// Used for HUD overlays and backgrounds
void drawRect2D(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

// Draw text in 2D screen space (for HUD display)
// x, y: screen position (bottom-left origin)
// text: string to display
// largeFont: if true, use 18pt font; else 12pt
void drawText2D(float x, float y, const char* text, bool largeFont)
{
    glRasterPos2f(x, y);  // Set text position
    
    // Choose font size
    void* font = largeFont ? GLUT_BITMAP_HELVETICA_18 : GLUT_BITMAP_HELVETICA_12;
    
    // Draw each character in the string
    for (const char* c = text; *c; ++c)
        glutBitmapCharacter(font, *c);
}

// Draw a road with lane markings
// length: how long the road is (Z direction)
// width: how wide the road is (X direction)  
// numDashes: number of white dashes to draw
void drawRoad(float length, float width, int numDashes)
{
    // === DRAW ROAD SURFACE ===
    glColor3f(0.5f, 0.5f, 0.5f);  // Grey color
    glBegin(GL_QUADS);
        glVertex3f(-width/2, -0.01f, -length/2);
        glVertex3f(width/2, -0.01f, -length/2);
        glVertex3f(width/2, -0.01f, length/2);
        glVertex3f(-width/2, -0.01f, length/2);
    glEnd();

    // === DRAW LANE MARKINGS (WHITE DASHES) ===
    glColor3f(1.0f, 1.0f, 1.0f);  // White color
    float dashSpacing = length / numDashes;
    
    // X positions for left and right lane dividers
    float laneX1 = -width / 3.0f;  // Left divider (between left and center lanes)
    float laneX2 = width / 3.0f;   // Right divider (between center and right lanes)

    glLineWidth(2.0f);  // Thicker lines for visibility
    glBegin(GL_LINES);
        // Draw dashes for each lane divider
        for (int i = 0; i < numDashes; i++) {
            float z = -length/2 + i * dashSpacing;
            float nextZ = z + dashSpacing * 0.5f;  // Dash is half of spacing

            // Left lane divider dashes
            glVertex3f(laneX1, 0.0f, z);
            glVertex3f(laneX1, 0.0f, nextZ);

            // Right lane divider dashes
            glVertex3f(laneX2, 0.0f, z);
            glVertex3f(laneX2, 0.0f, nextZ);
        }
    glEnd();
    glLineWidth(1.0f);  // Reset line width
}
