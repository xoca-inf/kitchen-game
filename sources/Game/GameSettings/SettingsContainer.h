#pragma once
#include "W4JSON.h"
#include "W4Framework.h"

template<typename Type>
struct SettingsContainer
{
public:
    void init(const nlohmann::json& data)
    {
        m_dataStore = data.get<std::unordered_map<std::string, Type>>();
    }

    Type operator[](const std::string& key) const
    {
        auto it = m_dataStore.find(key);
        if(it == m_dataStore.end())
        {
            FATAL_ERROR("[RESOURCE ERROR] Unable to find resource key '%s'", key.c_str());
        }
        return it->second;
    }
private:
    std::unordered_map<std::string ,Type> m_dataStore;
};