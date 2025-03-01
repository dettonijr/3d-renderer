#include "World.h"

World::World() {
}

World::~World() {
}

void World::add_obj(Obj& o) {
    objects.push_back(o);
}

const std::vector<Obj>& World::get_objects() const {
    return objects;
}