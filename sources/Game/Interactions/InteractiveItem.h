#pragma once

#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

class InteractiveItem
{
public:
    explicit InteractiveItem(sptr<VisibleNode> node, sptr<VisibleNode> disappear_node);
    sptr<VisibleNode> m_node;
    sptr<VisibleNode> m_disappear_node;
    virtual void onAwake();
    virtual void update(float dt);
    virtual void onInteract(std::function<void()> onInteractComplete);
    std::function<void()> m_onInteractComplete;
};
