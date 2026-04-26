# 3D Lane Runner Game – Assignment 4

A real-time 3D lane-based runner game built with **OpenGL (GLEW + FreeGLUT)** for the Computer Graphics course.

## 🎮 Game Overview

The player controls a red car that moves left and right between three lanes using arrow keys. Ten yellow obstacles move toward the car along the road. When a collision is detected (car and obstacle in the same lane at the same position), the screen displays **"COLLISION! Press R to reset"**. Pressing R fully resets the game state and restarts the animation.

### Dual Viewport System
- **Left 2/3 of screen:** Rear third-person 3D view of the car and road
- **Right 1/3 of screen:** Top-down view of the entire course and obstacle positions

---

## 🎯 Requirements Met

✅ **3D Environment** – Lane-based road with proper perspective  
✅ **Car Control** – Three lanes with left/right arrow key movement  
✅ **10 Obstacles** – Yellow cubes moving toward the car  
✅ **Collision Detection** – Detects same-lane, same-position collisions  
✅ **Collision Display** – "COLLISION!" message with "Press R to reset"  
✅ **Full Reset** – R key resets car position, obstacle positions, and all state  
✅ **Dual Viewports** – Rear 3D view + top-down view split screen  
✅ **Smooth Animation** – 60 FPS timer-driven loop matching Assignment 3 pattern  

---

## 📋 Architecture

### Core Classes

| Class | Purpose |
|-------|---------|
| `Car` | Player-controlled vehicle in a single lane |
| `Obstacle` | Enemy cubes moving down the road |
| `Game` | Game state, collision detection, rendering orchestration |
| `Primitives` | Utility functions for drawing 3D/2D elements |

### File Structure

```
Assignment_4/
├── include/
│   ├── Car.h              # Car class with lane management
│   ├── Obstacle.h         # Obstacle class with recycling
│   ├── Game.h             # Game state & collision logic
│   └── Primitives.h       # Drawing utilities
├── src/
│   ├── main.cpp           # GLUT setup and event loop
│   ├── Car.cpp            # Car implementation
│   ├── Obstacle.cpp       # Obstacle implementation
│   ├── Game.cpp           # Game logic & rendering
│   └── Primitives.cpp     # Primitive drawing
├── Assignment_4.sln       # Visual Studio solution
└── Assignment_4.vcxproj   # Visual Studio project
```

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| **←** / **→** | Move car left/right between lanes |
| **R** | Reset game (full state reset) |
| **Q** / **ESC** | Quit application |

---

## 🏗️ Physics & Collision

### Obstacle Spawning
- **10 obstacles** initialized at staggered Z positions
- **Spawn interval:** ~3 seconds apart
- **Movement speed:** 25 units/second toward camera
- **Recycling:** Off-screen obstacles respawn at far end in random lanes

### Collision Detection
```cpp
if (car.lane == obstacle.lane && |car.Z - obstacle.Z| < 2.0) {
    collision_detected = true;
}
```
- **Threshold:** 2.0 units along Z-axis
- **Display duration:** 2 seconds after collision
- **Reset:** R key clears collision state and resets all positions

---

## 🖥️ Rendering

### Rear View (2/3 of screen, left)
- **Projection:** Perspective (60° FOV)
- **Camera:** Follow-behind the car, elevated slightly
- **Scene:** Car, obstacles, road with lane markings

### Top-Down View (1/3 of screen, right)
- **Projection:** Orthographic looking straight down
- **Camera:** Fixed overhead position tracking car's Z
- **Scene:** Bird's-eye view of all game elements

### HUD Overlay
- Controls displayed in top-left corner
- Collision message centered with semi-transparent red background
- Text rendered via GLUT bitmap fonts

---

## 🔧 Build & Run

### Requirements
- **Visual Studio 2022** (MSVC v143)
- **GLEW** – OpenGL Extension Wrangler
- **FreeGLUT** – Windowing & input

### Setup (Windows with property sheets)
1. Open `Assignment_4.sln`
2. Ensure GLEW and FreeGLUT include/lib paths are in system environment or user property sheets
3. Select **Debug | x64** (or your platform)
4. Press `Ctrl+F5` to build and run

### If Linking Fails
Add to **Linker → Additional Dependencies** (Project Properties):
```
glew32s.lib; freeglut.lib; opengl32.lib; glu32.lib;
```

And to **Preprocessor Definitions**:
```
GLEW_STATIC
```

---

## 🎨 Visual Design

### Car
- **Red** rectangular prism with **windshield** (semi-transparent blue)
- **Dark grey wheels** at corners (spheres)
- Size: 0.8 × 0.8 × 0.8 units

### Obstacles
- **Gold/yellow** cubes
- Darker outline for depth perception
- Size: 0.6 × 0.6 × 0.6 units

### Road
- **Grey** base surface
- **White dashed** lane markings
- **Blue sky** background in rear view
- **Dark background** in top-down view

---

## 📊 Game State & Reset

When **R** is pressed:
1. Car position → center lane at `Z = -20`
2. All obstacles → respawn to initial staggered positions
3. Collision state → cleared
4. Score/timer → reset (ready for new game)
5. Animation → restarts immediately

---

## 🚀 Future Enhancements (Optional)

- Score/survival time tracking
- Progressive difficulty (speed increase)
- Power-ups or obstacles with different behaviors
- Sound effects
- Particle effects on collision
- Multiple difficulty levels

---

## 📝 Team Members

| Name | Student ID |
|------|------------|
| Ali Mahmoud Ahmed | 1230338 |
| Ahmed Attay Kamal | 1230011 |
| Mahmoud Mohamed Abdelfattah | 4220142 |
| Seif Allah Alaa Mohamed | 1200324 |

---

*Cairo University · Faculty of Engineering · Biomedical Engineering · Spring 2026*
