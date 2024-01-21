#pragma once
#include "W4Framework.h"
#include "Game/SpriteManager.h"

W4_USE_UNSTRICT_INTERFACE

class FinalComponent : public IComponent{
W4_COMPONENT(FinalComponent, IComponent)
public:
    FinalComponent(CloneTag, const FinalComponent& from, Node& owner)
            : Super(CloneTag{}, from, owner)
    {
    }

    void initialize(const variant::Variant& data) override;
    void update(float dt) override;
    void finalize() override;
    ~FinalComponent() override;

    void setup(sptr<SpriteManager> spriteManager, bool does_win, std::function<void()> callback);

    void startWomanAnimation();
    void endWomanAnimation();
    void startBoardAnimation();
    void endBoardAnimation();
    void startRepairAnimation();
    void endRepairAnimation();

private:
    std::function<void()> m_callback;
    sptr<Node> m_root;
    sptr<SpriteManager> m_spriteManager;
    sptr<VisibleNode> m_board;
    sptr<VisibleNode> m_woman;

    sptr<Tween<vec2>> m_womanTween;
    float m_womanTween_timeout = 1.5f;
    float m_womanTween_time = 0;
    bool m_isWomanBack = false;

    sptr<Tween<vec2>> m_boardTween;
    float m_boardTween_timeout = 1.5f;
    float m_boardTween_time = 0;
    bool m_isBoardBack = false;
};

