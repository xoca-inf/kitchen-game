#include "IntroComponent.h"
#include "Game/GameSettings/GameSettings.h"
#include "ClickHelpComponent.h"
#include "Game/AudioManager.h"


void IntroComponent::initialize(const variant::Variant& data) {
}

void IntroComponent::setup(sptr<SpriteManager> spriteManager, sptr<Node> root, std::function<void()> callback) {
    m_spriteManager = spriteManager;
    m_root = root;
    m_callback = callback;
    m_sad = m_spriteManager->initializeSprite("normal", true, m_root.get());
    m_whatHappend = m_spriteManager->initializeSprite("what_happend", true, m_root.get());
    m_repairKitchen = m_spriteManager->initializeSprite("repair_kitchen", true, m_root.get());
    startWomanAnimation();
    startBoardAnimation();
}

void IntroComponent::addCollider(){
    auto screenEventItem = [this](const CollisionInfo& info)
    {
//        auto* node = info.target->getParent();
        m_callback();
        m_isRepairBack = true;
        m_repairTween_time = 0;
        endRepairAnimation();
        AudioManager::instance().playClickSound();
    };
    auto collider = m_repairKitchen->addCollider<core::AABB>(m_repairKitchen->getName() + "_collider", 1.0f);
    collider->setScreencastCallback(ScreencastEvent::Down, screenEventItem);
}

void IntroComponent::startBoardAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("start_board_position");
    vec2 endPosition = GameSettings::instance().getVec2("end_board_position");
    m_boardTween_timeout = GameSettings::instance().getFloat("board_duration");
    m_boardTween = make::sptr<Tween<vec2>>(startPosition);
    m_boardTween->add(endPosition, m_boardTween_timeout, easing::Linear<vec2>);
}

void IntroComponent::endBoardAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("end_board_position");
    vec2 endPosition = GameSettings::instance().getVec2("start_board_position");
    auto stayDuration = GameSettings::instance().getFloat("board_stay_duration");
    auto boardDuration = GameSettings::instance().getFloat("board_duration");
    m_boardTween_timeout = boardDuration + stayDuration;
    m_boardTween = make::sptr<Tween<vec2>>(startPosition);
    m_boardTween->add(startPosition, stayDuration + boardDuration / 2, easing::Linear<vec2>);
    m_boardTween->add(endPosition, m_boardTween_timeout, easing::Linear<vec2>);
}

void IntroComponent::startWomanAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("start_woman_position");
    vec2 endPosition = GameSettings::instance().getVec2("end_woman_position");
    m_womanTween_timeout = GameSettings::instance().getFloat("woman_duration");
    m_womanTween = make::sptr<Tween<vec2>>(startPosition);
    m_womanTween->add(startPosition, m_womanTween_timeout/2, easing::Linear<vec2>);
    m_womanTween->add(endPosition, m_womanTween_timeout, easing::Linear<vec2>);
}

void IntroComponent::endWomanAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("end_woman_position");
    vec2 endPosition = GameSettings::instance().getVec2("start_woman_position");
    auto stayDuration = GameSettings::instance().getFloat("woman_stay_duration");
    auto womanDuration = GameSettings::instance().getFloat("woman_duration");
    m_womanTween_timeout = womanDuration + stayDuration;
    m_womanTween = make::sptr<Tween<vec2>>(startPosition);
    m_womanTween->add(startPosition, stayDuration + womanDuration / 2, easing::Linear<vec2>);
    m_womanTween->add(endPosition, m_womanTween_timeout, easing::Linear<vec2>);
}

void IntroComponent::startRepairAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("start_repair_position");
    vec2 endPosition = GameSettings::instance().getVec2("end_repair_position");
    m_repairTween_timeout = GameSettings::instance().getFloat("repair_duration");
    m_repairTween = make::sptr<Tween<vec2>>(startPosition);
    m_repairTween->add(startPosition, m_repairTween_timeout/2, easing::Linear<vec2>);
    m_repairTween->add(endPosition, m_repairTween_timeout, easing::Linear<vec2>);
    addCollider();
}

void IntroComponent::endRepairAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("end_repair_position");
    vec2 endPosition = GameSettings::instance().getVec2("start_repair_position");
    auto repairDuration = GameSettings::instance().getFloat("repair_duration");
    m_repairTween_timeout = repairDuration;
    m_repairTween = make::sptr<Tween<vec2>>(startPosition);
    m_repairTween->add(endPosition, m_repairTween_timeout, easing::Linear<vec2>);
}

void IntroComponent::update(float dt) {
    IComponent::update(dt);
    if(m_womanTween){
        m_womanTween_time += dt;
        vec2 pos = m_womanTween->getValue(m_womanTween_time);
        auto z = m_sad->getWorldTranslation().z;
        m_sad->setWorldTranslation(vec3(pos.x,pos.y,z));
        if(m_womanTween_time > m_womanTween_timeout){
            if(m_isWomanBack){
                m_womanTween = nullptr;
            }
            else{
                m_isWomanBack = true;
                m_womanTween_time = 0;
                endWomanAnimation();
            }
        }
    }
    if(m_boardTween){
        m_boardTween_time += dt;
        vec2 pos = m_boardTween->getValue(m_boardTween_time);
        auto z = m_whatHappend->getWorldTranslation().z;
        m_whatHappend->setWorldTranslation(vec3(pos.x,pos.y,z));
        if(m_boardTween_time > m_boardTween_timeout){
            if(m_isBoardBack){
                m_boardTween = nullptr;
                startRepairAnimation();
            }
            else{
                m_isBoardBack = true;
                m_boardTween_time = 0;
                endBoardAnimation();
            }
        }
    }
    if(m_repairTween){
        m_repairTween_time += dt;
        vec2 pos = m_repairTween->getValue(m_repairTween_time);
        auto z = m_repairKitchen->getWorldTranslation().z;
        m_repairKitchen->setWorldTranslation(vec3(pos.x,pos.y,z));
        if(m_repairTween_time > m_repairTween_timeout){
            if(m_isRepairBack){
                m_repairTween = nullptr;
            }
            else{
                if(!m_isClickStarted){
                    m_root->getFirstComponent<ClickHelpComponent>().setClickAnim(vec2(0,0), m_repairKitchen);
                    m_isClickStarted = true;
                }
            }
        }
    }
}

void IntroComponent::finalize() {
    IComponent::finalize();
    W4_LOG_DEBUG("[IntroComponent] FINALIZE called!");
}

IntroComponent::~IntroComponent() {
    W4_LOG_DEBUG("[IntroComponent] Destructor called!");
}