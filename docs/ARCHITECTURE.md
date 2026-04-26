# Assignment 4: Reused Patterns & Architecture

## Patterns Inherited from Previous Assignments

This document outlines how Assignment 4 leverages and extends the established patterns from the 2D Solar System and Bouncing Ball projects.

---

## 1. Timer-Driven Animation Loop

### Original Pattern (Solar System & Bouncing Ball)
```cpp
void timer(int value) {
    if (condition)
        update_state();
    
    glutPostRedisplay();
    glutTimerFunc(period_ms, timer, value + 1);
}
```

### Assignment 4 Implementation
- **File:** [src/main.cpp](src/main.cpp#L51-L67)
- **Function:** `timer(int value)`
- **Period:** 16 ms (≈60 FPS, same as Assignment 3)
- **Behavior:** Self-rescheduling timer calls `game->update()` and posts redisplay
- **Advantage:** Proven frame-rate control, smooth animation

---

## 2. GLUT Callback Architecture

### Pattern Reuse
```cpp
// Input handling
glutKeyboardFunc(keyboard)              // ASCII keys
glutSpecialFunc(specialKeyboard)        // Arrow keys, function keys

// Display & reshape
glutDisplayFunc(display)
glutReshapeFunc(reshape)

// Animation
glutTimerFunc(period, timer, 0)
```

### Assignment 4 Callbacks
- **keyboard()** → Handles Q/ESC (quit), R (reset)
- **specialKeyboard()** → Handles LEFT/RIGHT arrows for car movement
- **display()** → Calls `game->render()`
- **reshape()** → Updates viewport dimensions
- **timer()** → Calls `game->update()` with fixed delta time

---

## 3. State Management Pattern

### Original (Bouncing Ball)
```cpp
static int isAnimate = 0;
static float g_time = 0.0f;
static bool collisionState = false;

void reset() {
    isAnimate = 0;
    g_time = 0.0f;
    collisionState = false;
    // ... restore all state
}
```

### Assignment 4 Class-Based Approach
- **Game class** encapsulates all state (not globals)
- **Car class** tracks: `currentLane`, position
- **Obstacle class** tracks: `lane`, `position`, `velocity`
- **Game::reset()** atomically restores all objects
- **Benefit:** Cleaner scoping, easier to extend

```cpp
// Game.h
class Game {
private:
    Car car;
    std::vector<Obstacle> obstacles;
    bool collisionOccurred;
    float collisionDisplayTimer;
    
public:
    void reset();  // Full state reset
};
```

---

## 4. Keyboard Input Handling

### Pattern (Solar System)
```cpp
case '+': case '=':
    g_simSpeed += SPEED_STEP;
    break;
case 'r': case 'R':
    g_simSpeed = 5.0f;
    g_time = 0.0f;
    break;
```

### Assignment 4 Extension
- **Game::handleKeyPress()** encapsulates logic
- **Supports:** Q (quit), R (reset), ESC (quit)
- **Special keys:** LEFT/RIGHT arrows via `specialKeyboard()`
- **Immediate response:** Arrow keys trigger `car.moveLeft()/moveRight()`

```cpp
// src/main.cpp
void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        g_game->getCar().moveLeft();
    else if (key == GLUT_KEY_RIGHT)
        g_game->getCar().moveRight();
    glutPostRedisplay();
}
```

---

## 5. Text Rendering (HUD)

### Original (Bouncing Ball)
```cpp
void drawText2D(float x, float y, void* f, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c; ++c)
        glutBitmapCharacter(f, *c);
}
```

### Assignment 4 Reuse
- **File:** [src/Primitives.cpp](src/Primitives.cpp#L52-L61)
- **Function:** `drawText2D(float x, float y, const char* text, bool largeFont)`
- **Usage:** HUD overlay, collision message display
- **Fonts:** `GLUT_BITMAP_HELVETICA_12` (small), `GLUT_BITMAP_HELVETICA_18` (large)
- **Rendering:** 2D orthographic overlay on top of 3D scene

---

## 6. Transparency Blending

### Original (Solar System & Bouncing Ball)
```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glColor4f(r, g, b, alpha);
// draw geometry
```

### Assignment 4 Usage
- **Windshield:** Semi-transparent blue on car
- **HUD backgrounds:** Semi-transparent black for readability
- **Collision overlay:** Semi-transparent red for emphasis

```cpp
// Car windshield (Car.cpp)
glColor4f(0.5f, 0.7f, 1.0f, 0.5f);
glBegin(GL_QUADS);
    // draw windshield
glEnd();

// HUD background (Game.cpp)
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
drawRect2D(10.0f, 10.0f, 300.0f, 120.0f);
```

---

## 7. Collision Detection Logic

### Original (Bouncing Ball Physics)
```cpp
if (currentY <= 0.0f && t > 0.0f) {
    float impactVelocity = currentVelocityY - effectiveG * t;
    float newVelocity = -impactVelocity * restitution;
    // handle bounce
}
```

### Assignment 4 Simplification
- **3D bounding box collision** instead of continuous physics
- **Lane matching:** `car.lane == obstacle.lane`
- **Distance check:** `|car.Z - obstacle.Z| < COLLISION_THRESHOLD`
- **Response:** Set `collisionOccurred = true`, start display timer

```cpp
// Game.cpp
void Game::checkCollisions() {
    for (const auto& obs : obstacles) {
        if (car.getCurrentLane() != obs.getLane())
            continue;
        
        float zDiff = std::fabs(car.getZ() - obs.getZ());
        if (zDiff < COLLISION_THRESHOLD) {
            collisionOccurred = true;
            collisionDisplayTimer = COLLISION_DISPLAY_DURATION;
            return;
        }
    }
}
```

---

## 8. Object Recycling Pattern

### Original (Bouncing Ball)
```cpp
// Ball bounces with energy loss, eventually stops
if (fabs(newVelocityY) < 0.1f) {
    isStopped = 1;  // mark as stopped
}
```

### Assignment 4: Obstacle Recycling
- **Off-screen detection:** `obstacle.isOffScreen()` when `z > 10.0f`
- **Respawn mechanism:** Recycle furthest obstacle to rear spawn zone
- **Random distribution:** Assign random lane on respawn
- **Continuous spawning:** Maintains 10 obstacles in play

```cpp
// Game.cpp::updateObstacles()
if (obs.isOffScreen()) {
    float minZ = obstacles[0].getZ();
    int minIdx = 0;
    // find obstacle with smallest Z
    for (size_t i = 1; i < obstacles.size(); i++) {
        if (obstacles[i].getZ() < minZ) {
            minZ = obstacles[i].getZ();
            minIdx = i;
        }
    }
    int randomLane = rand() % Car::NUM_LANES;
    obstacles[minIdx].respawn(randomLane, INITIAL_SPAWN_Z);
}
```

---

## 9. Viewport Management

### New for Assignment 4: Dual Viewports
- **Left viewport (2/3):** 3D perspective rear-view camera
- **Right viewport (1/3):** Orthographic top-down camera
- **Simultaneous rendering:** Both rendered each frame with separate `glViewport()` calls

```cpp
// Game.cpp::renderRearView()
glViewport(0, 0, 800 * 2 / 3, 600);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(60.0f, aspect, 0.1f, 200.0f);
// ... render 3D scene

// Game.cpp::renderTopDownView()
glViewport(800 * 2 / 3, 0, 800 * 1 / 3, 600);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-10.0f, 10.0f, -50.0f, 50.0f, -100.0f, 100.0f);
// ... render top-down scene
```

---

## 10. 3D Primitive Rendering

### Original (Solar System)
```cpp
void drawCircle(float cx, float cy, float radius, int segments = 64) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i) {
        float angle = (float)(2.0 * M_PI * i / segments);
        glVertex2f(cx + radius * cosf(angle),
                   cy + radius * sinf(angle));
    }
    glEnd();
}
```

### Assignment 4: 3D Boxes & Spheres
- **drawBox()** → Creates axis-aligned boxes (car, obstacles)
- **drawSphere()** → Uses `glutSolidSphere()` for wheels
- **drawRoad()** → Road surface with lane markings
- **drawRect2D()** → 2D rectangles for HUD backgrounds

```cpp
// Primitives.cpp
void drawBox(float width, float height, float depth) {
    glBegin(GL_QUADS);
        // front, back, left, right, top, bottom faces
    glEnd();
}
```

---

## 11. Project Structure & Build System

### Consistency with Previous Assignments

| Aspect | Pattern |
|--------|---------|
| **Solution Layout** | Single `.sln` with nested folder structure |
| **Project File** | MSVC v143, Console subsystem, Unicode charset |
| **Include Dirs** | `$(ProjectDir)include` relative path |
| **Source Org** | `src/` for `.cpp`, `include/` for `.h` |
| **Compiler** | Visual Studio 2022 (v143) |
| **Target Platforms** | Win32, x64 (Debug/Release) |
| **Libraries** | GLEW (static), FreeGLUT, OpenGL32, GLU32 |

### File Structure
```
Assignment_4/
├── include/
│   └── *.h
├── src/
│   └── *.cpp
├── Assignment_4/
│   ├── Assignment_4.vcxproj
│   └── Assignment_4.vcxproj.filters
├── Assignment_4.sln
├── README.md
└── BUILD_SETUP.md
```

---

## Summary: Key Adaptations

| Original Pattern | Assignment 4 Evolution |
|------------------|------------------------|
| Global state variables | Encapsulated in `Game` class |
| Single-file implementation | Multi-file modular architecture |
| 2D rendering | 3D with dual viewports |
| Simple physics | Collision detection + object recycling |
| Fixed obstacles | Dynamic obstacle spawning & cycling |
| One viewport | Split-screen visualization |
| GLUT primitives | 3D boxes + spheres for game objects |

---

## Building on Prior Knowledge

1. **Assignment 1 (Spaceship Game):** Object-oriented structure, HUD overlay techniques
2. **2D Solar System:** Timer animation, smooth camera control, text rendering
3. **Assignment 3 (Bouncing Ball):** Collision detection, state reset logic, parameter management

**Assignment 4 synthesizes all three**, adding:
- 3D perspective rendering
- Dual viewport orchestration
- Object spawning/recycling management
- Lane-based game mechanics

---

*This architecture is designed to be extensible for future features like scoring, difficulty progression, particle effects, and audio.*
