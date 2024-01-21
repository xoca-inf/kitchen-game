#include "InteractiveItem.h"

class InteractiveSink : public InteractiveItem
{
public:
    explicit InteractiveSink(sptr<VisibleNode> node, sptr<VisibleNode> disappear_node) : InteractiveItem(node, disappear_node){};
    void update(float dt) override;
    void onAwake() override;
    void onInteract(std::function<void()>) override;

private:
    sptr<Tween<float>> m_SinkTween;
    float installTween_timeout = 0.5f;
    float installTween_time = 0;
};