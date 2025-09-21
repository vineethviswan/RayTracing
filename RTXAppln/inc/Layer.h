#pragma once

#include <string>

class Layer
{
public:
    virtual void OnUpdate (float deltaTime) = 0;
    virtual void OnRender () = 0;
};
