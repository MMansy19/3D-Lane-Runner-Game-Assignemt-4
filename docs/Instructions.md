# Assignment 4: 3D Lane Runner Game

## Objective
Create a 3D lane-based runner game featuring a car and 10 obstacles with dual viewport rendering (rear 3D view + top-down view). The game must include collision detection and a full reset mechanism.

## Requirements

### ✅ Core Gameplay
- **Car Control:** Player moves left/right between 3 lanes using arrow keys
- **Obstacles:** 10 obstacles move toward the car along the road
- **Collision Detection:** Detects when car and obstacle occupy the same lane and position
- **Collision Response:** Displays "TRY AGAIN – Press R" message on collision
- **Full Reset:** R key resets car position, obstacle positions, and restarts animation

### ✅ Rendering
- **Dual Viewports:**
  - Rear third-person view (2/3 of screen, left) showing car and obstacles from behind
  - Top-down view (1/3 of screen, right) showing the entire course
  - Both viewports render in real-time simultaneously
- **3D Graphics:** OpenGL with proper perspective and depth
- **Lane Visualization:** Clear lane markings on the road

### ✅ Technical Stack
- **Framework:** OpenGL 4.3 (Compatibility Profile)
- **Windowing:** FreeGLUT
- **Extensions:** GLEW (OpenGL Extension Wrangler)
- **Compiler:** MSVC v143 (Visual Studio 2022)
- **Language:** C++ with class-based architecture
- **Animation:** glutTimerFunc-based at ~60 FPS

### ✅ Game Balance
- Obstacle speed: 25 units/second
- Spawn interval: ~3 seconds
- Collision detection range: 2 units along Z-axis
- Collision display duration: 2 seconds
- Car can move at any speed (instantaneous lane changes)

## Deliverables

1. **Source Code** – Modular, well-commented C++ with separate classes for Car, Obstacle, Game
2. **Visual Studio Project** – Fully configured .sln/.vcxproj with proper include/link paths
3. **Documentation** – README.md with build instructions and architecture overview
4. **Executable** – Buildable and runnable on Windows with GLEW/FreeGLUT installed

## Evaluation Criteria

- **Functionality:** All requirements implemented and working
- **Code Quality:** Clean, modular architecture with reusable components
- **Graphics:** Smooth rendering, proper viewports, clear visual feedback
- **User Experience:** Responsive controls, clear collision messaging
- **Documentation:** Clear README and inline code comments

## Group Submission Note

This assignment is completed as a group; however, **each member is evaluated individually** based on their contribution to the code, documentation, and testing.

---

*Assignment 4 · Computer Graphics · Cairo University · Spring 2026*
