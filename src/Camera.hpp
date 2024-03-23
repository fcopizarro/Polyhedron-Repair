#pragma once

#include "common.hpp"


class Camera{

    public:

        Camera();
        Camera(float, float, float);

        void EventListener(SDL_Event&);
        void UpdateCamera();

    private:

        float cameraAngleX;
        float cameraAngleY;
        float sensitivity;



};
