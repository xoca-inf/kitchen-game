#pragma once

#include "W4Framework.h"
#include "W4JSON.h"
#include <unordered_map>

W4_USE_UNSTRICT_INTERFACE

enum StatusCode
{
    destructMergeState,
    continueMergeState
};

class AbstractItem : public w4::core::Node
{
public:
    void setPos(vec3 pos);
    void setSize(vec3 size);
    virtual void initialize() = 0;
    virtual StatusCode onSuccess() = 0;
    virtual void onUnsuccess() = 0;
    
    sptr<w4::core::Collider> m_collider;

    std::string type;
    std::string upgradeType;
};
