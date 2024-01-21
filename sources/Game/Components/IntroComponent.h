#pragma once
#include "W4Framework.h"
#include "Game/SpriteManager.h"

W4_USE_UNSTRICT_INTERFACE

class IntroComponent : public IComponent{
W4_COMPONENT(IntroComponent, IComponent)
public:
    IntroComponent(CloneTag, const IntroComponent& from, Node& owner)
            : Super(CloneTag{}, from, owner)
    {
    }

    void initialize(const variant::Variant& data) override;
    void update(float dt) override;
    void finalize() override;
    ~IntroComponent() override;

    void setup(sptr<SpriteManager> spriteManager, sptr<Node> root, std::function<void()> callback);

    void startWomanAnimation();
    void endWomanAnimation();
    void startBoardAnimation();
    void endBoardAnimation();
    void startRepairAnimation();
    void endRepairAnimation();
    void addCollider();

private:
    std::function<void()> m_callback;
    sptr<Node> m_root;
    sptr<SpriteManager> m_spriteManager;
    sptr<VisibleNode> m_sad;
    sptr<VisibleNode> m_whatHappend;
    sptr<VisibleNode> m_repairKitchen;

    sptr<Tween<vec2>> m_womanTween;
    float m_womanTween_timeout = 0.5f;
    float m_womanTween_time = 0;
    bool m_isWomanBack = false;

    sptr<Tween<vec2>> m_boardTween;
    float m_boardTween_timeout = 0.5f;
    float m_boardTween_time = 0;
    bool m_isBoardBack = false;

    sptr<Tween<vec2>> m_repairTween;
    float m_repairTween_timeout = 0.5f;
    float m_repairTween_time = 0;
    bool m_isRepairBack = false;
    bool m_isClickStarted = false;
};

