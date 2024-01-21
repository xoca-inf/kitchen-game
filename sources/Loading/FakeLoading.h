#pragma once

#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

class FakeLoading
{
public:
    FakeLoading(sptr<Node> root);
    ~FakeLoading();
    void initialize(std::function<void()> onComplete);
    void update(float dt);
    void shutdownLoader();
private:

    std::function<void()> m_onComplete;
    float m_time;
    float m_timeout;

    sptr<VisibleNode> m_bg;
};
