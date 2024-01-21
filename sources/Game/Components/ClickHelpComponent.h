#pragma once
#include "W4Framework.h"
#include "Game/SpriteManager.h"

W4_USE_UNSTRICT_INTERFACE

class ClickHelpComponent : public IComponent{
W4_COMPONENT(ClickHelpComponent, IComponent)
public:
    ClickHelpComponent(CloneTag, const ClickHelpComponent& from, Node& owner)
            : Super(CloneTag{}, from, owner)
    {
    }

    void initialize(const variant::Variant& data) override;
    void update(float dt) override;
    void finalize() override;
    ~ClickHelpComponent() override = default;

    void setup(sptr<SpriteManager> spriteManager);

    void setClickAnim(vec2 startPos, sptr<Node> targetNode);

private:

    void clickItem();
    void show();
    void disappear();

    std::string clickHandName = "hand_clicked";
    std::string handName = "hand_unclicked";

    sptr<VisibleNode> m_hand;
    sptr<LoadingTextures> m_textureManager;

    sptr<Node> m_targetNode;
    vec3 initTargetScale;
    sptr<Tween<float>> m_targetTween;
    float m_targetTween_timeout = 0.5f;
    float m_targetTween_time = 0;

    sptr<Tween<vec2>> m_tween;
    float m_tween_timeout = 1.5f;
    float m_tween_time = 0;

    sptr<Tween<float>> m_fade_tween;
    float m_fadeTween_timeout = 0.5f;
    float m_fadeTween_time = 0;

    bool m_isHandClicked = false;
};

