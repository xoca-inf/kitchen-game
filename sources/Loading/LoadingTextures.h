#pragma once
#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

class LoadingTextures
{
public:
    LoadingTextures() = default;
    void intialize( std::function<void()> onComplete, std::vector<std::pair<std::string, std::string>> listToLoad);
    sptr<Texture> get(std::string& key);
private:
    std::unordered_map<std::string, sptr<Texture>> m_textures;
    float m_time;
    float m_timeout;
};
