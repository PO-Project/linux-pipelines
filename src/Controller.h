#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <algorithm>
#include <string>
#include "TextRenderer.h"
#include "Process.h"
#include "Arrow.h"

class Controller
{
public:
    Renderer &getRenderer()
    {
        return renderer;
    }
    void up()
    {
        Traversable::previousY();
    }
    void down()
    {
        Traversable::nextY();
    }
    void left()
    {
        Traversable::previousX();
    }
    void right()
    {
        Traversable::nextX();
    }
    void edit(const std::string &str)
    {
        if (dynamic_cast<Process *>(Traversable::get()))
        {
            dynamic_cast<Process &>(*Traversable::get()).setContent(str);
        }
        render();
        //TODO inform about error
    }
    void process()
    {
        processes.emplace_back();
        render();
    }
    void pipe(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), static_cast<int (*)(int)>(std::toupper));
        if (str == "0" && dynamic_cast<Process *>(Traversable::get()))
        {
            tempPipeBegin = dynamic_cast<Process *>(Traversable::get());
            tempPipeBeginDesc = 'o';
        }
        else if (str == "1" && dynamic_cast<Process *>(Traversable::get()))
        {
            tempPipeBegin = dynamic_cast<Process *>(Traversable::get());
            tempPipeBeginDesc = 'e';
        }
        else if (tempPipeBegin && dynamic_cast<Process *>(Traversable::get()))
        {
            if (tempPipeBeginDesc == 'o')
                tempPipeBegin->pipeOut(dynamic_cast<Process &>(*Traversable::get()));
            else if (tempPipeBeginDesc == 'e')
                tempPipeBegin->pipeErr(dynamic_cast<Process &>(*Traversable::get()));
            clearTemp();
        }

        render();
    }
    void remove()
    {
        if (dynamic_cast<Process *>(Traversable::get()))
        {
            processes.remove(dynamic_cast<Process &>(*Traversable::get()));
            clearTemp();
        }
        else if (dynamic_cast<Arrow *>(Traversable::get()))
        {
            dynamic_cast<Arrow &>(*Traversable::get()).remove();
        }
        render();
        //TODO inform about error
    }
    void processKEY(const std::string &key)
    {
        if (key == "<DEL>")
            remove();
        else if (key == "<UARROW>")
            up();
        else if (key == "<RARROW>")
            right();
        else if (key == "<DARROW>")
            down();
        else if (key == "<LARROW>")
            left();
        render();
    }

private:
    void render()
    {
        renderer.clear();
        if (Traversable::get() != nullptr)
            renderer.setCenter(Traversable::get()->getAveragePosition());
        for (auto &process : processes)
            renderer.draw(process);
    }
    void clearTemp()
    {
        tempPipeBegin = nullptr;
        tempPipeBeginDesc = 'n';
    }
    std::list<Process> processes;
    Process *tempPipeBegin = nullptr;
    char tempPipeBeginDesc = 'n';
    TextRenderer renderer;
};

#endif /* !CONTROLLER_H_ */
