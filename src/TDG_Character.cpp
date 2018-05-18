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

void TDG_Character::init(const Entity* entity, EntityTyp typ, bool moveable)
{
    this->TDG_Entity::init(entity, typ);

    this->speed = entity->speed;

    this->moveable = moveable;
}

void TDG_Character::moveAndCollision(TDG_EntityList* eList, TDG_Background* bground)
{
    MovementStatus status = this->getMovementStatus();
    int posX = this->getPos()->getPosX();
    int posY = this->getPos()->getPosY();
    int speed = this->speed;

    if(status == m_north)
    {
        if(!collisionDetection(eList, bground, 0, -speed))
            this->getPos()->setPosY(posY - speed);
    }
    else if(status == m_north_east)
    {
        if(!collisionDetection(eList, bground, 0, -speed))
            this->getPos()->setPosY(posY - speed);
        if(!collisionDetection(eList, bground, speed, 0))
                this->getPos()->setPosX(posX + speed);
    }
    else if(status == m_east)
    {
        if(!collisionDetection(eList, bground, speed, 0))
            this->getPos()->setPosX(posX + speed);
    }
    else if(status == m_south_east)
    {
        if(!collisionDetection(eList, bground, 0, speed))
            this->getPos()->setPosY(posY + speed);
        if(!collisionDetection(eList, bground, speed, 0))
                this->getPos()->setPosX(posX + speed);
    }
    else if(status == m_south)
    {
        if(!collisionDetection(eList, bground, 0, speed))
            this->getPos()->setPosY(posY + speed);
    }
    else if(status == m_south_west)
    {
        if(!collisionDetection(eList, bground, 0, speed))
            this->getPos()->setPosY(posY + speed);
        if(!collisionDetection(eList, bground, -speed, 0))
                this->getPos()->setPosX(posX - speed);
    }
    else if(status == m_west)
    {
        if(!collisionDetection(eList, bground, -speed, 0))
            this->getPos()->setPosX(posX - speed);
    }
    else if(status == m_north_west)
    {
        if(!collisionDetection(eList, bground, 0, -speed))
            this->getPos()->setPosY(posY - speed);
        if(!collisionDetection(eList, bground, -speed, 0))
                this->getPos()->setPosX(posX - speed);
    }
}

bool TDG_Character::collisionDetection(TDG_EntityList* eList, TDG_Background* bground, int speedX, int speedY)
{
    //check for background (environment) collision
    if(this->collisionWith(bground, speedX, speedY))
        return true;
    else
    {
        //check collision on all other entities
        TDG_EntityList* col = eList;
        while(col != NULL)
        {
            //dont check collision on itself
            if(col->getEntity() != this)
            {
                //move back on collision detection with other entities
                if(this->collisionWith(col->getEntity(), speedX, speedY))
                    return true;
            }
            col = col->getNext();
        }
    }
    return false;
}

void TDG_Character::adjust(int posX, int posY, MovementStatus status)
{
    this->getPos()->setPosX((double)posX);
    this->getPos()->setPosY((double)posY);
    this->setMovementStatus(status);
}

bool TDG_Character::collisionWith(TDG_Entity* entity, int speedX, int speedY)
{
    if((this->getCBox() == NULL) || (entity->getCBox() == NULL))
        return false;

    return this->getCBox()->collisionWith(entity->getCBox(), speedX, speedY);
}

bool TDG_Character::collisionWith(TDG_Background* background, int speedX, int speedY)
{
    if((this->getCBox() == NULL) || (background == NULL))
        return false;

    return this->getCBox()->collisionWith(background, speedX, speedY);
}

bool TDG_Character::isMoveable()
{
    return this->moveable;
}
