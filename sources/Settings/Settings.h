#pragma once
#include "W4Framework.h"
#include "SettingsContainer.h"

W4_USE_UNSTRICT_INTERFACE

class Settings {
public:
    static Settings& instance() {
        static Settings instance;
        return instance;
    }

    Settings();
    virtual ~Settings();

    int           getInt(const std::string& key)     {return m_intData[key];}
    float         getFloat(const std::string& key)   {return m_floatData[key];}
    std::string   getString(const std::string& key)  {return m_stringData[key];}
    vec2          getVec2(const std::string& key)    {return m_vec2Data[key];}
    vec3          getVec3(const std::string& key)    {return m_vec3Data[key];}
    vec4          getVec4(const std::string& key)    {return m_vec4Data[key];}

private:
    SettingsContainer<int>          m_intData;
    SettingsContainer<float>        m_floatData;
    SettingsContainer<std::string>  m_stringData;
    SettingsContainer<vec2>         m_vec2Data;
    SettingsContainer<vec3>         m_vec3Data;
    SettingsContainer<vec4>         m_vec4Data;
};