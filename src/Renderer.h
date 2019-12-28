#ifndef RENDERER_H_
#define RENDERER_H_

#include <string>
#include "Drawable.h"
#include "Point.h"

class Renderer
{
public:
    void draw(const Drawable &drawable)
    {
        drawable.draw(*this);
    }
    virtual void drawString(const std::string &str, Point position) = 0;
    void drawString(const std::string &str)
    {
        drawString(str, center);
    }
    virtual void setCoordinates(int width, int height, int startx, int starty) = 0;
    virtual void clear() = 0;
    virtual void attributeOn(unsigned long attribute) = 0;
    virtual void attributeOff(unsigned long attribute) = 0;
    void setCenter(Point newCenter)
    {
        center = newCenter;
    }
    virtual void forceUpdate() = 0;

protected:
    Point center = {0, 0};
};

#endif /* !RENDERER_H_ */
