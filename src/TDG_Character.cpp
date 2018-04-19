#include "TDG_Character.h"

TDG_Character::TDG_Character() : TDG_Entity()
{
    this->moveable = false;
    this->speed = 0.0;
}

TDG_Character::~TDG_Character()
{
    //dtor
}

void TDG_Character::init(Entity entity, EntityTyp typ, bool moveable)
{
    this->TDG_Entity::init(entity, typ);

    this->speed = entity.speed;

    this->moveable = moveable;

    //some pixel of a characters left and right side can overlap other entities
    int overlappingEdgePixel = 2;

    //the collision box hight is half the entity graphics hight
    this->getCBox()->bindToPosition(this->getPos(), overlappingEdgePixel, (int) entity.graphicsHight/2,
                               entity.graphicsWidth - 2*overlappingEdgePixel, entity.graphicsHight - (int) entity.graphicsHight/2);
}

void TDG_Character::move()
{
    MovementStatus status = this->getMovementStatus();
    int posX = this->getPos()->getPosX();
    int posY = this->getPos()->getPosY();
    if(status == m_north)
        this->getPos()->setPosY(posY - this->speed);
    else if(status == m_north_east)
    {
        this->getPos()->setPosY(posY - this->speed);
        this->getPos()->setPosX(posX + this->speed);
    }
    else if(status == m_east)
        this->getPos()->setPosX(posX + this->speed);
    else if(status == m_south_east)
    {
        this->getPos()->setPosY(posY + this->speed);
        this->getPos()->setPosX(posX + this->speed);
    }
    else if(status == m_south)
        this->getPos()->setPosY(posY + this->speed);
    else if(status == m_south_west)
    {
        this->getPos()->setPosY(posY + this->speed);
        this->getPos()->setPosX(posX - this->speed);
    }
    else if(status == m_west)
        this->getPos()->setPosX(posX - this->speed);
    else if(status == m_north_west)
    {
        this->getPos()->setPosY(posY - this->speed);
        this->getPos()->setPosX(posX - this->speed);
    }
}

void TDG_Character::moveBack()
{
    MovementStatus status = this->getMovementStatus();
    int posX = this->getPos()->getPosX();
    int posY = this->getPos()->getPosY();
    if(status == m_north)
        this->getPos()->setPosY(posY + this->speed);
    else if(status == m_north_east)
    {
        this->getPos()->setPosY(posY + this->speed);
        this->getPos()->setPosX(posX - this->speed);
    }
    else if(status == m_east)
        this->getPos()->setPosX(posX - this->speed);
    else if(status == m_south_east)
    {
        this->getPos()->setPosY(posY - this->speed);
        this->getPos()->setPosX(posX - this->speed);
    }
    else if(status == m_south)
        this->getPos()->setPosY(posY - this->speed);
    else if(status == m_south_west)
    {
        this->getPos()->setPosY(posY - this->speed);
        this->getPos()->setPosX(posX + this->speed);
    }
    else if(status == m_west)
        this->getPos()->setPosX(posX + this->speed);
    else if(status == m_north_west)
    {
        this->getPos()->setPosY(posY + this->speed);
        this->getPos()->setPosX(posX + this->speed);
    }
}

bool TDG_Character::collisionWith(TDG_Entity* entity)
{
    if((this->getCBox() == NULL) || (entity->getCBox() == NULL))
        return false;

    return this->getCBox()->collisionWith(entity->getCBox());
}

bool TDG_Character::collisionWith(TDG_Background* background)
{
    if((this->getCBox() == NULL) || (background == NULL))
        return false;

    return this->getCBox()->collisionWith(background);
}

bool TDG_Character::isMoveable()
{
    return this->moveable;
}
