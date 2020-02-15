#include "LinuxPipelines.h"
#include "../backends/backend-nice/src/BackendNice.h"

int main()
{
    tools::LinuxPipelines tool;
    backends::BackendNice backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
