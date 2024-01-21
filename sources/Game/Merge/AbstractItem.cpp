#include "AbstractItem.h"

void AbstractItem::setPos(vec3 pos)
{
    setWorldTranslation(pos);
}

void AbstractItem::setSize(vec3 size)
{
    setWorldScale(size);
}
