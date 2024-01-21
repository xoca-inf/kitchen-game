#pragma once
#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

#include "Settings.h"

class Config
{
public:
    static void initialize();
    static const Settings& getGameSettings(); //TODO: remove extra class

private:
    static Settings m_settings;
};
