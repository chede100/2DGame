#include "TDG_Entity.h"


TDG_Entity::TDG_Entity()
{
    this->id = 0;
    this->typ = nothing;
    this->moveable = false;
    this->curStatus = noStatus;
    this->pos = new TDG_Position();
    this->animations = NULL;
    this->currentAnimation = NULL;
    this->currentImage = NULL;
    this->cBox = NULL;
}

TDG_Entity::~TDG_Entity()
{
    if(this->cBox != NULL)
        delete this->cBox;
    if(this->pos != NULL)
        delete this->pos;
}

void TDG_Entity::init(Entity entity, EntityTyp typ, bool moveable)
{
    this->name = entity.name;
    this->id = entity.id;
    this->animationID = entity.animationID;
    this->speed = entity.speed;

    this->pos->setPosX((double)entity.posX);
    this->pos->setPosY((double)entity.posY);

    this->typ = typ;
    this->curStatus = entity.firstStatus;
    this->moveable = moveable;

    //init collision box
    this->cBox = new TDG_CollisionBox();

    //some pixel of a characters left and right side can overlapp other entities
    int overlappingEdgePixel;;
    if(typ == Character)
        overlappingEdgePixel = 2;
    else
        overlappingEdgePixel = 0;

    //the collision box hight is half the entity graphics hight
    this->cBox->bindToPosition(this->pos, overlappingEdgePixel, (int) (1/2)*entity.graphicsHight,
                               entity.graphicsWidth - 2*overlappingEdgePixel, entity.graphicsHight - (int) (1/2)*entity.graphicsHight);


}

bool TDG_Entity::assignAnimations(TDG_StoredEntityAnimations* storedGraphics)
{
    //bind entity to a specific collection of animations
    TDG_EntityAnimations* animations = storedGraphics->getEntityAnimations(typ, this->animationID);
    if(animations == NULL)
    {
        cout << "Unable to assign animation for entity " << this->name << "! Animations are not stored for this entity!" << endl;
        return false;
    }
    this->animations = animations;

    //default entity animation (start animation)
    AnimationTyp animation = convertStatusToAnimationTyp(this->curStatus);
    if((this->currentAnimation = this->animations->getAnimation(animation)) == NULL)
    {
        this->currentAnimation = this->animations->getAnimation(stand_south);

        cout << "Start animation not stored. Switching to animation stand_south." << endl;
    }

    if(this->currentAnimation == NULL)
    {
        cout << "Unable to find start animation for entity " << this->name << "!" << endl;
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

void TDG_Entity::render(TDG_GUI* gui, TDG_View* view)
{
    int x = this->pos->getPosX() - view->getPosX();
    int y = this->pos->getPosY() - view->getPosY();
    int width = this->animations->getImagesWidth()*gui->getScaleFactor();
    int hight = this->animations->getImagesHight()*gui->getScaleFactor();

    SDL_Rect rect = {x, y, width, hight};

    //flip the animation image because only the move or stand animations for direction east are stored (west animations not stored)
    if(this->curStatus == m_north_west || this->curStatus == m_west || this->curStatus == m_south_west
       || this->curStatus == s_north_west || this->curStatus == s_west || this->curStatus == s_south_west)
       SDL_RenderCopyEx(gui->getRenderer(), this->currentImage->getImg(), NULL, &rect, 0.0, NULL, SDL_FLIP_VERTICAL);
    else
        SDL_RenderCopyEx(gui->getRenderer(), this->currentImage->getImg(), NULL, &rect, 0.0, NULL, SDL_FLIP_NONE);
}

bool TDG_Entity::updateAnimation()
{
    //shall animation be switched or stay the same?
    AnimationTyp requiredAnimation = convertStatusToAnimationTyp(this->curStatus);
    if(this->currentAnimation->getTyp() != requiredAnimation)
    {
        //switching to a other animation
        this->currentAnimation = this->animations->getAnimation(requiredAnimation);
        if(this->currentAnimation == NULL)
        {
            cout << "Unable to find required animation: " << requiredAnimation << endl;
            return false;
        }
        //start this animation at its first image
        this->currentImage = this->currentAnimation->getFirstImg();
    }
    else
    {
        //if animation is finished start from the beginning
        if(this->currentImage->getNext() == NULL)
            this->currentImage = this->currentAnimation->getFirstImg();
        //next image of animation
        else
            this->currentImage = this->currentImage->getNext();
    }

    return true;
}

void TDG_Entity::changeMovementStatus(Direction dir)
{
    if(dir == noDirection)
    {
        if(this->curStatus == m_north) this->curStatus = s_north;
        else if(this->curStatus == m_north_east) this->curStatus = s_north_east;
        else if(this->curStatus == m_east) this->curStatus = s_east;
        else if(this->curStatus == m_south_east) this->curStatus = s_south_east;
        else if(this->curStatus == m_south) this->curStatus = s_south;
        else if(this->curStatus == m_south_west) this->curStatus = s_south_west;
        else if(this->curStatus == m_west) this->curStatus = s_west;
        else if(this->curStatus == m_north_west) this->curStatus = s_north_west;
    }
    else
    {
        if(dir == north) this->curStatus = m_north;
        else if(dir == north_east) this->curStatus = m_north_east;
        else if(dir == east) this->curStatus = m_east;
        else if(dir == south_east) this->curStatus = m_south_east;
        else if(dir == south) this->curStatus = m_south;
        else if(dir == south_west) this->curStatus = m_south_west;
        else if(dir == west) this->curStatus = m_west;
        else if(dir == north_west) this->curStatus = m_north_west;
    }
}

void TDG_Entity::move()
{
    if(this->curStatus == m_north)
        this->pos->setPosY(this->pos->getPosY() - this->speed);
    else if(this->curStatus == m_north_east)
    {
        this->pos->setPosY(this->pos->getPosY() - this->speed);
        this->pos->setPosX(this->pos->getPosX() + this->speed);
    }
    else if(this->curStatus == m_east)
        this->pos->setPosX(this->pos->getPosX() + this->speed);
    else if(this->curStatus == m_south_east)
    {
        this->pos->setPosY(this->pos->getPosY() + this->speed);
        this->pos->setPosX(this->pos->getPosX() + this->speed);
    }
    else if(this->curStatus == m_south)
        this->pos->setPosY(this->pos->getPosY() + this->speed);
    else if(this->curStatus == m_south_west)
    {
        this->pos->setPosY(this->pos->getPosY() + this->speed);
        this->pos->setPosX(this->pos->getPosX() - this->speed);
    }
    else if(this->curStatus == m_west)
        this->pos->setPosX(this->pos->getPosX() - this->speed);
    else if(this->curStatus == m_north_west)
    {
        this->pos->setPosY(this->pos->getPosY() - this->speed);
        this->pos->setPosX(this->pos->getPosX() - this->speed);
    }
}

bool TDG_Entity::collisionWith(TDG_Entity* entity)
{
    return false;
}

bool TDG_Entity::collisionWith(TDG_Background* background)
{
    return false;
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
