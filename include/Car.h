#pragma once

// ─── Car class for lane-based runner game ──────────────────────────────────

class Car
{
public:
    // Constructor and lifecycle
    Car();
    ~Car() = default;

    // Movement control
    void moveLeft();
    void moveRight();
    void resetPosition();

    // State queries
    int getCurrentLane() const { return currentLane; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    // Constants
    static constexpr int LANE_LEFT = 0;
    static constexpr int LANE_CENTER = 1;
    static constexpr int LANE_RIGHT = 2;
    static constexpr int NUM_LANES = 3;

    static constexpr float LANE_WIDTH = 2.0f;
    static constexpr float LANE_SPACING = 3.0f;
    static constexpr float CAR_SIZE = 0.8f;
    static constexpr float CAR_START_Z = -20.0f;

    // Rendering
    void draw() const;

private:
    int currentLane;        // 0=left, 1=center, 2=right
    float x, y, z;         // World position (Z fixed at CAR_START_Z)

    // Helper to compute X from lane
    float laneToX(int lane) const;
};
