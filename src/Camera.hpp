#pragma once

#include "common.hpp"
#include "shader/Shader.hpp"



class Camera{

    public:

        // Stores the main vectors of the camera
        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

        // Prevents the camera from jumping around when first clicking left click
        bool firstClick = true;

        // Stores the width and height of the window
        int width;
        int height;

        // Adjust the speed of the camera and it's sensitivity when looking around
        float speed = 0.1f;
        float sensitivity = 1.0f;

        // Camera constructor to set up initial values
        Camera(int width, int height, glm::vec3 position);

        // Updates and exports the camera matrix to the Vertex Shader
        void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& , const char*);
        //void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
        // Handles camera inputs
        void Inputs(SDL_Event&);

        glm::vec3 GetPosition();
        float getX() const { return Position.x; }
        float getY() const { return Position.y; }
        float getZ() const { return Position.z; }

        glm::vec3 mPosition;
        glm::vec3 mFrontDir;
        glm::vec3 mUpDir{};
        glm::vec3 mRightDir{};
        
        float mYaw;
        float mPitch;

        static constexpr float DefaultYaw{ -90.0f };
        static constexpr float DefaultPitch{};
        static constexpr float DefaultSpeed{ 50.f };
        static constexpr float DefaultSensitivity{ 0.1f };
        static constexpr float DefaultZoom{ 35.0f };

        //void EventListener(SDL_Event&);
        //void UpdateCamera();
    private:
        bool middleButtonPressed = false;


};
