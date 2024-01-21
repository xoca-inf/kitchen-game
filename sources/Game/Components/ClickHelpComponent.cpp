#include "ClickHelpComponent.h"

void ClickHelpComponent::initialize(const variant::Variant &data) {

}

void ClickHelpComponent::setup(sptr<SpriteManager> spriteManager) {
    m_hand = spriteManager->initializeSprite("hand_unclicked", false, this->getOwner().getRoot().get());
    m_textureManager = spriteManager->getTextureManager();
}

void ClickHelpComponent::setClickAnim(vec2 startPos, sptr<Node> targetNode){
    W4_LOG_DEBUG("Set click anim");
    m_targetNode = targetNode;
    initTargetScale = targetNode->getWorldScale();
    m_tween_time = 0;
    m_tween = make::sptr<Tween<vec2>>(startPos);
    m_tween->add(startPos, m_fadeTween_timeout, Easing::Linear);
    m_tween->add(targetNode->getWorldTranslation().xy, m_tween_timeout + m_fadeTween_timeout, Easing::Linear);
    m_isHandClicked = false;
    show();
}

void ClickHelpComponent::disappear() {
    m_fadeTween_time = 0;
    m_fade_tween = make::sptr<Tween<float>>(1);
    m_fade_tween->add(0, m_fadeTween_timeout, Easing::Linear);
}

void ClickHelpComponent::show() {
    W4_LOG_DEBUG("show click");
    m_fadeTween_time = 0;
    m_hand->setEnabled(true);
    m_fade_tween = make::sptr<Tween<float>>(0);
    m_fade_tween->add(1, m_fadeTween_timeout, Easing::Linear);
}

void ClickHelpComponent::clickItem() {
    m_hand->getMaterialInst()->setTexture(w4::resources::TextureId::TEXTURE_0, m_textureManager->get(clickHandName));
    m_targetTween_time = 0;
    m_targetTween = make::sptr<Tween<float>>(1);
    m_targetTween->add(1.2f, m_targetTween_timeout / 2, Easing::Linear);
    m_targetTween->add(1.0f, m_targetTween_timeout, Easing::Linear);
}

void ClickHelpComponent::update(float dt) {
    if(m_tween){
        m_tween_time += dt;
        float z = m_hand->getWorldTranslation().z;
        m_hand->setWorldTranslation(vec3(m_tween->getValue(m_tween_time).x, m_tween->getValue(m_tween_time).y, z));
        if(m_tween_time > m_tween_timeout + m_fadeTween_timeout){
            clickItem();
            m_tween = nullptr;
        }
    }

    if(m_fade_tween){
        m_fadeTween_time += dt;
        m_hand->getMaterialInst()->setParam("u_color", color(1,1,1,m_fade_tween->getValue(m_fadeTween_time)));
        if(m_fadeTween_time > m_fadeTween_timeout){
            m_fade_tween = nullptr;
        }
    }

    if(m_targetTween){
        m_targetTween_time += dt;
        m_targetNode->setWorldScale(initTargetScale*m_targetTween->getValue(m_targetTween_time));
        if(!m_isHandClicked){
            if(m_targetTween_time >= m_targetTween_timeout / 2){
                m_hand->getMaterialInst()->setTexture(w4::resources::TextureId::TEXTURE_0, m_textureManager->get(handName));
                m_isHandClicked = true;
            }
        }
        if(m_targetTween_time > m_targetTween_timeout){
            m_targetTween = nullptr;
            disappear();
        }
    }
}

void ClickHelpComponent::finalize() {

}





