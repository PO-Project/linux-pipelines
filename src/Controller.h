#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "TextRenderer.h"
#include "Process.h"
#include "Arrow.h"
#include "Text.h"
#include "File.h"

class Controller
{
public:
    Renderer &getRenderer()
    {
        return renderer;
    }
    void up()
    {
        switch (activeGroup)
        {
        case GroupMode::GRAPH:
            Traversable<GroupMode::GRAPH>::previousY();
            break;
        case GroupMode::TEXT:
            Traversable<GroupMode::TEXT>::previousY();
            break;
        }
    }
    void down()
    {
        switch (activeGroup)
        {
        case GroupMode::GRAPH:
            Traversable<GroupMode::GRAPH>::nextY();
            break;
        case GroupMode::TEXT:
            Traversable<GroupMode::TEXT>::nextY();
            break;
        }
    }
    void left()
    {
        switch (activeGroup)
        {
        case GroupMode::GRAPH:
            Traversable<GroupMode::GRAPH>::previousX();
            break;
        case GroupMode::TEXT:
            Traversable<GroupMode::TEXT>::previousX();
            break;
        }
    }
    void right()
    {
        switch (activeGroup)
        {
        case GroupMode::GRAPH:
            Traversable<GroupMode::GRAPH>::nextX();
            break;
        case GroupMode::TEXT:
            Traversable<GroupMode::TEXT>::nextX();
            break;
        }
    }
    void edit(const std::string &str)
    {
        if (activeGroup == GroupMode::GRAPH)
        {
            if (dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()))
            {
                dynamic_cast<Process &>(*Traversable<GroupMode::GRAPH>::get()).setContent(str);
                render();
            }
            else
                message("You must select process to edit.");
        }
        else
            message("You can edit processes only in graph edition mode.");
        //TODO inform about error
    }
    void process()
    {
        if (activeGroup == GroupMode::GRAPH)
        {
            processes.emplace_back(new Process);
            render();
        }
        else if (activeGroup == GroupMode::TEXT && activePathList && dynamic_cast<Text *>(Traversable<GroupMode::TEXT>::get()))
        {
            processes.emplace_back(new Process);
            processes.back()->setContent(dynamic_cast<Text &>(*Traversable<GroupMode::TEXT>::get()).getContent());
        }
        else
            message("You can add processes only in graph edition mode or after 'list' command'.");
    }
    void file()
    {
        if (activeGroup == GroupMode::GRAPH)
        {
            processes.emplace_back(new File);
            render();
        }
        else if (activeGroup == GroupMode::TEXT && activePathList && dynamic_cast<Text *>(Traversable<GroupMode::TEXT>::get()))
        {
            processes.emplace_back(new File);
            processes.back()->setContent(dynamic_cast<Text &>(*Traversable<GroupMode::TEXT>::get()).getContent());
        }
        else
            message("You can add processes only in graph edition mode or after 'list' command'.");
    }
    void pipe(std::string str)
    {
        if (activeGroup == GroupMode::GRAPH)
        {
            std::transform(str.begin(), str.end(), str.begin(), static_cast<int (*)(int)>(std::toupper));
            if ((str == "1" || str == "STDOUT") && dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()))
            {
                tempPipeBegin = dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get());
                tempPipeBeginDesc = 'o';
                message("New pipe starts in stdout of " + tempPipeBegin->getContent());
            }
            else if ((str == "2" || str == "STDERR") && dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()))
            {
                tempPipeBegin = dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get());
                tempPipeBeginDesc = 'e';
                message("New pipe starts in stderr of " + tempPipeBegin->getContent());
            }
            else if (tempPipeBegin && dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()))
            {
                if (tempPipeBeginDesc == 'o')
                    tempPipeBegin->pipeOut(dynamic_cast<Process &>(*Traversable<GroupMode::GRAPH>::get()));
                else if (tempPipeBeginDesc == 'e')
                    tempPipeBegin->pipeErr(dynamic_cast<Process &>(*Traversable<GroupMode::GRAPH>::get()));
                clearTemp();
                render();
            }
        }
    }
    void swap()
    {
        if (activeGroup == GroupMode::GRAPH)
        {
            if (dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()))
            {
                if (tempSwapBegin)
                {
                    Process::swap(*tempSwapBegin, *dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()));
                    tempSwapBegin = nullptr;
                }
                else
                {
                    tempSwapBegin = dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get());
                }
                render();
            }
            else
                message("You must select process to swap.");
        }
        else
            message("You can swap processes only in graph edition mode.");
    }
    void remove()
    {
        if (activeGroup == GroupMode::GRAPH)
        {
            if (dynamic_cast<Process *>(Traversable<GroupMode::GRAPH>::get()))
            {
                auto it = std::find_if(processes.begin(), processes.end(), [](const auto &p) {
                    return *p.get() == dynamic_cast<Process &>(*Traversable<GroupMode::GRAPH>::get());
                });
                processes.erase(it);
                clearTemp();
                render();
            }
            else if (dynamic_cast<Arrow *>(Traversable<GroupMode::GRAPH>::get()))
            {
                dynamic_cast<Arrow &>(*Traversable<GroupMode::GRAPH>::get()).remove();
                render();
            }
            else
                message("You must select process or pipe to remove.");
        }
        else
            message("You can remove processes and pipes only in graph edition mode.");
    }
    void processKEY(const std::string &key)
    {
        if (key == "<UARROW>")
            up();
        else if (key == "<RARROW>")
            right();
        else if (key == "<DARROW>")
            down();
        else if (key == "<LARROW>")
            left();
        else
        {
            if (key == "<DEL>")
                remove();
            activeGroup = GroupMode::GRAPH;
            activePathList = false;
        }
        render();
    }
    void list()
    {
        const char *pathC = std::getenv("PATH");
        if (pathC != nullptr)
        {
            std::string path(pathC);

            initText();
            activePathList = true;
            std::vector<std::string> commands;
            struct stat sb;
            std::string str;
            std::istringstream stream(path);
            loadingAnimation();
            while (std::getline(stream, str, ':'))
            {
                loadingAnimation();
                try
                {
                    for (const auto &entry : std::filesystem::directory_iterator(str))
                    {
                        loadingAnimation();
                        if (!std::filesystem::is_directory(entry.path()))
                            if (!entry.path().has_extension() && stat(entry.path().c_str(), &sb) == 0 && (sb.st_mode & S_IXUSR || sb.st_mode & S_IXGRP || sb.st_mode & S_IXOTH))
                                commands.push_back(entry.path().filename().string());
                    }
                }
                catch (...)
                {
                }
            }
            try
            {
                for (const auto &entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
                {
                    loadingAnimation();
                    if (!std::filesystem::is_directory(entry.path()))
                        commands.push_back(entry.path().filename().string());
                }
            }
            catch (...)
            {
            }
            commands.push_back("/dev/null");
            std::sort(commands.begin(), commands.end());
            for (const auto &command : commands)
            {
                loadingAnimation();
                Text::emplaceTexts(texts, command);
            }
            render();
        }
    }
    void save(const std::string &filename)
    {
        std::ofstream file(filename);
        processes.sort();
        for (const auto &process : processes)
        {
            file << process->getContent();
            if (dynamic_cast<File *>(process.get()))
                file << " #file";
            file << '\n';
            if (process->getOut())
            {
                auto it = std::find_if(processes.begin(), processes.end(), [&process](const auto &p) {
                    return *p.get() == *process->getOut();
                });
                //std::find(processes.begin(), processes.end(), *process.getOut());
                file << std::distance(processes.begin(), it) << '\n';
            }
            else
                file << "-1\n";
            if (process->getErr())
            {
                auto it = std::find_if(processes.begin(), processes.end(), [&process](const auto &p) {
                    return *p.get() == *process->getErr();
                });
                file << std::distance(processes.begin(), it) << '\n';
            }
            else
                file << "-1\n";
        }
    }
    void open(const std::string &filename)
    {
        std::ifstream file(filename);
        processes.clear();
        std::string str;
        std::vector<std::pair<int, int>> outPipes, errPipes;
        int n = 0, o, e;
        try
        {
            while (std::getline(file, str, '\n'))
            {
                if (str.find(" #file") != std::string::npos)
                {
                    processes.emplace_back(new File);
                    processes.back()->setContent(str.substr(0, str.find(" #file")));
                }
                else
                {
                    processes.emplace_back(new Process);
                    processes.back()->setContent(str);
                }
                std::getline(file, str, '\n');
                o = std::stoi(str);
                if (o != -1)
                    outPipes.push_back({n, o});
                std::getline(file, str, '\n');
                e = std::stoi(str);
                if (e != -1)
                    errPipes.push_back({n, e});
                n++;
            }
        }
        catch (...)
        {
            message("File is corrupted, failed to open.");
            return;
        }
        auto a = processes.begin();
        auto b = processes.begin();
        for (const auto &p : outPipes)
        {
            a = processes.begin();
            b = processes.begin();
            std::advance(a, p.first);
            std::advance(b, p.second);
            (*a)->pipeOut(**b);
        }
        for (const auto &p : errPipes)
        {
            a = processes.begin();
            b = processes.begin();
            std::advance(a, p.first);
            std::advance(b, p.second);
            (*a)->pipeErr(**b);
        }
        render();
    }
    void exportSh(const std::string &filename)
    {
        std::ofstream file(filename);
        for (const auto &process : processes)
        {
            if (process->getIn())
                file << "mkfifo "
                     << "/tmp/" + process->pipeNameIn() << '\n';
        }
        for (const auto &process : processes)
        {
            if (process->getIn())
            {
                if (dynamic_cast<File *>(process.get()))
                    file << "cat "
                         << "/tmp/" + process->pipeNameIn() << " > ";
                else
                    file << "cat "
                         << "/tmp/" + process->pipeNameIn() << " | ";
            }
            if (dynamic_cast<File *>(process.get()) && process->getOut())
                file << "cat ";
            file << process->getContent();
            if (process->getOut())
                file << " > "
                     << "/tmp/" + process->pipeNameOut();
            if (process->getErr())
                file << " 2> "
                     << "/tmp/" + process->pipeNameErr();
            file << " &\npids[" << process->getIndex() << "]=$!\n";
        }
        file << "for pid in ${pids[*]}; do\n\twait $pid\ndone\n";
        file << "rm /tmp/*.linuxPipelinesPipe 2> /dev/null\n";
    }

private:
    void render()
    {
        renderer.clear();
        switch (activeGroup)
        {
        case GroupMode::GRAPH:
            if (Traversable<GroupMode::GRAPH>::get() != nullptr)
                renderer.setCenter(Traversable<GroupMode::GRAPH>::get()->getAveragePosition());
            for (auto &process : processes)
                renderer.draw(*process);
            break;
        case GroupMode::TEXT:
            if (Traversable<GroupMode::TEXT>::get() != nullptr)
                renderer.setCenter(Traversable<GroupMode::TEXT>::get()->getAveragePosition());
            for (auto &text : texts)
                renderer.draw(text);

            break;
        }
    }
    void clearTemp()
    {
        tempPipeBegin = nullptr;
        tempSwapBegin = nullptr;
        tempPipeBeginDesc = 'n';
    }
    void initText()
    {
        activePathList = false;
        activeGroup = GroupMode::TEXT;
        texts.clear();
    }
    void loadingAnimation()
    {
        constexpr int dots = 4;
        constexpr int waitTime = 1000;
        static short c = 0;
        static std::string s = "Loading ";
        c++;
        if (c == dots * waitTime)
        {
            s = "Loading ";
            renderer.drawString(s + std::string(dots, ' '));
            renderer.forceUpdate();
            c = 0;
        }
        if (c % waitTime == 0 && c)
        {
            s.push_back('.');
            renderer.drawString(s);
            renderer.forceUpdate();
        }
    }
    void message(const std::string &str)
    {
        initText();
        Text::emplaceTexts(texts, str + "\nPress any non arrow key to continue ...");
        render();
    }
    GroupMode activeGroup = GroupMode::GRAPH;
    bool activePathList = 0;
    std::list<std::unique_ptr<Process>> processes;
    std::list<Text> texts;
    Process *tempPipeBegin = nullptr;
    Process *tempSwapBegin = nullptr;
    char tempPipeBeginDesc = 'n';
    TextRenderer renderer;
};

#endif /* !CONTROLLER_H_ */
