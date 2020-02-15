#ifndef LINUXPIPELINES_H_
#define LINUXPIPELINES_H_

#include <map>
#include "ToolBase.hpp"
#include "BackendBase.hpp"
#include "Controller.h"

namespace tools
{
class LinuxPipelines : public ToolBase
{
public:
    LinuxPipelines()
    {
        setEntry("IS_SAVED", "NO");
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        return entries[key];
    }
    void setCoordinates(int width, int height, int startx, int starty) noexcept override
    {
        ctrl.getRenderer().setCoordinates(width, height, startx, starty);
    }
    void setBackend(backends::BackendBase &newBackend) override
    {
        ToolBase::setBackend(newBackend);

        setEntry("IS_SAVED", "YES");

        auto bindHelper = [this, &newBackend](const std::string &command, std::function<void()> callback, const std::string &helpMessage) {
            newBackend.bind(command, [this, callback]() {
                callback();
                if (ctrl.getDirty())
                    setEntry("IS_SAVED", "NO");
                else
                    setEntry("IS_SAVED", "YES");
            },
                            helpMessage);
        };

        //Arrows
        for (const auto &key : {"<UARROW>", "<DARROW>", "<RARROW>", "<LARROW>"})
        {
            newBackend.bind(key, [this, key]() {
                ctrl.processKEY(key);
                if (ctrl.getDirty())
                    setEntry("IS_SAVED", "NO");
                else
                    setEntry("IS_SAVED", "YES");
            },
                            "navigation");
        }
        //Edition
        bindHelper("<EDITION>", [this]() {
            ctrl.processKEY(getEntry("KEY"));
        },
                   "");
        //Add process
        bindHelper("#vim#:process ${PROCESS_NAME}", [this]() {
            ctrl.process(getEntry("PROCESS_NAME"));
        },
                   "add new process to graph");
        bindHelper("#nice#.Add.Process${Provide a process name: |PROCESS_NAME}", [this]() {
            ctrl.process(getEntry("PROCESS_NAME"));
        },
                   "add new process to graph");

        //Add file
        bindHelper("#vim#:file ${FILE_NAME}", [this]() {
            ctrl.file(getEntry("FILE_NAME"));
        },
                   "add new file to graph");
        bindHelper("#nice#.Add.File${Provide a file name: |FILE_NAME}", [this]() {
            ctrl.file(getEntry("FILE_NAME"));
        },
                   "add new file to graph");

        //Edit selected
        bindHelper("#vim#:quickedit ${NEW_CONTENT}", [this]() {
            ctrl.edit(getEntry("NEW_CONTENT"));
        },
                   "edit selected entity (process or file)");
        bindHelper("#nice#.Selection.Edit${Provide new content: |NEW_CONTENT}", [this]() {
            ctrl.edit(getEntry("NEW_CONTENT"));
        },
                   "edit selected entity (process or file)");

        //Remove selected
        bindHelper("#vim#:remove", [this]() {
            ctrl.remove();
        },
                   "remove selected entity (process, file or pipe)");
        bindHelper("#nice#.Selection.Remove", [this]() {
            ctrl.remove();
        },
                   "remove selected entity (process, file or pipe)");
        bindHelper("<DEL>", [this]() {
            ctrl.remove();
        },
                   "remove selected entity (process, file or pipe)");

        //Show list
        bindHelper("#vim#:list", [this]() {
            ctrl.list();
        },
                   "open list of binaries and files accesible from PATH environmental variable");
        bindHelper("#nice#.PATH list.Show", [this]() {
            ctrl.list();
        },
                   "open list of binaries and files accesible from PATH environmental variable");

        //Insert from list
        bindHelper("#vim#:insert ${TYPE}", [this]() {
            ctrl.insert(getEntry("TYPE"));
        },
                   "insert selected element (process or file) from list (see :list) to graph, type should be 'process' or 'file'");
        bindHelper("#nice#.PATH list.Insert process", [this]() {
            ctrl.insert("process");
        },
                   "insert selected element from list as process (see PATH list.Show) to graph");
        bindHelper("#nice#.PATH list.Insert file", [this]() {
            ctrl.insert("file");
        },
                   "insert selected element from list as file (see PATH list.Show) to graph");

        //Save
        bindHelper("#vim#:save ${PROJECT_FILE}", [this]() {
            ctrl.save(getEntry("PROJECT_FILE"));
        },
                   "save linux pipelines project file");
        bindHelper("#nice#.File.Save${Provide a project file name: |PROJECT_FILE}", [this]() {
            ctrl.save(getEntry("PROJECT_FILE"));
        },
                   "save linux pipelines project file");

        //Open
        bindHelper("#vim#:open ${PROJECT_FILE}", [this]() {
            ctrl.open(getEntry("PROJECT_FILE"));
        },
                   "open linux pipelines project file");
        bindHelper("#nice#.File.Open${Provide a project file name: |PROJECT_FILE}", [this]() {
            ctrl.open(getEntry("PROJECT_FILE"));
        },
                   "open linux pipelines project file");

        //Export .sh file
        bindHelper("#vim#:export ${SCRIPT_FILE}", [this]() {
            ctrl.exportSh(getEntry("SCRIPT_FILE"));
        },
                   "export linux pipelines project to bash script file");
        bindHelper("#nice#.File.Export${Provide a name for new bash script: |SCRIPT_FILE}", [this]() {
            ctrl.exportSh(getEntry("SCRIPT_FILE"));
        },
                   "export linux pipelines project to bash script file");

        //Swap selected
        bindHelper("#vim#:swap", [this]() {
            ctrl.swap();
        },
                   "start or end swap operation (consecutive calls swaps consecutively selected elements)");
        bindHelper("#nice#.Selection.Swap", [this]() {
            ctrl.swap();
        },
                   "start or end swap operation (consecutive calls swaps consecutively selected elements)");

        //Pipe out
        bindHelper("#vim#:pipeout", [this]() {
            ctrl.pipe("STDOUT");
        },
                   "start creating pipe in stdout of selected process");
        bindHelper("#nice#.Pipe.Start in stdout", [this]() {
            ctrl.pipe("STDOUT");
        },
                   "start creating pipe in stdout of selected process");

        //Pipe err
        bindHelper("#vim#:pipeerr", [this]() {
            ctrl.pipe("STDERR");
        },
                   "start creating pipe in stderr of selected process");
        bindHelper("#nice#.Pipe.Start in stderr", [this]() {
            ctrl.pipe("STDERR");
        },
                   "start creating pipe in stderr of selected process");

        //Pipe end
        bindHelper("#vim#:pipe", [this]() {
            ctrl.pipe("");
        },
                   "end creating pipe, redirect to selected entity (process or file)");
        bindHelper("#nice#.Pipe.End", [this]() {
            ctrl.pipe("");
        },
                   "end creating pipe, redirect to selected entity (process or file)");
        
        bindHelper("#vim#:edit!EDIT", [this]() {}, "enter edit mode");
        bindHelper("#vim#o", [this]() {
            ctrl.goToGraph();
        },
                   "accept the message (ok)");
    }

private:
    std::map<std::string, std::string>
        entries;
    std::string text;
    Controller ctrl;
};
} // namespace tools

#endif /* !LINUXPIPELINES_H_ */
