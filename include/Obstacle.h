#pragma once

// ─── Obstacle class for lane-based runner game ──────────────────────────────

class Obstacle
{
public:
    // Constructor and lifecycle
    Obstacle(int lane, float startZ);
    ~Obstacle() = default;

    // Update simulation
    void update(float deltaTime, float speed);

    // State queries
    int getLane() const { return lane; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    bool isOffScreen() const; // Obstacle passed the car

    // Rendering
    void draw() const;

    // Reset (respawn)
    void respawn(int newLane, float newStartZ);

    // Constants
    static constexpr float OBSTACLE_SIZE = 0.6f;
    static constexpr float LANE_WIDTH = 2.0f;
    static constexpr float LANE_SPACING = 3.0f;

private:
    int lane;               // 0=left, 1=center, 2=right
    float x, y, z;         // World position
    float startZ;          // Initial Z when spawned

    // Helper to compute X from lane
    float laneToX(int lane) const;
};
