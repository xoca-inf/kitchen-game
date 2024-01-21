#include "Interaction.h"

Interaction::~Interaction()
{
    if (m_touchBeginHandler)
    {
        m_touchBeginHandler->unsubscribe();
        m_touchBeginHandler = nullptr;
    }
    if (m_touchEndHandler)
    {
        m_touchEndHandler->unsubscribe();
        m_touchEndHandler = nullptr;
    }
    if (m_touchMoveHandler)
    {
        m_touchMoveHandler->unsubscribe();
        m_touchMoveHandler = nullptr;
    }
}

Interaction::Interaction(sptr<Node> selectedItem,sptr<InteractiveItem> interactiveItem, std::function<void()> onIntersect) {
    m_SelectedItem = selectedItem;
    m_SelectedItem->setName("selectedItem");

    m_IntersectItem = interactiveItem;
    m_onIntersect = onIntersect;
    m_IntersectItem->onAwake();
    initialize();
}

void Interaction::initialize() {

    auto IntersectEventItem = [this](const CollisionInfo& info)
    {
        auto* node = info.target->getParent();
        W4_LOG_DEBUG(node->getName().c_str());
        if(node->getName() == m_IntersectItem->m_disappear_node->getName()){
            AudioManager::instance().playTransform();
            m_IntersectItem->onInteract(m_onIntersect);
            m_intersect_collider->setIntersecting(false);
            m_selectedItem_collider->setIntersecting(false);
            m_SelectedItem->setEnabled(false);
        }
    };

    m_intersect_collider = m_IntersectItem->m_disappear_node->addCollider<core::AABB>(m_IntersectItem->m_disappear_node->getName() + "_collider", 1.0f);
    m_intersect_collider->setIntersecting();
    m_selectedItem_collider = m_SelectedItem->addCollider<core::AABB>(m_SelectedItem->getName() + "_collider", 1.0f);
    m_selectedItem_collider->setIntersectionBeginCallback(IntersectEventItem);
    m_selectedItem_collider->setReceiveRaycasts(true);

    initializeHandlers();
}

void Interaction::initializeHandlers()
{
    m_camera = Render::getScreenCamera();

    m_touchBeginHandler = Touch::Begin::subscribe(std::bind(&Interaction::onTouchBegin, this, std::placeholders::_1));
    m_touchEndHandler = Touch::End::subscribe(std::bind(&Interaction::onTouchEnd, this, std::placeholders::_1));
    m_touchMoveHandler = Touch::Move::subscribe(std::bind(&Interaction::onTouchMove, this, std::placeholders::_1));
}


void Interaction::onTouchBegin(const Touch::Begin& evt)
{
    auto ray = m_camera->createRayFromScreen(evt.point);
    ray.length = 10;
    auto hit = Render::raycast(ray);
    if (hit.target)
    {
        if (!m_doesClicked && m_SelectedItem && hit.target->getParent()->getName() == "selectedItem")
        {
            m_doesClicked = true;

            auto x = (float)evt.point.x / Platform::getSize().w * 9.0f / 16.0f;
            auto y = (float)evt.point.y / Platform::getSize().h;

            m_prevPoint = vec2(x, y);
        }
    }
}

void Interaction::onTouchMove(const Touch::Move& evt)
{
    if (m_SelectedItem && m_doesClicked)
    {
        auto x = (float)evt.point.x / Platform::getSize().w * 9.0f / 16.0f;
        auto y = (float)evt.point.y / Platform::getSize().h;

        vec3 delta(x - m_prevPoint.x, -y + m_prevPoint.y, 0);
        m_SelectedItem->setWorldTranslation(m_SelectedItem->getWorldTranslation() + delta * 11);

        m_prevPoint = vec2(x, y);
    }
}

void Interaction::onTouchEnd(const Touch::End& evt)
{
    auto ray = m_camera->createRayFromScreen(evt.point);
    ray.length = 10;
    auto hit = Render::raycast(ray);
    if (hit.target)
    {
        if (m_doesClicked && m_SelectedItem)
        {
            m_doesClicked = false;
        }
    }
}

void Interaction::update(float dt) {
    if (m_IntersectItem)
        m_IntersectItem->update(dt);
}