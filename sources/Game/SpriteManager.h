#pragma once

#include "W4Framework.h"
#include "W4JSON.h"
#include "Loading/LoadingTextures.h"
#include <unordered_map>

W4_USE_UNSTRICT_INTERFACE

class SpriteManager
{
public:
    SpriteManager(std::string screenConfigPath, bool currentOrientation);

    sptr<VisibleNode> initializeSprite(std::string name, bool isEnabled, Node * const root);
    void refresh();
    void changeOrientation(bool currentOrientation);
    void setTextureManager(sptr<LoadingTextures> textureManager);
    sptr<LoadingTextures> getTextureManager();
private:
    void initializeJson();
    void updateSprite(const std::string & name, const nlohmann::json& value);
private:
    std::string m_screenConfigPath;
    bool m_currentOrientation;

    sptr<LoadingTextures> m_textureManager;

    nlohmann::json m_jsonPortrait;
    nlohmann::json m_jsonLandscape;

    std::unordered_map<std::string, sptr<Node>> m_widgets;
};
