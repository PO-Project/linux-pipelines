#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_

#include <ncurses.h>
#include <panel.h>
#include "Renderer.h"

class TextRenderer : public Renderer
{
public:
    using Renderer::drawString;
    ~TextRenderer()
    {
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }
    }
    void setCoordinates(int width, int height, int startx, int starty) override
    {
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }

        window = newwin(width, height, startx, starty);
        panel = new_panel(window);
    }
    void drawString(const std::string &str, Point position) override
    {
        Point max, halfMax;
        getmaxyx(window, max.y, max.x);
        halfMax = max / 2;
        position = position - center + halfMax;
        int shift = 0;
        if (position.x < 0)
        {
            shift = std::min((int)str.size(), -position.x);
            position.x = 0;
        }
        mvwprintw(window, position.y, position.x, "%*.*s", 1, max.x - position.x, str.c_str() + shift);
        wmove(window, 0, 0);
    }
    void clear() override
    {
        werase(window);
    }
    void attributeOn(unsigned long attribute) override
    {
        wattron(window, attribute);
    }
    void attributeOff(unsigned long attribute) override
    {
        wattroff(window, attribute);
    }
    void forceUpdate() override
    {
        update_panels();
        doupdate();
    }

private:
    WINDOW *window = nullptr;
    PANEL *panel = nullptr;
};

#endif /* !TEXTRENDERER_H_ */
