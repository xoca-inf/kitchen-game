#include "KitchenMergeGame.h"
#include "Merge/Merge.h"
#include "Merge/Item.h"
#include "Interactions/Interaction.h"
#include "Components/IntroComponent.h"
#include "Components/FinalComponent.h"
#include "Components/ClickHelpComponent.h"
#include "AudioManager.h"

KitchenMergeGame::~KitchenMergeGame()
{
    W4_LOG_DEBUG("destructor");
    m_root->getParent()->removeChild(m_root);

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

void KitchenMergeGame::initialize(std::string screenConfigPath, std::string orientation, sptr<SpriteManager> SpriteManager)
{
    m_configPath = screenConfigPath;
    m_currentOrientation = orientation == "portrait";

    m_camera = Render::getScreenCamera();
    m_camera->setLocalTranslation(vec3(0, 0, -10));

    m_root = make::sptr<Node>();
    Render::getRoot()->addChild(m_root);
    m_spriteManager = SpriteManager;
    m_root->template addComponent<ClickHelpComponent>().setup(m_spriteManager);
    m_root->template addComponent<IntroComponent>().setup(m_spriteManager, m_root, [this]()
        {
            m_root->getFirstComponent<ClickHelpComponent>().setClickAnim(vec2(0,0), m_brokenFridge);
            initializeButtons();
            AudioManager::instance().playWater();
        });
    initializeHandlers();
    initializeSprites();
}

void KitchenMergeGame::initializeButtons()
{
    m_brokenFridge->addCollider<core::AABB>("button", 1.0f)->setScreencastCallback(ScreencastEvent::Down, [this](const CollisionInfo& info)
        {
            if (!m_isMergingNow){
                initializeMerge(MergeTypes::Fridge);
                AudioManager::instance().playClickSound();
            }

        });

    m_brokenSink->addCollider<core::AABB>("button", 1.0f)->setScreencastCallback(ScreencastEvent::Down, [this](const CollisionInfo& info)
        {
            if (!m_isMergingNow){
                initializeMerge(MergeTypes::Sink);
                AudioManager::instance().playClickSound();
            }

        });

    m_fire->addCollider<core::AABB>("button", 1.0f)->setScreencastCallback(ScreencastEvent::Down, [this](const CollisionInfo& info)
        {
            if (!m_isMergingNow){
                initializeMerge(MergeTypes::Plate);
                AudioManager::instance().playClickSound();
            }

        });
}

void KitchenMergeGame::initializeMerge(MergeTypes type)
{
    m_isMergingNow = true;
    m_merge = nullptr;
    m_countMerge++;
    m_merge = make::sptr<Merge>(vec2(4, 4), "work_place", m_spriteManager, [this, type](sptr<Item> item)
        {
            sptr<VisibleNode> from;
            sptr<VisibleNode> to;
            sptr<VisibleNode> disappear;
            std::function<void()> callback = [this]()
            {
                m_interaction = nullptr;
                if (m_countMerge == 3)
                {
                    AudioManager::instance().playWinSound();
                    m_root->template addComponent<FinalComponent>().setup(m_spriteManager, true, [this]()
                        {
                            m_state = GameState::Lose;
                            W4_LOG_ERROR("u lose");
                        });
                }
                m_isMergingNow = false;
            };

            switch (type)
            {
            case MergeTypes::Sink:
                m_boltorez = m_spriteManager->initializeSprite(item->m_spriteName, true, m_root.get());
                disappear = m_brokenSink;
                from = m_boltorez;
                m_interaction = make::sptr<Interaction>(from, make::sptr<InteractiveSink>(nullptr, disappear), callback);
                break;
            case MergeTypes::Fridge:
                m_ploskogubci = m_spriteManager->initializeSprite(item->m_spriteName, true, m_root.get());
                to = m_fridge;
                from = m_ploskogubci;
                disappear = m_brokenFridge;
                m_interaction = make::sptr<Interaction>(from, make::sptr<InteractiveFridge>(to, disappear), callback);
                m_root->getFirstComponent<ClickHelpComponent>().setClickAnim(from->getWorldTranslation().xy, to);
                break;
            case MergeTypes::Plate:
                m_vederko = m_spriteManager->initializeSprite("vederko", true, m_root.get());
                from = m_vederko;
                m_interaction = make::sptr<Interaction>(from, make::sptr<InteractivePlate>(nullptr, m_fire), callback);
                break;
            }
            W4_LOG_DEBUG("-1");
        },
        [this]()
        {
            m_merge->destroy();
            m_merge->setEnabled(false);
            m_root->removeChild(m_merge);
        });
    m_merge->initialize();
    // -----------------------
    m_merge->setPos(vec3(0, 0, 0));
    m_merge->setSize(vec3(4, 4, 1));

    std::vector<sptr<AbstractItem>> items;

    switch (type)
    {
    case MergeTypes::Fridge:
        items.push_back(make::sptr<Item>("otvertka", m_spriteManager));
        items.push_back(make::sptr<Item>("toporik", m_spriteManager));

        items.push_back(make::sptr<Item>("toporik", m_spriteManager));

        items.push_back(make::sptr<Item>("kist", m_spriteManager));
        items.push_back(make::sptr<Item>("nabor_shestigrannikov", m_spriteManager));

        items.push_back(make::sptr<Item>("molotok", m_spriteManager));

        items.push_back(make::sptr<Item>("gubka_high", m_spriteManager));

        items.push_back(make::sptr<Item>("stameska", m_spriteManager));

        items.push_back(make::sptr<Item>("yaschik_closed", m_spriteManager));

        items.push_back(make::sptr<Item>("yaschik_open", m_spriteManager));
        break;
    case MergeTypes::Plate:
        items.push_back(make::sptr<Item>("gubka", m_spriteManager));
        items.push_back(make::sptr<Item>("gubka", m_spriteManager));

        items.push_back(make::sptr<Item>("valik", m_spriteManager));

        items.push_back(make::sptr<Item>("cveti_high", m_spriteManager));

        items.push_back(make::sptr<Item>("cveti_light", m_spriteManager));

        items.push_back(make::sptr<Item>("stameska", m_spriteManager));

        items.push_back(make::sptr<Item>("gubka_high", m_spriteManager));

        items.push_back(make::sptr<Item>("yaschik_closed", m_spriteManager));

        items.push_back(make::sptr<Item>("gaechnii_klyuch", m_spriteManager));
        break;

    case MergeTypes::Sink:
        items.push_back(make::sptr<Item>("otvertka", m_spriteManager));
        items.push_back(make::sptr<Item>("otvertka", m_spriteManager));

        items.push_back(make::sptr<Item>("valik", m_spriteManager));

        items.push_back(make::sptr<Item>("cveti_high", m_spriteManager));

        items.push_back(make::sptr<Item>("cveti_light", m_spriteManager));

        items.push_back(make::sptr<Item>("gubka_high", m_spriteManager));
        items.push_back(make::sptr<Item>("stameska", m_spriteManager));

        items.push_back(make::sptr<Item>("yaschik_closed", m_spriteManager));

        items.push_back(make::sptr<Item>("gaechnii_klyuch", m_spriteManager));

        items.push_back(make::sptr<Item>("gubka", m_spriteManager));
        break;
    default:
        break;
    }

    // -----------------------

    m_merge->initializeItems(items);

    m_root->addChild(m_merge);
}

void KitchenMergeGame::initializeSprites()
{
    m_kitchen = m_spriteManager->initializeSprite("kitchen", true, m_root.get());
    m_boom = m_spriteManager->initializeSprite("boom", false, m_root.get());
    m_brokenFridge = m_spriteManager->initializeSprite("broken_fridge", true, m_root.get());
    m_fridge = m_spriteManager->initializeSprite("fridge", false, m_root.get());
    m_sink = m_spriteManager->initializeSprite("sink", false, m_root.get());
    m_brokenSink = m_spriteManager->initializeSprite("broken_sink", true, m_root.get());
    m_workPlace = m_spriteManager->initializeSprite("work_place", false, m_root.get());
    m_happy = m_spriteManager->initializeSprite("happy", false, m_root.get());
    m_normal = m_spriteManager->initializeSprite("normal", false, m_root.get());
    m_win = m_spriteManager->initializeSprite("win", false, m_root.get());
    m_fail = m_spriteManager->initializeSprite("fail", false, m_root.get());
    m_fire = m_spriteManager->initializeSprite("flame", true, m_root.get());
}

void KitchenMergeGame::initializeCollisions(sptr<Node> selectedItem, sptr<Node> intersectItem, std::function<void()> onIntersect)
{

    auto screenEventItem = [](const CollisionInfo& info)
    {
        auto* node = info.target->getParent();
        node->setWorldTranslation(math::vec3(info.point.x, info.point.y, 0));
    };

    auto IntersectEventItem = [this, onIntersect](const CollisionInfo& info)
    {
        auto* node = info.target->getParent();
        W4_LOG_DEBUG(node->getName().c_str());
        if (node->getName() == m_IntersectItem->getName())
        {
            onIntersect();
            m_intersect_collider->setIntersecting(false);
            m_selectedItem_collider->setIntersecting(false);
            m_SelectedItem->setEnabled(false);
        }
    };
    m_IntersectItem = intersectItem;
    m_SelectedItem = selectedItem;
    m_intersect_collider = m_IntersectItem->addCollider<core::AABB>(m_IntersectItem->getName() + "_collider", m_fridge->as<Mesh>());
    m_intersect_collider->setIntersecting();

    m_selectedItem_collider = m_SelectedItem->addCollider<core::AABB>(m_SelectedItem->getName() + "_collider", 1.0f);
    m_selectedItem_collider->setScreencastCallback(ScreencastEvent::Move, screenEventItem);
    m_selectedItem_collider->setIntersectionBeginCallback(IntersectEventItem);
    m_selectedItem_collider->setBlockScreencasts(false);
}

void KitchenMergeGame::onFridgeIntersect()
{
    auto initScale = m_IntersectItem->getWorldScale();
    m_FridgeTween = make::sptr<Tween<vec2>>(vec2(0, 0));
    m_FridgeTween->add(vec2(initScale.x, initScale.y), installTween_timeout, easing::Linear<vec2>);
}

void KitchenMergeGame::update(float dt)
{
    m_totalTime += dt;

    if (m_brokenFridge)
        m_brokenFridge->getMaterialInst()->setParam("time", m_totalTime);

    if (m_brokenSink)
        m_brokenSink->getMaterialInst()->setParam("time", m_totalTime);

    if (m_fire)
        m_fire->getMaterialInst()->setParam("time", m_totalTime);

    if (m_interaction)
        m_interaction->update(dt);

    if (m_merge)
        m_merge->update(dt);

    if (m_FridgeTween)
    {
        installTween_time += dt;
        m_fridge->setLocalScale(vec3(m_FridgeTween->getValue(installTween_time).x, m_FridgeTween->getValue(installTween_time).y, 1));
        if (installTween_time > installTween_timeout)
            m_FridgeTween = nullptr;
    }
}

void KitchenMergeGame::changeOrientation(std::string orientation)
{
}

void KitchenMergeGame::initializeHandlers()
{
    m_touchBeginHandler = Touch::Begin::subscribe(std::bind(&KitchenMergeGame::onTouchBegin, this, std::placeholders::_1));
    m_touchEndHandler = Touch::End::subscribe(std::bind(&KitchenMergeGame::onTouchEnd, this, std::placeholders::_1));
    m_touchMoveHandler = Touch::Move::subscribe(std::bind(&KitchenMergeGame::onTouchMove, this, std::placeholders::_1));
}

void KitchenMergeGame::onTouchBegin(const Touch::Begin& evt)
{
    auto ray = m_camera->createRayFromScreen(evt.point);
    ray.length = 10;
    auto hit = Render::raycast(ray);
    if (hit.target != nullptr)
    {
        W4_LOG_DEBUG("something hit");
        std::string name = hit.target->getParent()->getName();
        W4_LOG_DEBUG(name.c_str());
    }
}

void KitchenMergeGame::onTouchEnd(const Touch::End& evt)
{
}

void KitchenMergeGame::onTouchMove(const Touch::Move& evt)
{
}

KitchenMergeGame::GameState KitchenMergeGame::getState()
{
    return m_state;
}