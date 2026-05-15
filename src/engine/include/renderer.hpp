
//renderer.hpp
#pragma once
 
#include "entity.hpp"
 
class Renderer
{
public:
    static void init();

    static void draw(const Entity& entity);
 
    Renderer()              = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};
 
