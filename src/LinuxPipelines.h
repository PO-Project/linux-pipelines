#ifndef LINUXPIPELINES_H_
#define LINUXPIPELINES_H_

#include <map>
#include "../common-interface/ToolBase.hpp"
#include "../common-interface/BackendBase.hpp"
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
        newBackend.bind("<EDITION>", [this]() {
            ctrl.processKEY(getEntry("KEY"));
        },
                        "");
        newBackend.bind("process", [this]() {
            ctrl.process();
        },
                        "");
        newBackend.bind("pipe", [this]() {
            ctrl.pipe(getEntry("ARG"));
        },
                        "");
        newBackend.bind("edit", [this]() {
            ctrl.edit(getEntry("ARG"));
        },
                        "");
        newBackend.bind("remove", [this]() {
            ctrl.remove();
        },
                        "");
                        newBackend.bind("list", [this]() {
            ctrl.list();
        },
                        "");
                        newBackend.bind("save", [this]() {
            ctrl.save(getEntry("ARG"));
        },
                        "");
                        newBackend.bind("open", [this]() {
            ctrl.open(getEntry("ARG"));
        },
                        "");
                        newBackend.bind("export", [this]() {
            ctrl.exportSh(getEntry("ARG"));
        },
                        "");
                        newBackend.bind("file", [this]() {
            ctrl.file();
        },
                        "");
    
    }

private:
    std::map<std::string, std::string> entries;
    std::string text;
    Controller ctrl;
};
} // namespace tools

#endif /* !LINUXPIPELINES_H_ */
