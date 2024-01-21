#include "InteractiveItem.h"

class InteractiveFridge : public InteractiveItem
{
public:
    explicit InteractiveFridge(sptr<VisibleNode> node, sptr<VisibleNode> disappear_node) : InteractiveItem(node, disappear_node){};
    void update(float dt) override;
    void onAwake() override;
    void onInteract(std::function<void()>) override;

private:
    sptr<Tween<vec2>> m_FridgeTween;
    float installTween_timeout = 0.5f;
    float installTween_time = 0;
    sptr<Tween<vec2>> m_BrokenFridgeTween;
    float brokenTween_timeout = 0.5f;
    float brokenTween_time = 0;
};