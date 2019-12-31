#ifndef BOX_H_
#define BOX_H_

#include <queue>
#include <iostream>
#include <ncurses.h>
#include "Traversable.h"
#include "Renderer.h"

class Box : public Traversable<GroupMode::GRAPH>
{
public:
    Box() : size({3, 3}), content("  ")
    {
        if (freeIndexes.empty())
            index = maxIndex++;
        else
        {
            index = freeIndexes.front();
            freeIndexes.pop();
        }
        pos.x = 0;
        pos.y = index * 4;
    }
    Box(const Box &) = delete;
    Box(Box &&) = delete;
    virtual ~Box()
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
    void setContent(const std::string &newContent)
    {
        content = newContent;
        size.x = content.size() + 2;
    }
    std::string getContent() const
    {
        return content;
    }
    bool operator==(const Box &rhs) const
    {
        return index == rhs.index;
    }
    int getIndex() const
    {
        return index;
    }
    static void swap(Box &a, Box &b)
    {
        std::swap(a.index, b.index);
        std::swap(a.pos, b.pos);
    }

protected:
    void draw(Renderer &renderer) const override
    {
        if (isSelected())
            renderer.attributeOn(A_REVERSE);
        renderer.drawString("+" + std::string(size.x, '-') + "+", pos);
        renderer.drawString("| " + content + " |", pos + Point{0, 1});
        renderer.drawString("+" + std::string(size.x, '-') + "+", pos + Point{0, 2});
        if (isSelected())
            renderer.attributeOff(A_REVERSE);
    }

private:
    Point pos, size;
    int index;
    std::string content;
    inline static std::queue<int> freeIndexes;
    inline static int maxIndex = 0;
};

#endif /* !BOX_H_ */