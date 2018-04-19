#include "TDG_Object.h"

TDG_Object::TDG_Object() : TDG_Entity()
{
    //ctor
}

TDG_Object::~TDG_Object()
{
    //dtor
}

void TDG_Object::init(Entity entity, EntityTyp typ)
{
    this->TDG_Entity::init(entity, typ);

    //the collision box hight is half the entity graphics hight
    this->getCBox()->bindToPosition(this->getPos(), 0, (int) entity.graphicsHight/2,
                               entity.graphicsWidth, entity.graphicsHight - (int) entity.graphicsHight/2);
}
