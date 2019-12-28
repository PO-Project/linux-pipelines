#ifndef TRAVERSABLE_H_
#define TRAVERSABLE_H_

#include <list>
#include <functional>
#include <iterator>
#include <algorithm>
#include "Drawable.h"

enum class GroupMode
{
    TEXT,
    GRAPH,
};

template <GroupMode group>
class Traversable : public Drawable
{
public:
    Traversable() : selected(false)
    {

        traversablesX.push_front(*this);
        traversablesY.push_front(*this);
        myItX = traversablesX.begin();
        myItY = traversablesY.begin();
        if (traversablesX.size() == 1)
        {
            selected = true;
            itX = traversablesX.begin();
            itY = traversablesY.begin();
        }
    }
    virtual ~Traversable()
    {
        if (selected)
        {
            itY = std::next(itY);
            if (itY == traversablesY.end())
                itY = traversablesY.begin();
            itY->get().selected = true;
            itX = itY->get().myItX;
        }
        traversablesX.erase(myItX);
        traversablesY.erase(myItY);
    } 
    bool operator<(const Traversable &rhs) const
    {
        return getAveragePosition().y < rhs.getAveragePosition().y;
    }
    bool operator>(const Traversable &rhs) const
    {
        return getAveragePosition().y > rhs.getAveragePosition().y;
    }
    static void nextX()
    {
        if (traversablesX.size() == 0)
            return;
        traversablesX.sort([](const auto &A, const auto &B) {
            const Traversable &a = A;
            const Traversable &b = B;
            return a.getAveragePosition().x < b.getAveragePosition().x;
        });
        itX->get().selected = false;
        itX = std::next(itX);
        if (itX == traversablesX.end())
            itX = traversablesX.begin();
        itX->get().selected = true;
        itY = itX->get().myItY;
    }
    static void previousX()
    {
        if (traversablesX.size() == 0)
            return;
        traversablesX.sort([](const auto &A, const auto &B) {
            const Traversable &a = A;
            const Traversable &b = B;
            return a.getAveragePosition().x < b.getAveragePosition().x;
        });
        itX->get().selected = false;
        if (itX == traversablesX.begin())
            itX = traversablesX.end();
        itX = std::prev(itX);
        itX->get().selected = true;
        itY = itX->get().myItY;
    }
    static void nextY()
    {
        if (traversablesY.size() == 0)
            return;
        traversablesY.sort([](const auto &A, const auto &B) {
            const Traversable &a = A;
            const Traversable &b = B;
            return a.getAveragePosition().y < b.getAveragePosition().y;
        });
        itY->get().selected = false;
        itY = std::next(itY);
        if (itY == traversablesY.end())
            itY = traversablesY.begin();
        itY->get().selected = true;
        itX = itY->get().myItX;
    }
    static void previousY()
    {
        if (traversablesY.size() == 0)
            return;
        traversablesY.sort([](const auto &A, const auto &B) {
            const Traversable &a = A;
            const Traversable &b = B;
            return a.getAveragePosition().y < b.getAveragePosition().y;
        });
        itY->get().selected = false;
        if (itY == traversablesY.begin())
            itY = traversablesY.end();
        itY = std::prev(itY);
        itY->get().selected = true;
        itX = itY->get().myItX;
    }
    static Traversable *get()
    {
        if (traversablesX.size() == 0)
            return nullptr;
        return &(itX->get());
    }
    bool isSelected() const
    {
        return selected;
    }
    Point getAveragePosition() const
    {
        return getPosition() + getSize() / 2;
    }

private:
    bool selected;

    inline static typename std::list<std::reference_wrapper<Traversable>> traversablesX, traversablesY;
    inline static typename std::list<std::reference_wrapper<Traversable>>::iterator itX, itY;
    typename std::list<std::reference_wrapper<Traversable>>::iterator myItX, myItY;
};

#endif /* !TRAVERSABLE_H_ */
