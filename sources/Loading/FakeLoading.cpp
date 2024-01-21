#include "FakeLoading.h"

FakeLoading::~FakeLoading()
{
    Platform::hideLoader();
}

FakeLoading::FakeLoading(sptr<Node> root)
{

}

void FakeLoading::initialize(std::function<void()> onComplete)
{
    m_onComplete = onComplete;

    m_time = 0.0f;

    m_timeout = 0.0f;

    Platform::showLoader();
}

void FakeLoading::shutdownLoader()
{
    m_bg->setEnabled(false);
    Platform::hideLoader();
}

void FakeLoading::update(float dt)
{
    m_time += dt;

    if (m_time >= m_timeout)
    {
        if (m_onComplete)
        {
            shutdownLoader();
            m_onComplete();
            return;
        }
    }
}