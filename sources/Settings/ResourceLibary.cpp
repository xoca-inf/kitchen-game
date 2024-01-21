#include "ResourceLibary.h"

ResourceLibary::ResourceLibary()
{
    auto file = w4::filesystem::open("resources.json");
    auto json = nlohmann::json::parse(file->data(), file->data() + file->size());
    m_modelsData.init(json.at("models"));
    m_materialsData.init(json.at("materials"));
    m_texturesData.init(json.at("textures"));
    W4_LOG_DEBUG("[RESOURCE LIBARY] Loaded");
}

ResourceLibary::~ResourceLibary() {}