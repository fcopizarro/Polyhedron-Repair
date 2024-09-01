#pragma once    

#include "common.hpp"
#include "ui/ui.hpp"
#include "scene/scene.hpp"

class Event
{
private:
    /* data */
public:
    void event();
    void handle(bool&, Scene&, UI&);

};
