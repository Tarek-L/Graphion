
#pragma once 
#include "entity.hpp"

class Renderer {
public:
    void clear() const;

    void draw(const Entity& entity) const;
};
