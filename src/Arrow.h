#ifndef ARROW_H_
#define ARROW_H_

#include <queue>
#include <iostream>
#include <functional>
#include <ncurses.h>
#include "Traversable.h"
#include "Renderer.h"

class Arrow : public Traversable
{
public:
    Arrow(Point newBegin, Point newEnd, std::function<void()> newRemover) : begin(newBegin), end(newEnd), remover(newRemover)
    {
        if (freeIndexes.empty())
            index = maxIndex++;
        else
        {
            index = freeIndexes.front();
            freeIndexes.pop();
        }
        pos.x = -index * 2;
        pos.y = std::min(begin.y, end.y);
        size.x = index * 2;
        size.y = std::abs(begin.y - end.y);
    }
    Arrow(const Arrow &) = delete;
    Arrow(Arrow &&) = delete;
    virtual ~Arrow()
    {
        freeIndexes.push(index);
    }
    Point getPosition() const override
    {
        return pos;
    }
    Point getSize() const override
    {
        return size;
    }
    bool operator==(const Arrow &rhs) const
    {
        return index == rhs.index;
    }
    void remove()
    {
        remover();
    }

private:
    void draw(Renderer &renderer) const override
    {
        if (isSelected())
            renderer.attributeOn(A_REVERSE);
        renderer.drawString({pos.x, begin.y}, "+" + std::string(size.x - 2, '-') + "<");
        renderer.drawString({pos.x, end.y}, "+" + std::string(size.x - 2, '-') + ">");
        for (int i = std::min(begin.y, end.y) + 1; i < std::max(begin.y, end.y); i++)
            renderer.drawString({pos.x, i}, "|");
        if (isSelected())
            renderer.attributeOff(A_REVERSE);
    }
    Point pos, size;
    Point begin, end;
    int index;
    std::function<void()> remover;
    inline static std::queue<int> freeIndexes;
    inline static int maxIndex = 3;
};

#endif /* !ARROW_H_ */
