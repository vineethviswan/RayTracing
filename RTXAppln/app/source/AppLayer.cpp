
#include "AppLayer.h"
#include "Application.h"
#include "Logger.h"

AppLayer::AppLayer () { Logger::Log (Logger::Level::INFO, "Creating AppLayer"); }

AppLayer::~AppLayer () { }

void AppLayer::OnUpdate (double ts) { }

void AppLayer::OnRender () { }
