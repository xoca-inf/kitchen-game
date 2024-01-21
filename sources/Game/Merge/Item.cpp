#include "Item.h"
#include "Game/AudioManager.h"

Item::Item(std::string name, sptr<SpriteManager> manager)
{
    m_manager = manager;
    m_spriteName = name;
    type = name;

    upgradeType = getUpgradeType(type);
}

std::string Item::getUpgradeType(std::string type) {
  std::string upgradeType;

  if (type == "boltorez")
    upgradeType = "noUpgrade";
  else if (type == "toporik")
    upgradeType = "molotok";
  else if (type == "otvertka")
    upgradeType = "gaechnii_klyuch";
  else if (type == "molotok")
    upgradeType = "ploskogubci";
  else if (type == "ploskogubci")
    upgradeType = "noUpgrade";
  else if (type == "gaechnii_klyuch")
    upgradeType = "razvodnoi_klyuch";
  else if (type == "razvodnoi_klyuch")
    upgradeType = "noUpgrade";
  else if (type == "gubka")
    upgradeType = "gubka_high";
  else if (type == "gubka_high")
    upgradeType = "vederko";
  else if (type == "cveti_light")
    upgradeType = "cveti_high";
  else if (type == "cveti_high")
    upgradeType = "noUpgrade";
  else if (type == "kist")
    upgradeType = "valik";
  else if (type == "valik")
    upgradeType = "noUpgrade";
  else if (type == "gubka")
    upgradeType = "gubka_high";
  else if (type == "gubka_high")
    upgradeType = "noUpgrade";
  else if (type == "yaschik_closed")
    upgradeType = "yaschik_open";
  else if (type == "yaschik_open")
    upgradeType = "noUpgrade";
  else if (type == "stameska")
    upgradeType = "nabor_shestigrannikov";
  else if (type == "nabor_shestigrannikov")
    upgradeType = "noUpgrade";
  else if (type == "vederko")
    upgradeType = "noUpgrade";

  return upgradeType;
}

void Item::initialize()
{
    m_self = m_manager->initializeSprite(m_spriteName, true, this);
    m_self->setName("sprite");
}

StatusCode Item::onSuccess()
{
  W4_LOG_ERROR("UpgradeType - %s", upgradeType.c_str());

    type = upgradeType;
    upgradeType = getUpgradeType(upgradeType);
    m_spriteName = type;

    auto emitter = ParticlesEmitter::get("particles/merge_success.part");
    emitter->setWorldScale(getWorldScale() / 100.0f);
    emitter->setWorldTranslation(getWorldTranslation());
    emitter->setRenderOrder(15);
    emitter->setTransformMode(ParticlesEmitterParameters::TransformMode::EMITTER);
    Render::getRoot()->addChild(emitter);
    emitter->start();

    AudioManager::instance().playMergeSound();

    if (upgradeType == "noUpgrade") {
      return StatusCode::destructMergeState;
    }

    removeChild(m_self);

    m_self = m_manager->initializeSprite(type, true, this);
    m_self->setName("sprite");
    m_self->setLocalTranslation(vec3(0, 0, 0));

    return StatusCode::continueMergeState;
}

void Item::onUnsuccess()
{
}
