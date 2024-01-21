#include "Settings.h"

Settings::Settings()
{
    auto file = w4::filesystem::open("settings.json");
    auto json = nlohmann::json::parse(file->data(), file->data() + file->size());
    m_intData.init(json.at("int"));
    m_floatData.init(json.at("float"));
    m_stringData.init(json.at("string"));
    m_vec2Data.init(json.at("vector2"));
    m_vec3Data.init(json.at("vector3"));
    m_vec4Data.init(json.at("vector4"));
    W4_LOG_DEBUG("[GAME SETTINGS] Loaded");
}

Settings::~Settings() {}
