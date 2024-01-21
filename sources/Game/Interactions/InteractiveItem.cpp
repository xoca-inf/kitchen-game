#include "InteractiveItem.h"

InteractiveItem::InteractiveItem(sptr<VisibleNode> node, sptr<VisibleNode> disappear_node) {
    m_node = node;
    m_disappear_node = disappear_node;
}

void InteractiveItem::onAwake() {
    W4_LOG_DEBUG("name %s + awake on interaction", m_node->getName().c_str());
}

void InteractiveItem::update(float dt) {

}

void InteractiveItem::onInteract(std::function<void()> onInteractComplete) {

}