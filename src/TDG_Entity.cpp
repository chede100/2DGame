#include "TDG_Entity.h"


TDG_Entity::TDG_Entity()
{
    this->id = 0;
    this->typ = nothing;
    this->curStatus = noStatus;
    this->pos = new TDG_Position();
    this->animations = NULL;
    this->currentAnimation = NULL;
    this->currentImage = NULL;
    this->cBox = NULL;
    this->inv = NULL;
}

TDG_Entity::~TDG_Entity()
{
    if(this->cBox != NULL)
        delete this->cBox;
    if(this->pos != NULL)
        delete this->pos;
    if(this->inv != NULL)
        delete this->inv;
}

void TDG_Entity::init(const Entity* entity, EntityTyp typ, TDG_StoredEntityAnimations* storedGraphics)
{
    this->name = entity->name;
    this->id = entity->id;
    this->animationID = entity->animationID;
    this->scale = entity->scale;

    this->pos->setPosX((double)entity->posX);
    this->pos->setPosY((double)entity->posY);

    this->typ = typ;
    this->curStatus = entity->firstStatus;

    if(assignAnimations(storedGraphics))
    {
        //init collision box
        if((entity->width != 0) && (entity->hight != 0))
        {
            this->cBox = new TDG_CollisionBox();

            this->cBox->setSize(entity->width, entity->hight);

            int xCorrection = (this->animations->getImagesWidth()*this->scale - this->cBox->getWidth())/2;
            int yCorrection = this->animations->getImagesHight()*this->scale - this->cBox->getHight();

            //bind cBox to the position of the entity
            this->getCBox()->bindToPosition(this->getPos(), xCorrection, yCorrection);
        }
    }
    else
        cout << "Unable to find graphics for entity! ID: " << entity->id << endl;
}

void TDG_Entity::createInventory(TDG_Window* win, const Entity* entity)
{
    this->inv = new TDG_Inventory();
    this->inv->create(entity->inventorySize);

    list<Item>::const_iterator it, e;
    for (it = entity->inventory.begin(), e = entity->inventory.end(); it != e; it++)
    {
        this->inv->add(win, it->name, it->id, it->amount, it->maxAmount);
    }
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

void TDG_Entity::render(TDG_Window* win, TDG_View* view)
{
    int x = this->pos->getPosX() - view->getPosX();
    int y = this->pos->getPosY() - view->getPosY();
    int width = this->animations->getImagesWidth()*this->scale;
    int hight = this->animations->getImagesHight()*this->scale;

    SDL_Rect rect = {x, y, width, hight};

    //flip the animation image because only the move or stand animations for direction east are stored (west animations not stored)
    if((this->curStatus == m_north_west) || (this->curStatus == m_west) || (this->curStatus == m_south_west)
       || (this->curStatus == s_north_west) || (this->curStatus == s_west) || (this->curStatus == s_south_west))
       SDL_RenderCopyEx(win->getRenderer(), this->currentImage->getImg(), NULL, &rect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
    else
        SDL_RenderCopy(win->getRenderer(), this->currentImage->getImg(), NULL, &rect);
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

TDG_CollisionBox* TDG_Entity::getCBox()
{
    return this->cBox;
}

MovementStatus TDG_Entity::getMovementStatus()
{
    return this->curStatus;
}

TDG_Position* TDG_Entity::getPos()
{
    return this->pos;
}

EntityTyp TDG_Entity::getTyp()
{
    return this->typ;
}

int TDG_Entity::getImageWidth()
{
    return this->animations->getImagesWidth()*this->scale;
}

int TDG_Entity::getImageHight()
{
    return this->animations->getImagesHight()*this->scale;
}

TDG_ImageList* TDG_Entity::getCurrentImage()
{
    return this->currentImage;
}

int TDG_Entity::getAnimationID()
{
    return this->animationID;
}

int TDG_Entity::getID()
{
    return this->id;
}

TDG_Inventory* TDG_Entity::getInventory()
{
    return this->inv;
}

void TDG_Entity::setMovementStatus(MovementStatus status)
{
    this->curStatus = status;
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
