#include "BackendNice.h"
#include "LinuxPipelines.h"

int main()
{
    tools::LinuxPipelines tool;
    backends::BackendNice backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
