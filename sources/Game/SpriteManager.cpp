#include "SpriteManager.h"

SpriteManager::SpriteManager(std::string screenConfigPath, bool currentOrientation) : 
    m_screenConfigPath(screenConfigPath), m_currentOrientation(currentOrientation)
{
    initializeJson();
}

void SpriteManager::initializeJson()
{
    std::string fullPath = m_screenConfigPath + "portrait" + ".json";
    if (auto file = filesystem::open(fullPath); file && file->good())
    {
        m_jsonPortrait = nlohmann::json::parse(file->data(), file->data() + file->size());
    }
    else
    {
        W4_LOG_ERROR("configuration portrait file not found : %s", fullPath.c_str());
    }

    fullPath = m_screenConfigPath + "landscape" + ".json";
    if (auto file = filesystem::open(fullPath); file && file->good())
    {
        m_jsonLandscape = nlohmann::json::parse(file->data(), file->data() + file->size());
    }
    else
    {
        W4_LOG_ERROR("configuration landscape file not found : %s", fullPath.c_str());
    }
}

void SpriteManager::setTextureManager(sptr<LoadingTextures> textureManager)
{
    m_textureManager = textureManager;
}

sptr<LoadingTextures> SpriteManager::getTextureManager(){
    return m_textureManager;
}

sptr<VisibleNode> SpriteManager::initializeSprite(std::string name, bool isEnabled, Node * const root)
{
    nlohmann::json sprite = (m_currentOrientation ? m_jsonPortrait : m_jsonLandscape)["images"][name];
    vec2 scale = sprite["size"].get<vec2>();
    vec3 pos = sprite["position"].get<vec3>();
    int renderOrder = sprite["order"].get<int>();
    auto plane = Mesh::create::plane(vec2(1, 1));
    plane->setWorldTranslation(pos);
    auto m_bgMaterialInst = Material::get("materials/" + sprite["material"].get<std::string>() + ".mat")->createInstance();
    m_bgMaterialInst->setTexture(w4::resources::TextureId::TEXTURE_0, m_textureManager->get(name));
    m_bgMaterialInst->enableBlending(true);
    m_bgMaterialInst->setParam("u_color", color(1,1,1,1));
    plane->setWorldScale(vec3(scale.x, scale.y, 1));
    plane->setMaterialInst(m_bgMaterialInst);
    plane->setRenderOrder(renderOrder);
    root->addChild(plane);
    m_widgets[name] = plane;
    plane->setEnabled(isEnabled);
    plane->setName(name);

    return plane;
}

void SpriteManager::changeOrientation(bool orientation)
{
    if (m_currentOrientation != orientation)
    {
        m_currentOrientation = orientation;        

        for (auto& value : m_widgets)
        {
            updateSprite(value.first, (m_currentOrientation ? m_jsonPortrait : m_jsonLandscape)["images"][value.first]);
        }
    }
}

void SpriteManager::updateSprite(const std::string& name, const nlohmann::json& value)
{
    auto widget = m_widgets[name];
    vec2 scale = value["size"].get<vec2>();
    widget->setWorldScale(vec3(scale.x, scale.y, 1));
    widget->setWorldTranslation(value["position"].get<vec3>());
}

void SpriteManager::refresh()
{
    m_widgets.clear();
}