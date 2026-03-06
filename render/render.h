#ifndef RENDER_H
#define RENDER_H

class Renderer {
public:
    virtual ~Renderer() {}

    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void clear() = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void drawFilledTriangle(
        int x0, int y0,
        int x1, int y1,
        int x2, int y2) = 0;

    virtual void flush() = 0;
};

#endif