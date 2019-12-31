#include "VimBackend.hpp"
#include "LinuxPipelines.h"

int main()
{
    tools::LinuxPipelines tool;
    backends::VimBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
