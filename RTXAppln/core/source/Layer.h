#pragma once

class Layer
{
public:
    virtual ~Layer () = default;

    virtual void OnAttach () { }
    virtual void OnUpdate (double ts) { }
    virtual void OnRender () { }
};
