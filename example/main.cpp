#include "ExampleBackendImproved.h"
#include "../src/LinuxPipelines.h"

int main()
{
    tools::LinuxPipelines tool;
    backends::ExampleBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
