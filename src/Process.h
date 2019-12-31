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
        setContent("echo \"Hello world!\"");
    }
    virtual ~Process()
    {
        resetIn();
        resetOut();
        resetErr();
    }
    virtual void pipeOut(Process &process)
    {
        resetOut();
        outArrow = std::make_unique<Arrow>(getPosition(), process.getPosition() + Point{0, 2}, [this]() {
            resetOut();
        });
        out = &process;
        process.pipeIn(*this);
    }
    virtual void pipeErr(Process &process)
    {
        resetErr();
        errArrow = std::make_unique<Arrow>(getPosition() + Point{0, 1}, process.getPosition() + Point{0, 2}, [this]() {
            resetErr();
        });
        err = &process;
        process.pipeIn(*this);
    }
    Process *getErr() const
    {
        return err;
    }
    Process *getOut() const
    {
        return out;
    }
    Process *getIn() const
    {
        return in;
    }
    std::string pipeNameIn() const
    {
        std::string ret;
        if (in)
        {
            if (in->getOut() == this)
                ret = in->getOut()->getContent() + std::to_string(in->getOut()->getIndex()) + "OutTo";
            if (in->getErr() == this)
                ret = in->getErr()->getContent() + std::to_string(in->getErr()->getIndex()) + "ErrTo";
            ret += getContent() + std::to_string(getIndex()) + "In";

            ret.erase(std::remove_if(ret.begin(), ret.end(), [](auto c) {
                          return !std::isalnum(c);
                      }),
                      ret.end());
            ret += ".linuxPipelinesPipe";
        }

        return ret;
    }
    std::string pipeNameOut() const
    {
        if (out)
            return out->pipeNameIn();
        return "";
    }
    std::string pipeNameErr() const
    {
        if (out)
            return out->pipeNameErr();
        return "";
    }
    static void swap(Process &a, Process &b)
    {
        Box::swap(a, b);
        a.refreshPipes();
        b.refreshPipes();
    }

protected:
    virtual void pipeIn(Process &process)
    {
        if (in)
            resetIn();
        in = &process;
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

private:
    void refreshPipes(bool recursive = true)
    {
        if (out)
            pipeOut(*out);
        if (err)
            pipeErr(*err);
        if (in && recursive)
            in->refreshPipes();
    }
    void draw(Renderer &renderer) const override
    {
        if (outArrow)
            renderer.draw(*outArrow);
        if (errArrow)
            renderer.draw(*errArrow);
        Box::draw(renderer);
    }
    std::unique_ptr<Arrow> outArrow, errArrow;
    Process *in, *out, *err;
};

#endif /* !PROCESS_H_ */
