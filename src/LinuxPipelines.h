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
        for (const auto &key : {"<UARROW>", "<DARROW>", "<RARROW>", "<LARROW>"})
        {
            newBackend.bind(key, [this, key]() {
                ctrl.processKEY(key);
                if (ctrl.getDirty())
                    setEntry("IS_SAVED", "NO");
                else
                    setEntry("IS_SAVED", "YES");
            },
                            "");
        }
        bindHelper("<EDITION>", [this]() {
            ctrl.processKEY(getEntry("KEY"));
        },
                   "");
        bindHelper(":process ${ARG}", [this]() {
            ctrl.process(getEntry("ARG"));
        },
                   "");
        bindHelper(":quickedit ${ARG}", [this]() {
            ctrl.edit(getEntry("ARG"));
        },
                   "");
        bindHelper(":remove", [this]() {
            ctrl.remove();
        },
                   "");
        bindHelper("<DEL>", [this]() {
            ctrl.remove();
        },
                   "");
        bindHelper(":list", [this]() {
            ctrl.list();
        },
                   "");
        bindHelper(":insert", [this]() {
            ctrl.list();
        },
                   "");
        bindHelper(":save ${ARG}", [this]() {
            ctrl.save(getEntry("ARG"));
        },
                   "");
        bindHelper(":open ${ARG}", [this]() {
            ctrl.open(getEntry("ARG"));
        },
                   "");
        bindHelper(":export ${ARG}", [this]() {
            ctrl.exportSh(getEntry("ARG"));
        },
                   "");
        bindHelper(":file ${ARG}", [this]() {
            ctrl.file(getEntry("ARG"));
        },
                   "");
        bindHelper(":swap", [this]() {
            ctrl.swap();
        },
                   "");
        bindHelper(":pipeout", [this]() {
            ctrl.pipe("STDOUT");
        },
                   "");
        bindHelper(":pipeerr", [this]() {
            ctrl.pipe("STDERR");
        },
                   "");
        bindHelper(":pipe", [this]() {
            ctrl.pipe("");
        },
                   "");
        bindHelper("#vim#:edit!EDIT", [this]() {}, "");
        bindHelper("o", [this]() {
            ctrl.goToGraph();
        },
                   "");
    }

private:
    std::map<std::string, std::string>
        entries;
    std::string text;
    Controller ctrl;
};
} // namespace tools

#endif /* !LINUXPIPELINES_H_ */
