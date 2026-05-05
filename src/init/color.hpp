#pragma once

#define BLACK   0.0f, 0.0f, 0.0f, 1.0f
#define WHITE   1.0f, 1.0f, 1.0f, 1.0f

#define RED     1.0f, 0.0f, 0.0f, 1.0f
#define GREEN   0.0f, 1.0f, 0.0f, 1.0f
#define BLUE    0.0f, 0.0f, 1.0f, 1.0f

#define YELLOW  1.0f, 1.0f, 0.0f, 1.0f
#define CYAN    0.0f, 1.0f, 1.0f, 1.0f
#define MAGENTA 1.0f, 0.0f, 1.0f, 1.0f

#define DARK_GRAY  0.2f, 0.2f, 0.2f, 1.0f
#define GRAY       0.5f, 0.5f, 0.5f, 1.0f
#define LIGHT_GRAY 0.8f, 0.8f, 0.8f, 1.0f

#define ORANGE 1.0f, 0.5f, 0.0f, 1.0f
#define PURPLE 0.5f, 0.0f, 0.5f, 1.0f
#define PINK   1.0f, 0.4f, 0.7f, 1.0f

struct Color {
    float r, g, b, a;
    void set(float red, float green, float blue, float alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    void set(float value) { r = g = b = a = value; }

    void setR(float red) { r = red; }
    void setG(float green) { g = green; }
    void setB(float blue) { b = blue; }
    void setA(float alpha) { a = alpha; }

    float getR() const { return r; }
    float getG() const { return g; }
    float getB() const { return b; }
    float getA() const { return a; }
};
