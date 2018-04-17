#include "TDG_Entity.h"


TDG_Entity::TDG_Entity() : TDG_Position()
{
    this->id = 0;
    this->typ = nothing;
    this->moveable = false;
    this->newDir = noDirection;
    this->currStatus = noStatus;
    this->animations = NULL;
    this->currentAnimation = NULL;
    this->currentImage = NULL;
    this->cBox = NULL;
}

TDG_Entity::~TDG_Entity()
{
    if(this->cBox != NULL)
        delete this->cBox;
}

bool TDG_Entity::init(Entity entity, EntityTyp typ, bool moveable, TDG_StoredEntityAnimations* storedGraphics)
{
    this->name = entity.name;
    this->id = entity.id;
    this->speed = entity.speed;

    this->setPosX(entity.posX);
    this->setPosY(entity.posY);

    this->typ = typ;
    this->currStatus = entity.firstStatus;
    this->moveable = moveable;

    //bind entity to a collection of animations
    TDG_EntityAnimations* animations = storedGraphics->getEntityAnimations(typ, entity.animationID);
    if(animations == NULL)
    {
        cout << "Unable to initialize entity " << this->name << "! Animations are not stored for this entity!" << endl;
        return false;
    }
    this->animations = animations;

    //default entity animation (start animation)
    AnimationTyp firstAnimation = convertStatusToAnimationTyp(entity.firstStatus);
    if((this->currentAnimation = this->animations->getAnimation(firstAnimation)) == NULL)
    {
        this->currentAnimation = this->animations->getAnimation(stand_south);

        cout << "Start animation not stored. Switching to animation stand_south." << endl;
    }

    if(this->currentAnimation == NULL)
    {
        cout << "Unable to find default animation stand_south and initialize entity " << this->name << "!" << endl;
        return false;
    }

    //start animation at first image
    this->currentImage = this->currentAnimation->getFirstImg();
    if(this->currentImage == NULL)
    {
        cout << "Unable to access first image of start animation for entity " << this->name << "!" << endl;
        return false;
    }

    return true;
}

AnimationTyp TDG_Entity::convertStatusToAnimationTyp(MovementStatus status)
{
    if((status == s_east) || (status == s_north_east) || (status == s_west) || (status == s_north_west) || (status == s_south_east) || (status == s_south_west))
        return stand_east;
    else if(status == s_north)
        return stand_north;
    else if(status == s_south)
        return stand_south;

    else if((status == m_east) || (status == m_north_east) || (status == m_west) || (status == m_north_west) || (status == m_south_east) || (status == m_south_west))
        return move_east;
    else if(status == m_north)
        return move_north;
    else if(status == m_south)
        return move_south;

    return stand_south;
}
