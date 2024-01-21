#pragma once

#include "W4Framework.h"
#include "InteractiveItem.h"
#include "InteractiveFridge.h"
#include "InteractiveSink.h"
#include "InteractivePlate.h"
#include "Game/AudioManager.h"

W4_USE_UNSTRICT_INTERFACE

class Interaction
{
public:
    ~Interaction();
    Interaction(sptr<Node> selectedItem, sptr<InteractiveItem> interactiveItem, std::function<void()> onIntersect);
    void initialize();
    void update(float dt);
private:
    void initializeHandlers();

    void onTouchBegin(const Touch::Begin& evt);
    void onTouchEnd(const Touch::End& evt);
    void onTouchMove(const Touch::Move& evt);
private:
    sptr<event::Touch::Begin::Handle> m_touchBeginHandler;
    sptr<event::Touch::End::Handle> m_touchEndHandler;
    sptr<event::Touch::Move::Handle> m_touchMoveHandler;

    sptr<Node> m_SelectedItem;
    sptr<Collider> m_selectedItem_collider;

    sptr<InteractiveItem> m_IntersectItem;
    sptr<Collider> m_intersect_collider;

    std::function<void()> m_onIntersect;

    sptr<Camera> m_camera;

    bool m_doesClicked = false;

    vec2 m_prevPoint;
};
