#pragma once    

#include "common.hpp"
#include "camera/Camera.hpp"
#include "models/models.hpp"
#include "ui/ui.hpp"

class Event
{
private:
    /* data */
public:
    void event();
    void handle(bool&, Camera&, Model&, UI&);

};
