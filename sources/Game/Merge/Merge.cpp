#include "Merge.h"
#include "Item.h"
#include "Game/GameSettings/GameSettings.h"

Merge::Merge(vec2 dim, std::string&& name, sptr<SpriteManager> manager, std::function<void(sptr<Item>)> callback, std::function<void()> callbackAnimationFinished) : AbstractMerge(dim)
{
    m_manager = manager;
    m_spriteName = name;
    m_callback = callback;
    m_callbackAnimationFinished = callbackAnimationFinished;
}

void Merge::initialize()
{
    m_self = m_manager->initializeSprite(m_spriteName, true, this);
    startAnimation();
}

void Merge::destruct(sptr<AbstractItem> finalItem)
{
    destroyColliders();

    m_callback(finalItem->as<Item>());

    endAnimation();
}

void Merge::startAnimation()
{
    vec2 startPosition = GameSettings::instance().getVec2("merge_begin_position");
    vec2 endPosition = GameSettings::instance().getVec2("merge_end_position");
    m_tweenTimeout = GameSettings::instance().getFloat("merge_duration");
    m_tween = make::sptr<Tween<vec2>>(startPosition);
    m_tween->add(endPosition, m_tweenTimeout, easing::Linear<vec2>);
}

void Merge::endAnimation()
{
    vec2 startPosition = GameSettings::instance().getVec2("merge_end_position");
    vec2 endPosition = GameSettings::instance().getVec2("merge_begin_position");
    auto boardDuration = GameSettings::instance().getFloat("merge_duration");
    m_tweenTimeout = boardDuration;
    m_tween = make::sptr<Tween<vec2>>(startPosition);
    m_tween->add(endPosition, m_tweenTimeout, easing::Linear<vec2>);
}

void Merge::update(float dt)
{
    if (m_tween)
    {
        m_tweenTime += dt;
        vec2 pos = m_tween->getValue(m_tweenTime);
        auto z = getWorldTranslation().z;
        setPos(vec3(pos.x, pos.y, z));
        if (m_tweenTime > m_tweenTimeout)
        {
            if (m_isBack)
            {
                m_tween = nullptr;
                m_callbackAnimationFinished();
            }
            else
            {
                m_tween = nullptr;
                m_isBack = true;
                m_tweenTime = 0;
            }
        }
    }
}
