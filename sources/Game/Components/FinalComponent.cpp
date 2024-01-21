#include "FinalComponent.h"
#include "Game/GameSettings/GameSettings.h"


void FinalComponent::initialize(const variant::Variant& data) {
}

void FinalComponent::setup(sptr<SpriteManager> spriteManager, bool does_win, std::function<void()> callback) {
    m_spriteManager = spriteManager;
    m_root = this->getOwner().getRoot();
    m_callback = callback;
    m_woman = m_spriteManager->initializeSprite(does_win ? "happy" : "sad", true, m_root.get());
    m_board = m_spriteManager->initializeSprite(does_win ? "win" : "fail", true, m_root.get());
    startBoardAnimation();
    startWomanAnimation();
}

void FinalComponent::startBoardAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("final_board_begin_position");
    vec2 endPosition = GameSettings::instance().getVec2("final_board_end_position");
    m_boardTween_timeout = GameSettings::instance().getFloat("board_duration");
    m_boardTween = make::sptr<Tween<vec2>>(startPosition);
    m_boardTween->add(endPosition, m_boardTween_timeout, easing::Linear<vec2>);
}

void FinalComponent::endBoardAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("final_board_end_position");
    vec2 endPosition = GameSettings::instance().getVec2("final_board_begin_position");
    auto stayDuration = GameSettings::instance().getFloat("final_board_stay_duration");
    auto boardDuration = GameSettings::instance().getFloat("board_duration");
    m_boardTween_timeout = boardDuration + stayDuration;
    m_boardTween = make::sptr<Tween<vec2>>(startPosition);
    m_boardTween->add(startPosition, stayDuration + boardDuration / 2, easing::Linear<vec2>);
    m_boardTween->add(endPosition, m_boardTween_timeout, easing::Linear<vec2>);
}

void FinalComponent::startWomanAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("final_woman_begin_position");
    vec2 endPosition = GameSettings::instance().getVec2("final_woman_end_position");
    m_womanTween_timeout = GameSettings::instance().getFloat("woman_duration");
    m_womanTween = make::sptr<Tween<vec2>>(startPosition);
    m_womanTween->add(startPosition, m_womanTween_timeout/2, easing::Linear<vec2>);
    m_womanTween->add(endPosition, m_womanTween_timeout, easing::Linear<vec2>);
}

void FinalComponent::endWomanAnimation(){
    vec2 startPosition = GameSettings::instance().getVec2("final_woman_end_position");
    vec2 endPosition = GameSettings::instance().getVec2("final_woman_begin_position");
    auto stayDuration = GameSettings::instance().getFloat("final_woman_stay_duration");
    auto womanDuration = GameSettings::instance().getFloat("woman_duration");
    m_womanTween_timeout = womanDuration + stayDuration;
    m_womanTween = make::sptr<Tween<vec2>>(startPosition);
    m_womanTween->add(startPosition, stayDuration + womanDuration / 2, easing::Linear<vec2>);
    m_womanTween->add(endPosition, m_womanTween_timeout, easing::Linear<vec2>);
}

void FinalComponent::update(float dt) {
    IComponent::update(dt);
    if(m_womanTween){
        m_womanTween_time += dt;
        vec2 pos = m_womanTween->getValue(m_womanTween_time);
        auto z = m_woman->getWorldTranslation().z;
        m_woman->setWorldTranslation(vec3(pos.x,pos.y,z));
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
        auto z = m_board->getWorldTranslation().z;
        m_board->setWorldTranslation(vec3(pos.x,pos.y,z));
        if(m_boardTween_time > m_boardTween_timeout){
            if(m_isBoardBack){
                m_boardTween = nullptr;
                m_callback();
            }
            else{
                m_isBoardBack = true;
                m_boardTween_time = 0;
                endBoardAnimation();
            }
        }
    }
}

void FinalComponent::finalize() {
    IComponent::finalize();
    W4_LOG_DEBUG("[IntroComponent] FINALIZE called!");
}

FinalComponent::~FinalComponent() {
    W4_LOG_DEBUG("[IntroComponent] Destructor called!");
}