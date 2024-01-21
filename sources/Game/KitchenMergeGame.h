#pragma once

#include "W4Framework.h"
#include "Game/SpriteManager.h"

#include "../Config.h"
#include <chrono>
#include <thread>

W4_USE_UNSTRICT_INTERFACE

class KitchenMergeGame
{
public:
  // enums
    // enums
  enum MergeTypes {
    Fridge,
    Sink,
    Plate
  };

  enum GameState {
    Playing,
    Lose
  };

public:
    KitchenMergeGame() = default;
    ~KitchenMergeGame();
    void initialize(std::string screenConfigPath, std::string orientation, sptr<SpriteManager> SpriteManager);
    void update(float dt);
    GameState getState();
    void changeOrientation(std::string orientation);
private:
    void initializeSprites();
    void initializeHandlers();
    void initializeMerge(MergeTypes type);
    void initializeCollisions(sptr<Node> selectedItem, sptr<Node> intersectItem,  std::function<void()> onIntersect);
    void onFridgeIntersect();
    void initializeButtons();

    void onTouchBegin(const Touch::Begin& evt);
    void onTouchEnd(const Touch::End& evt);
    void onTouchMove(const Touch::Move& evt);

    void onCollision();
protected:
    sptr<Node> m_root;

private:
    GameState m_state;

    sptr<event::Touch::Begin::Handle> m_touchBeginHandler;
    sptr<event::Touch::End::Handle> m_touchEndHandler;
    sptr<event::Touch::Move::Handle> m_touchMoveHandler;

    std::string m_configPath;
    bool m_currentOrientation;
    sptr<Camera> m_camera;

    sptr<SpriteManager> m_spriteManager;

    sptr<class Merge> m_merge;
    sptr<class Interaction> m_interaction;

    sptr<VisibleNode> m_kitchen;
    sptr<VisibleNode> m_brokenFridge;
    sptr<VisibleNode> m_fridge;
    sptr<VisibleNode> m_brokenSink;
    sptr<VisibleNode> m_sink;
    sptr<VisibleNode> m_plate;
    sptr<VisibleNode> m_workPlace;
    sptr<VisibleNode> m_win;
    sptr<VisibleNode> m_boom;
    sptr<VisibleNode> m_fail;
    sptr<VisibleNode> m_happy;
    sptr<VisibleNode> m_normal;
    sptr<VisibleNode> m_boltorez;
    sptr<VisibleNode> m_ploskogubci;
    sptr<VisibleNode> m_fire;
    sptr<VisibleNode> m_vederko;

    sptr<Node> m_SelectedItem;
    sptr<Collider> m_selectedItem_collider;

    sptr<Node> m_IntersectItem;
    sptr<Collider> m_intersect_collider;

    sptr<Tween<vec2>> m_FridgeTween;
    float installTween_timeout = 0.5f;
    float installTween_time = 0;

    float m_totalTime = 0.0f;

    int m_countMerge = 0;
    bool m_isMergingNow = false;
};
