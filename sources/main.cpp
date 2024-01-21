#include "W4Framework.h"
#include "Config.h"
#include "Loading/FakeLoading.h"
#include "Game/KitchenMergeGame.h"
#include "Loading/LoadingTextures.h"
#include "Game/SpriteManager.h"
#include "Game/GameSettings/GameSettings.h"

W4_USE_UNSTRICT_INTERFACE

struct KitchenMerge : public IGame
{
    void onStart() override
    {
        Config::initialize();
        GameSettings::instance();
        m_isPortrait = Config::getGameSettings().orientation == "portrait";
        m_waitResize = false;
        W4_LOG_DEBUG("onStart");
        initializeFsm();
    }

    void onUpdate(float dt) override
    {
        m_time += dt;
        m_fsm.update(dt);
    }

    void onResize(const math::size& sz) override
    {
        if (m_waitResize)
        {
            Config::initialize();
            if (m_isPortrait != (Config::getGameSettings().orientation == "portrait"))
            {
                m_isPortrait = Config::getGameSettings().orientation == "portrait";
                m_fsm.processEvent(FSM::Events::Refresh);
            }
        }
    }

    void initializeFsm()
    {
        m_fsm.state<FSM::States::Preloading>().onEnterFunc = [this](auto&, auto&, auto&)
        {
          m_fakeLoading = make::sptr<FakeLoading>(Render::getRoot());

          W4_LOG_INFO("Logo onEnter");
          m_waitResize = false;

          m_fakeLoading->initialize([this]()                 {
            m_textureLoader = make::sptr<LoadingTextures>();

            m_SpriteManager = make::sptr<SpriteManager>("ui/game/", m_isPortrait);
            m_SpriteManager->setTextureManager(m_textureLoader);

            m_textureLoader->intialize([this]()                         {
              m_fsm.processEvent(FSM::Events::Initialize);
              },
                    {
                        {"kitchen", "textures/kitchen.png"},
                        {"boom", "textures/boom.png"},
                        {"broken_fridge", "textures/broken_fridge.png"},
                        {"broken_sink", "textures/broken_sink.png"},
                        {"fail", "textures/fail.png"},
                        {"fridge", "textures/fridge.png"},
                        {"hand_clicked", "textures/hand_clicked.png"},
                        {"hand_unclicked", "textures/hand_unclicked.png"},
                        {"repair_kitchen", "textures/repair_kitchen.png"},
                        {"sink", "textures/sink.png"},
                        {"what_happend", "textures/what_happend.png"},
                        {"win", "textures/win.png"},
                        {"work_place", "textures/work_place.png"},
                        {"cveti_light", "textures/cveti_light.png"},
                        {"cveti_high", "textures/cveti_high.png"},
                        {"gubka_high", "textures/gubka_high.png"},
                        {"vederko", "textures/vederko.png"},
                        {"schetka", "textures/schetka.png"},
                        {"boltorez", "textures/boltorez.png"},
                        {"gaechnii_klyuch", "textures/gaechnii_klyuch.png"},
                        {"razvodnoi_klyuch", "textures/razvodnoi_klyuch.png"},
                        {"valik", "textures/valik.png"},
                        {"ploskogubci", "textures/ploskogubci.png"},
                        {"yaschik_closed", "textures/yaschik_closed.png"},
                        {"yaschik_open", "textures/yaschik_open.png"},
                        {"toporik", "textures/toporik.png"},
                        {"molotok", "textures/molotok.png"},
                        {"kist", "textures/kist.png"},
                        {"otvertka", "textures/otvertka.png"},
                        {"happy", "textures/happy.png"},
                        {"normal", "textures/normal.png"},
                        {"sad", "textures/sad.png"},
                        {"stameska", "textures/stameska.png"},
                        {"nabor_shestigrannikov", "textures/nabor_shestigrannikov.png"},
                        {"flame", "textures/flame.png"},
                        {"gubka", "textures/gubka.png"},
                    });
            });
        };

        m_fsm.state<FSM::States::Preloading>().onUpdateFunc = [this](auto&, float dt)
        {
          m_fakeLoading->update(dt);
        };

        m_fsm.state<FSM::States::Preloading>().onLeaveFunc = [this](auto&, auto&, auto&)
        {
            W4_LOG_INFO("Preloading onLeaveFunc");
        };


        //------------

        m_fsm.state<FSM::States::Logo>().onEnterFunc = [this](auto&, auto&, auto&)
        {
          m_game = make::sptr<KitchenMergeGame>();
          m_game->initialize("ui/game/", Config::getGameSettings().orientation, m_SpriteManager);
          m_fsm.processEvent(FSM::Events::Next);
        };

        m_fsm.state<FSM::States::Logo>().onUpdateFunc = [this](auto&, float dt)
        {
        };

        m_fsm.state<FSM::States::Logo>().onLeaveFunc = [this](auto&, auto&, auto&)
        {
            W4_LOG_INFO("Logo onLeaveFunc");
            m_fakeLoading = nullptr;
        };

        //------------


        //------------

        m_fsm.state<FSM::States::Game>().onEnterFunc = [this](auto&, auto&, auto&)
        {
            W4_LOG_INFO("Game onEnter");
            m_waitResize = true;
            changeScreensOrientation();
        };

        m_fsm.state<FSM::States::Game>().onUpdateFunc = [this](auto&, float dt)
        {
            if (m_game)
            {
                m_game->update(dt);
                switch (m_game->getState())
                {
                case KitchenMergeGame::GameState::Lose:
                  m_fsm.processEvent(FSM::Events::Lose);
                  break;
                }
            }

        };

        m_fsm.state<FSM::States::Game>().onLeaveFunc = [this](auto&, auto&, auto&)
        {
            W4_LOG_INFO("Game onLeaveFunc");
        };

        //------------


        //------------

        m_fsm.state<FSM::States::Lose>().onEnterFunc = [this](auto&, auto&, auto&)
        {
            m_game = nullptr;
            m_waitResize = true;
            m_fsm.processEvent(FSM::Events::Restart);
        };

        m_fsm.state<FSM::States::Lose>().onUpdateFunc = [this](auto&, float dt)
        {
           /* m_finalScreen->update(dt);
            switch (m_finalScreen->getState())
            {
            case ScreenState::Restart:
                m_fsm.processEvent(FSM::Events::Restart);
                break;
            }*/

        };

        m_fsm.state<FSM::States::Lose>().onLeaveFunc = [this](auto&, auto&, auto&)
        {
            W4_LOG_INFO("Lose onLeaveFunc");
        };


        m_fsm.state<FSM::States::GameRestarting>().onEnterFunc = [this](auto&, auto&, auto&)
        {
            changeScreensOrientation();
            m_fsm.processEvent(FSM::Events::Next);
        };

        m_fsm.state<FSM::States::FinalRestarting>().onEnterFunc = [this](auto&, auto&, auto&)
        {
            changeScreensOrientation();
            m_fsm.processEvent(FSM::Events::Next);
        };

        m_fsm.init<FSM::States::Preloading>(FSM::Events::Initialize);
    }

