#pragma once

#include "W4Framework.h"
#include "AbstractMerge.h"
#include "../SpriteManager.h"

W4_USE_UNSTRICT_INTERFACE

class Merge : public AbstractMerge
{
public:
    Merge(vec2 dim, std::string&& name, sptr<SpriteManager> manager, std::function<void(sptr<class Item>)> callback, std::function<void()> callbackAnimationFinished);
    void initialize();
    void destruct(sptr<AbstractItem> finalItem) override;
    void update(float dt);
private:
    void startAnimation();
    void endAnimation();

    sptr<SpriteManager> m_manager;
    sptr<Node> m_self;
    std::string m_spriteName;

    std::function<void(sptr<class Item>)> m_callback;
    std::function<void()>m_callbackAnimationFinished;

    sptr<Tween<vec2>> m_tween;
    float m_tweenTimeout = 1.5f;
    float m_tweenTime = 0;
    bool m_isBack = false;
};
