#include "TDG_Object.h"

TDG_Object::TDG_Object() : TDG_Entity()
{
    //ctor
}

TDG_Object::~TDG_Object()
{
    //dtor
}

void TDG_Object::init(const Entity* entity, EntityTyp typ, TDG_StoredEntityAnimations* storedGraphics)
{
    this->TDG_Entity::init(entity, typ, storedGraphics);
}
