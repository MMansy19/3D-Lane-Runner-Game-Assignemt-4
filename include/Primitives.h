#pragma once

// ─── Utility functions for drawing primitives ────────────────────────────

// Draw filled box (cube) at current position
void drawBox(float width, float height, float depth);

// Draw filled sphere
void drawSphere(float radius, int segments = 20);

// Draw a filled rectangle (2D)
void drawRect2D(float x, float y, float width, float height);

// Draw text in 2D screen space (for HUD)
void drawText2D(float x, float y, const char* text, bool largeFont = false);

// Draw road lane markings (for visualization)
void drawRoad(float length, float width, int numDashes);
