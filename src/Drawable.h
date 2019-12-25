#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include "Point.h"

class Renderer;

class Drawable
{
public:
    virtual Point getPosition() const = 0;
    virtual Point getSize() const = 0;
    virtual ~Drawable() = default;

private:
    virtual void draw(Renderer &renderer) const = 0;
    friend class Renderer;
};

#endif /* !DRAWABLE_H_ */
