#pragma once
#include "W4JSON.h"
#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

struct Settings
{
    std::string orientation = "portrait";
    size guiResolution = size(1, 1);
    size aspect = size(9, 16);
    bool editorMode;
};

namespace nlohmann
{
    template<>
    struct adl_serializer<Settings>
    {
        static void to_json(json& j, const Settings& obj)
        {
            j["orientation"] = obj.orientation;
            j["guiResolution"] = obj.guiResolution;
        }

        static void from_json(const json& j, Settings& obj)
        {
            obj.orientation = j["orientation"];
            obj.guiResolution = j["guiResolution"];
        }
    };
}
