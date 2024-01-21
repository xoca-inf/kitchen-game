#include "LoadingTextures.h"

void LoadingTextures::intialize(std::function<void()> onComplete, std::vector<std::pair<std::string, std::string>> listToLoad)
{
    for (auto& texture : listToLoad)
    {
        m_textures[texture.first] = Texture::get(texture.second);
    }
    onComplete();
}

sptr<Texture> LoadingTextures::get(std::string& key)
{
    return m_textures[key];
}