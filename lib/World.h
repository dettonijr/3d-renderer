#ifndef __WORLD_H__
#define __WORLD_H__

#include "Obj.h"
#include <vector>

class World {
public:
    World();
    ~World();

    void add_obj(Obj& o);
    const std::vector<Obj>& get_objects() const;

private:
    std::vector<Obj> objects;
};

#endif // __WORLD_H__