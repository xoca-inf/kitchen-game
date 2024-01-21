#include "InteractiveSink.h"
#include "Game/AudioManager.h"

void InteractiveSink::onAwake() {

}

void InteractiveSink::update(float dt)
{
    InteractiveItem::update(dt);
    if (m_SinkTween)
    {
        installTween_time += dt;
        m_disappear_node->getMaterialInst()->setParam("u_color", color(1,1,1,m_SinkTween->getValue(installTween_time)));
        if(installTween_time > installTween_timeout){
            m_SinkTween = nullptr;
            m_onInteractComplete();
            return;
        }
    }
}

void InteractiveSink::onInteract(std::function<void()> onInteractComplete)
{
    AudioManager::instance().stopWater();
    m_onInteractComplete = onInteractComplete;
    W4_LOG_DEBUG("SINK interacted!");
    m_SinkTween = make::sptr<Tween<float>>(1);
    m_SinkTween->add(0, installTween_timeout, easing::Linear<float>);

    auto emitter = ParticlesEmitter::get("particles/intersection_success.part");
    emitter->setWorldScale(vec3(1.0f / 200.0f));
    emitter->setWorldTranslation(m_disappear_node->getWorldTranslation());
    emitter->setRenderOrder(15);
    emitter->setTransformMode(ParticlesEmitterParameters::TransformMode::EMITTER);
    Render::getRoot()->addChild(emitter);
    emitter->start();
}