#pragma once

#include "Layer.h"

class AppLayer : public Layer
{
public:
    AppLayer ();
    ~AppLayer ();

    virtual void OnUpdate (double ts) override;
    virtual void OnRender () override;
};
