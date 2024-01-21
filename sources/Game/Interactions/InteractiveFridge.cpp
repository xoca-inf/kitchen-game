#include "InteractiveFridge.h"

void InteractiveFridge::onAwake() {
    W4_LOG_DEBUG("FRIIIDGE AWAKE");
}

void InteractiveFridge::update(float dt)
{
    InteractiveItem::update(dt);
    if (m_FridgeTween)
    {
        installTween_time += dt;
        m_node->setLocalScale(vec3(m_FridgeTween->getValue(installTween_time).x, m_FridgeTween->getValue(installTween_time).y, 1));
        if(installTween_time > installTween_timeout){
            m_FridgeTween = nullptr;
            m_onInteractComplete();
            return;
        }
    }
    if (m_BrokenFridgeTween){
        brokenTween_time += dt;
        m_disappear_node->setLocalScale(vec3(m_BrokenFridgeTween->getValue(brokenTween_time).x, m_BrokenFridgeTween->getValue(brokenTween_time).y, 1));
        if(brokenTween_time > brokenTween_timeout){
            m_BrokenFridgeTween = nullptr;
            auto initScale = m_node->getWorldScale();
            m_node->setWorldScale(vec3(0,0,0));
            m_node->setEnabled(true);
            m_FridgeTween = make::sptr<Tween<vec2>>(vec2(0,0));
            m_FridgeTween->add(vec2(initScale.x, initScale.y), installTween_timeout, easing::Linear<vec2>);
        }
    }
}

void InteractiveFridge::onInteract(std::function<void()> onInteractComplete)
{
    W4_LOG_DEBUG("FRIIIDGE");
    m_onInteractComplete = onInteractComplete;
    auto brokenInitScale = m_disappear_node->getWorldScale();
    m_BrokenFridgeTween = make::sptr<Tween<vec2>>(vec2(brokenInitScale.x,brokenInitScale.y));
    m_BrokenFridgeTween->add(vec2(0, 0), brokenTween_timeout, easing::Linear<vec2>);

    auto emitter = ParticlesEmitter::get("particles/intersection_success.part");
    emitter->setWorldScale(vec3(1.0f / 200.0f));
    emitter->setWorldTranslation(m_node->getWorldTranslation());
    emitter->setRenderOrder(15);
    emitter->setTransformMode(ParticlesEmitterParameters::TransformMode::EMITTER);
    Render::getRoot()->addChild(emitter);
    emitter->start();
}