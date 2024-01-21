#pragma once
#include "W4Framework.h"
#include "SettingsContainer.h"

W4_USE_UNSTRICT_INTERFACE

class ResourceLibary {
public:
    static ResourceLibary& instance() {
        static ResourceLibary instance;
        return instance;
    }

    ResourceLibary();
    virtual ~ResourceLibary();

    sptr<Asset>        getModel(const std::string& key)        {return Asset::get(m_modelsData[key]);}
    sptr<Material>     getMaterial(const std::string& key)     {return Material::get(m_materialsData[key]);}
    sptr<Texture>      getTexture(const std::string& key)      {return Texture::get(m_texturesData[key]);}

    std::string       getModelPath(const std::string& key)     {return m_modelsData[key];}
    std::string       getMaterialPath(const std::string& key)  {return m_materialsData[key];}
    std::string       getTexturePath(const std::string& key)   {return m_texturesData[key];}

private:
    SettingsContainer<std::string>  m_modelsData;
    SettingsContainer<std::string>  m_materialsData;
    SettingsContainer<std::string>  m_texturesData;
};