#ifndef TEXT_H_
#define TEXT_H_

#include <queue>
#include <iostream>
#include <sstream>
#include <ncurses.h>
#include "Traversable.h"
#include "Renderer.h"

class Text : public Traversable<GroupMode::TEXT>
{
public:
    template <class Emplacable>
    static void emplaceTexts(Emplacable &container, const std::string &text)
    {
        int maxWidth = 0;
        std::string str;
        std::istringstream stream(text);
        while (std::getline(stream, str, '\n'))
        {
            container.emplace(container.end(), str);
            maxWidth = std::max(maxWidth, static_cast<int>(str.size()));
        }
        for (Text &text : container)
        {
            text.size.x = maxWidth;
        }
    }
    Text(const std::string &str) : size({static_cast<int>(str.size()), 1}), content(str)
    {
        if (freeIndexes.empty())
            index = maxIndex++;
        else
        {
            index = freeIndexes.front();
            freeIndexes.pop();
        }
        pos.x = 0;
        pos.y = index;
    }
    Text(const Text &) = delete;
    Text(Text &&) = delete;
    virtual ~Text()
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
        size.x = content.size();
    }
    std::string getContent()
    {
        return content;
    }
    bool operator==(const Text &rhs) const
    {
        return index == rhs.index;
    }

protected:
    void draw(Renderer &renderer) const override
    {
        if (isSelected())
            renderer.attributeOn(A_REVERSE);
        renderer.drawString(content, pos);
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

#endif /* !TEXT_H_ */