    void changeScreensOrientation()
    {
        if (m_game)
            m_game->changeOrientation(Config::getGameSettings().orientation);
    }


private:
    struct FSM
    {
        enum class Events
        {
            Initialize,
            Next,
            Lose,
            Refresh,
            Restart
        };
        struct States
        {
            struct Preloading : w4::fsm::FuncState<Events>
            {
            };
            struct Logo : w4::fsm::FuncState<Events>
            {
            };
            struct Game : w4::fsm::FuncState<Events>
            {
            };
            struct Lose : w4::fsm::FuncState<Events>
            {
            };
            struct GameRestarting : w4::fsm::FuncState<Events>
            {
                float m_timeout;
            };
            struct FinalRestarting : w4::fsm::FuncState<Events>
            {
                float m_timeout;
            };
        };
        using Type = w4::fsm::FSM<
            w4::fsm::Transition<Events::Initialize, States::Preloading, States::Logo>,
            w4::fsm::Transition<Events::Next, States::Logo, States::Game>,
            w4::fsm::Transition<Events::Lose, States::Game, States::Lose>,

            w4::fsm::Transition<Events::Restart, States::Lose, States::Logo>,
            w4::fsm::Transition<Events::Refresh, States::Game, States::GameRestarting>,
            w4::fsm::Transition<Events::Refresh, States::Lose, States::FinalRestarting>,

            w4::fsm::Transition<Events::Next, States::GameRestarting, States::Game>,
            w4::fsm::Transition<Events::Next, States::FinalRestarting, States::Lose>
        >;
    };

    FSM::Type m_fsm;
    float m_time;
    sptr<KitchenMergeGame> m_game;

    sptr<Audio> bg_audio;

    sptr<LoadingTextures> m_textureLoader;

    bool m_waitResize;
    bool m_isPortrait;

    sptr<FakeLoading> m_fakeLoading;
    sptr<SpriteManager> m_SpriteManager;
};


W4_RUN(KitchenMerge)
