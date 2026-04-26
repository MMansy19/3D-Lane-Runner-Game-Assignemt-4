// ============================================================================
// ASSIGNMENT 4: 3D LANE RUNNER GAME
// OpenGL + FreeGLUT + GLEW
// ============================================================================

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>
#include <ctime>

#include "Game.h"

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

static Game* g_game = nullptr;          // Single game instance
static int g_windowWidth = 800;         // Window width in pixels
static int g_windowHeight = 600;        // Window height in pixels
static int g_animationPeriod = 16;      // Timer interval in milliseconds (16ms = 60 FPS)
static int g_lastUpdateTimeMs = 0;      // Last timestamp used for delta-time updates

// ============================================================================
// GLUT CALLBACK FUNCTIONS
// ============================================================================

// Called whenever the window needs to be redrawn
// This is where we render our game graphics each frame
void display()
{
    if (g_game) {
        g_game->render();  // Tell game to render the scene
    }
}

// Called when the window is resized
// Updates viewport to match new window dimensions
void reshape(int w, int h)
{
    if (h == 0) h = 1;  // Prevent division by zero
    g_windowWidth = w;
    g_windowHeight = h;
    if (g_game) {
        g_game->setWindowSize(w, h);
    }
    glViewport(0, 0, w, h);  // Set OpenGL viewport to new size
}

// Called every 16 milliseconds by glutTimerFunc
// This is our main animation/game loop - updates game state each frame
void timer(int value)
{
    if (g_game) {
        int nowMs = glutGet(GLUT_ELAPSED_TIME);
        float deltaTime = (nowMs - g_lastUpdateTimeMs) / 1000.0f;
        g_lastUpdateTimeMs = nowMs;

        // Clamp delta-time to avoid huge jumps if window was paused/minimized.
        if (deltaTime < 0.0f) deltaTime = 0.0f;
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        g_game->update(deltaTime);  // Update game logic with elapsed time
    }

    glutPostRedisplay();  // Request window to be redrawn
    // Reschedule this timer to run again in 16ms
    glutTimerFunc((unsigned int)g_animationPeriod, timer, value + 1);
}

// Called when ASCII key is pressed (letters, numbers, special chars)
void keyboard(unsigned char key, int x, int y)
{
    // Let the game handle the keyboard input
    if (g_game) {
        // Support A/D as alternative lane controls.
        if (key == 'a' || key == 'A') {
            g_game->getCar().moveLeft();
        }
        else if (key == 'd' || key == 'D') {
            g_game->getCar().moveRight();
        }

        g_game->handleKeyPress(key);
        glutPostRedisplay();  // Redraw after keyboard input
    }
}

// Called when special keys are pressed (arrow keys, function keys, etc.)
void specialKeyboard(int key, int x, int y)
{
    if (g_game) {
        // LEFT arrow: move car to the left lane
        if (key == GLUT_KEY_LEFT) {
            g_game->getCar().moveLeft();
            glutPostRedisplay();
        }
        // RIGHT arrow: move car to the right lane
        else if (key == GLUT_KEY_RIGHT) {
            g_game->getCar().moveRight();
            glutPostRedisplay();
        }
    }
}

// ============================================================================
// MAIN FUNCTION - PROGRAM ENTRY POINT
// ============================================================================

int main(int argc, char** argv)
{
    // STEP 1: Seed random number generator for obstacle random lanes
    srand((unsigned)time(nullptr));

    // STEP 2: Initialize GLUT (Graphics Utility Toolkit)
    //         This sets up the windowing system
    glutInit(&argc, argv);
    
    // Set OpenGL context: version 4.3, compatibility profile (allows older features)
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    
    // Set display mode: double buffering (smooth animation) + RGB color + depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Set initial window size
    glutInitWindowSize(g_windowWidth, g_windowHeight);
    
    // Set initial window position on screen
    glutInitWindowPosition(100, 100);
    
    // Create the window with title
    glutCreateWindow("3D Lane Runner Game - Assignment 4");

    // STEP 3: Initialize GLEW (OpenGL Extension Wrangler)
    //         Loads all modern OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // STEP 4: Set up OpenGL rendering settings
    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);  // Sky blue background
    glEnable(GL_DEPTH_TEST);                // Enable 3D depth sorting
    // Fixed-function lighting is disabled because our immediate-mode meshes
    // do not provide normals, which makes colors look very dark or inconsistent.
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_BLEND);                     // Enable transparency blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // How to blend transparent colors

    // STEP 6: Create our game instance
    g_game = new Game();
    g_game->setWindowSize(g_windowWidth, g_windowHeight);
    g_lastUpdateTimeMs = glutGet(GLUT_ELAPSED_TIME);

    // STEP 7: Register GLUT callback functions
    //         These functions are called by GLUT when events occur
    glutDisplayFunc(display);          // Called when window needs redrawing
    glutReshapeFunc(reshape);          // Called when window is resized
    glutKeyboardFunc(keyboard);        // Called when ASCII key is pressed
    glutSpecialFunc(specialKeyboard);  // Called when special key is pressed

    // STEP 8: Start the game animation timer
    //         This fires every 16ms to update the game
    glutTimerFunc((unsigned int)g_animationPeriod, timer, 0);

    // STEP 9: Print instructions to console
    printf("===========================================\n");
    printf("  3D LANE RUNNER GAME (Assignment 4)\n");
    printf("===========================================\n");
    printf("\nControls:\n");
    printf("  LEFT/RIGHT arrows  - Move car left/right\n");
    printf("  R                  - Reset game\n");
    printf("  Q/ESC              - Quit\n");
    printf("\nObjective:\n");
    printf("  Avoid obstacles moving toward you!\n");
    printf("  When collision occurs, press R to restart.\n");
    printf("===========================================\n\n");

    // STEP 10: Enter GLUT event loop
    //          This runs forever, processing events and calling callbacks
    glutMainLoop();

    // STEP 11: Cleanup (only reached if window is closed)
    delete g_game;
    g_game = nullptr;

    return 0;
}
