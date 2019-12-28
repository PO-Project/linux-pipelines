#ifndef FILE_H_
#define FILE_H_

#include "Process.h"

class File : public Process
{
public:
File()
    {
        setContent("file.txt");
    }
    void pipeOut(Process& process) override
    {
        resetIn();
        Process::pipeOut(process);
    }
    void pipeErr(Process& process) override
    {
        resetIn();
        Process::pipeErr(process);
    }
    private:
    void pipeIn(Process& process) override
    {
        resetOut();
        resetErr();
        Process::pipeIn(process);
    }
};

#endif /* !FILE_H_ */
