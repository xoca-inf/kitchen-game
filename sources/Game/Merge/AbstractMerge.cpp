#include "AbstractMerge.h"
#include "AbstractItem.h"

AbstractMerge::~AbstractMerge()
{
    W4_LOG_DEBUG("destructor");

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

AbstractMerge::AbstractMerge(vec2 dim)
{
    m_rows = dim.x;
    m_cols = dim.y;

    field.resize(m_rows);
    for (int i = 0; i < m_rows; i++)
        field[i].resize(m_cols, nullptr);

    m_doesValid = true;
}

void AbstractMerge::setPos(vec3 pos)
{
    setWorldTranslation(pos);
    m_x = pos.x;
    m_y = pos.y;
}

void AbstractMerge::setSize(vec3 size)
{
    setWorldScale(size);
    m_width = size.x;
    m_height = size.y;
}

void AbstractMerge::destroyColliders()
{
    for (auto& rows : field)
    {
        for (auto& cols : rows)
        {
            cols->removeCollider("collider");
        }
    }

    m_doesValid = false;
}

void AbstractMerge::destroy()
{
    for (int i = 0; i < field.size(); i++)
    {
        for (int j = 0; j < field[i].size(); j++)
        {
            if (field[i][j] != nullptr)
                removeChild(field[i][j]);
            field[i][j] = nullptr;
        }
        field[i].clear();
    }

    field.clear();
}

void AbstractMerge::initializeHandlers()
{
    m_camera = Render::getScreenCamera();

    m_touchBeginHandler = Touch::Begin::subscribe(std::bind(&AbstractMerge::onTouchBegin, this, std::placeholders::_1));
    m_touchEndHandler = Touch::End::subscribe(std::bind(&AbstractMerge::onTouchEnd, this, std::placeholders::_1));
    m_touchMoveHandler = Touch::Move::subscribe(std::bind(&AbstractMerge::onTouchMove, this, std::placeholders::_1));
}

void AbstractMerge::onTouchBegin(const Touch::Begin& evt)
{
    if (m_doesValid)
    {
        auto ray = m_camera->createRayFromScreen(evt.point);
        ray.length = 10;
        auto hit = Render::raycast(ray);
        if (hit.target)
        {
            auto sprite = hit.target->getParent()->as<AbstractItem>();

            if (sprite && !m_currentNode)
            {
                m_currentNode = sprite;

                auto x = (float)evt.point.x / Platform::getSize().w * 9.0f / 16.0f;
                auto y = (float)evt.point.y / Platform::getSize().h;

                m_prevPoint = vec2(x, y);

                vec2 pos = m_currentNode->getWorldTranslation().xy;
                pos -= vec2(m_x - m_width / 2, m_y - m_height / 2);
                int x_pos = pos.x;
                int y_pos = pos.y;

                m_currentNodeFieldPos = ivec2(x_pos, y_pos);

                m_currentNode->setWorldTranslation(m_currentNode->getWorldTranslation() + vec3(0, 0, -0.1));

                auto visSprite = sprite->getChild<VisibleNode>("sprite");
                visSprite->setRenderOrder(visSprite->getRenderOrder() + 1);
            }
        }
    }
}

void AbstractMerge::onTouchMove(const Touch::Move& evt)
{
    if (m_doesValid && m_currentNode)
    {
        auto x = (float)evt.point.x / Platform::getSize().w * 9.0f / 16.0f;
        auto y = (float)evt.point.y / Platform::getSize().h;

        vec3 delta(x - m_prevPoint.x, -y + m_prevPoint.y, 0);
        m_currentNode->setWorldTranslation(m_currentNode->getWorldTranslation() + delta * 11);

        m_prevPoint = vec2(x, y);
    }
}

void AbstractMerge::onTouchEnd(const Touch::End& evt)
{
    if (m_doesValid)
    {
        auto ray = m_camera->createRayFromScreen(evt.point);
        ray.length = 10;
        auto hit = Render::raycast(ray);
        if (hit.target)
        {
            auto sprite = hit.target->getParent()->as<AbstractItem>();

            if (sprite && m_currentNode)
            {
                m_currentNode->setWorldTranslation(m_currentNode->getWorldTranslation() + vec3(0, 0, 0.1));

                auto visSprite = sprite->getChild<VisibleNode>("sprite");
                visSprite->setRenderOrder(visSprite->getRenderOrder() - 1);

                vec2 pos = m_currentNode->getWorldTranslation().xy;
                pos -= vec2(m_x - m_width / 2, m_y - m_height / 2);
                int x_pos = pos.x;
                int y_pos = pos.y;

                if (x_pos < 0 || x_pos >= m_cols
                    || y_pos < 0 || y_pos >= m_rows
                    || field[x_pos][y_pos] == m_currentNode)
                {
                    m_currentNode->setWorldTranslation(vec3(m_x + (m_currentNodeFieldPos.x - (m_cols - 1) / 2.0) * (m_width / m_cols),
                        m_y - ((m_rows - m_currentNodeFieldPos.y - 1) - (m_rows - 1) / 2.0) * (m_height / m_rows),
                        0) * 0.92f);
                }
                else
                {
                    if (field[x_pos][y_pos] == nullptr)
                    {
                        field[x_pos][y_pos] = field[m_currentNodeFieldPos.x][m_currentNodeFieldPos.y];
                        field[m_currentNodeFieldPos.x][m_currentNodeFieldPos.y] = nullptr;
                        m_currentNode->setWorldTranslation(vec3(m_x + (x_pos - (m_cols - 1) / 2.0) * (m_width / m_cols),
                            m_y - ((m_rows - y_pos - 1) - (m_rows - 1) / 2.0) * (m_height / m_rows),
                            0) * 0.92f);
                    }
                    else if (m_currentNode->type == field[x_pos][y_pos]->type)
                    {
                        field[x_pos][y_pos]->removeCollider("collider");
                        field[x_pos][y_pos]->setEnabled(false);
                        field[x_pos][y_pos]->getParent()->removeChild(field[x_pos][y_pos]);
                        field[x_pos][y_pos] = m_currentNode;
                        field[m_currentNodeFieldPos.x][m_currentNodeFieldPos.y] = nullptr;

                        m_currentNode->setWorldTranslation(vec3(m_x + (x_pos - (m_cols - 1) / 2.0) * (m_width / m_cols),
                            m_y - ((m_rows - y_pos - 1) - (m_rows - 1) / 2.0) * (m_height / m_rows),
                            0) * 0.92f);

                        if (m_currentNode->onSuccess() == StatusCode::destructMergeState)
                        {
                            destruct(m_currentNode);
                        }
                    }
                    else
                    {
                        m_currentNode->onUnsuccess();
                        m_currentNode->setWorldTranslation(vec3(m_x + (m_currentNodeFieldPos.x - (m_cols - 1) / 2.0) * (m_width / m_cols),
                            m_y - ((m_rows - m_currentNodeFieldPos.y - 1) - (m_rows - 1) / 2.0) * (m_height / m_rows),
                            0) * 0.92f);
                    }
                }
            }
            m_currentNode = nullptr;
        }
    }
}

void AbstractMerge::initializeItems(std::vector<sptr<AbstractItem>>& items)
{
    for (int i = 0; i < items.size(); i++)
    {
        int freeSlot = findNextFreeSpace();
        int cur_x = freeSlot / m_cols, cur_y = freeSlot % m_cols;
        field[cur_x][m_rows - cur_y - 1] = items[i];
        items[i]->initialize();

        this->addChild(items[i]);

        auto sprite = items[i];
        sprite->setSize(vec3(m_width / m_cols, m_height / m_rows, 1) * 0.92f);
        sprite->setPos(vec3(m_x + (cur_x - (m_cols - 1) / 2.0) * (m_width / m_cols), m_y - (cur_y - (m_rows - 1) / 2.0) * (m_height / m_rows), 0) * 0.92f);

        auto collider = sprite->addCollider<w4::core::AABB>("collider", Mesh::create::plane(sprite->getWorldScale().xy));
        collider->setReceiveRaycasts(true);
    }

    initializeHandlers();
}

int AbstractMerge::findNextFreeSpace()
{
    int newRandom = random<int>(0, m_rows * m_cols - 1);
    int tmp_x = newRandom / m_cols, tmp_y = newRandom % m_cols;
    while (field[tmp_x][m_rows - tmp_y - 1] != nullptr)
    {
        newRandom = (newRandom + 1) % (m_rows * m_cols);

        tmp_x = newRandom / m_cols;
        tmp_y = newRandom % m_cols;
    }

    return newRandom;
}
