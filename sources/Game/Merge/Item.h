#pragma once

#include "W4Framework.h"
#include "AbstractItem.h"
#include "../SpriteManager.h"

W4_USE_UNSTRICT_INTERFACE

class Item : public AbstractItem
{
public:
    Item();
    Item(std::string name, sptr<SpriteManager> manager);
    void initialize();
    StatusCode onSuccess() final;
    void onUnsuccess() final;
    sptr<Node> m_self;
    sptr<SpriteManager> m_manager;
    std::string m_spriteName;
private:

    std::string getUpgradeType(std::string type);
};
