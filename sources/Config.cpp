#include "Config.h"

W4_JS_IMPORT int getWindowSizeWidth();
W4_JS_IMPORT int getWindowSizeHeight();
W4_JS_IMPORT void changeBackground(std::string);

Settings Config::m_settings;

void Config::initialize()
{
    int w = getWindowSizeWidth();
    int h = getWindowSizeHeight();

    W4_LOG_ERROR("h - %d, w - %d", h, w);
    // ×ÒÎÁÛ ÄÈÌÀ ÍÀÑ ÍÅ ÓÁÈË
    w = 1080;
    h = 1920;
    // -----------------------

    if (h >= w && m_settings.orientation == "landscape")
    {
        m_settings.orientation = "portrait";
        m_settings.aspect = size(9, 16);
        m_settings.guiResolution = size(1080, 1920);
    }
    else if (h < w && m_settings.orientation == "portrait")
    {
        m_settings.orientation = "landscape";
        m_settings.aspect = size(16, 9);
        m_settings.guiResolution = size(1920, 1080);
    }
    else
        return;

    w4::platform::Platform::setWindowStyle(m_settings.orientation.c_str());
    w4::platform::Platform::setWindowAspect(m_settings.aspect.w, m_settings.aspect.h);
    w4::gui::setVirtualResolution(m_settings.guiResolution);

    changeBackground("#212d43");
}

const Settings& Config::getGameSettings()
{
    return m_settings;
}
