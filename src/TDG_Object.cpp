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
}
