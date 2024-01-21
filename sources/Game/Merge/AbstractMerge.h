#pragma once

#include "W4Framework.h"
#include "W4JSON.h"
#include <unordered_map>
#include "AbstractItem.h"

W4_USE_UNSTRICT_INTERFACE


class AbstractMerge : public w4::core::Node
{
public:
    ~AbstractMerge();
    AbstractMerge() = default;
    AbstractMerge(vec2 dim);
    void setPos(vec3 pos);
    void setSize(vec3 size);
    void initializeItems(std::vector<sptr<AbstractItem>>& items);
    virtual void initialize() = 0;
    virtual void destruct(sptr<AbstractItem> finalItem) = 0;
    void destroy();
    void destroyColliders();
private:
    int findNextFreeSpace();
    void initializeHandlers();

    void onTouchBegin(const Touch::Begin& evt);
    void onTouchEnd(const Touch::End& evt);
    void onTouchMove(const Touch::Move& evt);
private:
    sptr<event::Touch::Begin::Handle> m_touchBeginHandler;
    sptr<event::Touch::End::Handle> m_touchEndHandler;
    sptr<event::Touch::Move::Handle> m_touchMoveHandler;

    std::vector<std::vector<sptr<AbstractItem>>> field;

    sptr<AbstractItem> m_currentNode;
    ivec2 m_currentNodeFieldPos;

    unsigned int m_rows, m_cols;
    float m_width = 0, m_height = 0;
    float m_x = 0, m_y = 0;

    vec2 m_prevPoint;

    sptr<Camera> m_camera;

    bool m_doesClicked;
    bool m_doesValid;
};
