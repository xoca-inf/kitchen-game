#include "InteractivePlate.h"

void InteractivePlate::onAwake() {
}

void InteractivePlate::update(float dt)
{
    InteractiveItem::update(dt);
    if (m_PlateTween)
    {
        installTween_time += dt;
        m_disappear_node->getMaterialInst()->setParam("u_color", color(1,1,1,m_PlateTween->getValue(installTween_time)));
        if(installTween_time > installTween_timeout){
            m_PlateTween = nullptr;
            m_onInteractComplete();
            return;
        }
    }
}

void InteractivePlate::onInteract(std::function<void()> onInteractComplete)
{
    m_onInteractComplete = onInteractComplete;
    W4_LOG_DEBUG("plate interacted!");
    auto initScale = m_node->getWorldScale();
    m_PlateTween = make::sptr<Tween<float>>(1);
    m_PlateTween->add(0, installTween_timeout, easing::Linear<float>);

    auto emitter = ParticlesEmitter::get("particles/intersection_success.part");
    emitter->setWorldScale(vec3(1.0f / 200.0f));
    emitter->setWorldTranslation(m_node->getWorldTranslation());
    emitter->setRenderOrder(15);
    emitter->setTransformMode(ParticlesEmitterParameters::TransformMode::EMITTER);
    Render::getRoot()->addChild(emitter);
    emitter->start();
}