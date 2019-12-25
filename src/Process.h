#ifndef PROCESS_H_
#define PROCESS_H_

#include <memory>
#include "Box.h"
#include "Arrow.h"

class Process : public Box
{
public:
    Process()
    {
        in = nullptr;
        out = nullptr;
        err = nullptr;
    }
    ~Process()
    {
        resetIn();
        resetOut();
        resetErr();
    }
    void pipeOut(Process &process)
    {
        resetOut();
        if (process.in)
            process.resetIn();
        outArrow = std::make_unique<Arrow>(getPosition(), process.getPosition() + Point{0, 2}, [this]() {
            resetOut();
        });
        out = &process;
        process.in = this;
    }
    void pipeErr(Process &process)
    {
        resetErr();
        if (process.in)
            process.resetIn();
        errArrow = std::make_unique<Arrow>(getPosition() + Point{0, 1}, process.getPosition() + Point{0, 2}, [this]() {
            resetErr();
        });
        err = &process;
        process.in = this;
    }

private:
    void draw(Renderer &renderer) const override
    {
        if (outArrow)
            renderer.draw(*outArrow);
        if (errArrow)
            renderer.draw(*errArrow);
        Box::draw(renderer);
    }
    void resetIn()
    {
        if (in && in->out == this)
        {
            in->outArrow.reset();
            in->out = nullptr;
            in = nullptr;
        }
        if (in && in->err == this)
        {
            in->errArrow.reset();
            in->err = nullptr;
            in = nullptr;
        }
    }
    void resetOut()
    {
        if (out)
        {
            out->in = nullptr;
            outArrow.reset();
            out = nullptr;
        }
    }
    void resetErr()
    {
        if (err)
        {
            err->in = nullptr;
            errArrow.reset();
            err = nullptr;
        }
    }
    std::unique_ptr<Arrow> outArrow, errArrow;
    Process *in, *out, *err;
};

#endif /* !PROCESS_H_ */
