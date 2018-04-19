#include "TDG_EntityHandler.h"

TDG_EntityHandler::TDG_EntityHandler()
{
    this->animationTimerRunning = false;
    this->motionTimerRunning = false;
    this->collisionDetection = false;
    this->bg = NULL;

    this->first = new TDG_EntityList();
}

TDG_EntityHandler::~TDG_EntityHandler()
{
    TDG_EntityList* tmp = this->first;
    TDG_EntityList* next;

    while(tmp != NULL)
    {
        next = tmp->getNext();
        delete tmp;
        tmp = next;
    }

    if(this->animationTimerRunning)
        SDL_RemoveTimer(this->animationTimer);

    if(this->motionTimerRunning)
        SDL_RemoveTimer(this->motionTimer);
}

void TDG_EntityHandler::add(TDG_Entity* newEntity)
{
    if(this->first->getEntity() == NULL)
    {
        this->first->setEntity(newEntity);
    }
    else
    {
        TDG_EntityList* next = this->first->getNext();

        this->first->setNext(new TDG_EntityList());
        this->first->getNext()->setNext(next);
        this->first->getNext()->setPrevious(this->first);
        this->first->getNext()->setEntity(newEntity);

        if(next != NULL)
            next->setPrevious(this->first->getNext());
    }
}


bool TDG_EntityHandler::remove(TDG_Entity* entity)
{
    if(entity == NULL)
    {
        cout << "Cant remove null pointer from entity list!" << endl;
        return false;
    }

    TDG_EntityList* tmp = this->first->getNext();

    if(this->first->getEntity() == entity)
    {
        delete this->first;
        tmp->setPrevious(NULL);
        this->first = tmp;

        return true;
    }
    else
    {
        while(tmp != NULL)
        {
            if(tmp->getEntity() == entity)
            {
                TDG_EntityList* next = tmp->getNext();
                TDG_EntityList* previous =  tmp->getPrevious();

                delete tmp;

                next->setPrevious(previous);
                previous->setNext(next);

                return true;
            }
            tmp = tmp->getNext();
        }
    }
    return false;
}

void TDG_EntityHandler::render(TDG_GUI* gui, TDG_View* view)
{
    //sort before rendering
    sort();

    TDG_EntityList* tmp = this->first;
    while(tmp != NULL)
    {
        tmp->getEntity()->render(gui, view);
        tmp = tmp->getNext();
    }
}

void TDG_EntityHandler::startAnimations()
{
    if(!this->animationTimerRunning)
    {
        this->animationTimer = SDL_AddTimer(125, &next_image_timer, this);
        this->animationTimerRunning = true;
    }
}

void TDG_EntityHandler::stopAnimations()
{
    if(this->animationTimerRunning)
        SDL_RemoveTimer(this->animationTimer);

    this->animationTimerRunning = false;
}

void TDG_EntityHandler::startMotion()
{
    if(!this->motionTimerRunning)
    {
        this->motionTimer = SDL_AddTimer(20, &motion_timer, this);
        this->motionTimerRunning = true;
    }
}

void TDG_EntityHandler::stopMotion()
{
    if(this->motionTimerRunning)
        SDL_RemoveTimer(this->motionTimer);

    this->motionTimerRunning = false;
}

void TDG_EntityHandler::enableCollision(TDG_Background* bg)
{
    this->bg = bg;
    this->collisionDetection = true;
}

void TDG_EntityHandler::disableCollision()
{
    this->collisionDetection = false;
}

void TDG_EntityHandler::sort()
{
    TDG_EntityList* A = this->first;

    if(A != NULL)
    {
        TDG_EntityList* B = this->first->getNext();

        bool finished = false;

        while(!finished)
        {
            finished = true;

            while(B != NULL)
            {
                if(swap(A, B))
                    finished = false;

                A = B;
                B = A->getNext();
            }
            A = this->first;
            B = this->first->getNext();
        }
    }
}

bool TDG_EntityHandler::swap(TDG_EntityList* A, TDG_EntityList* B)
{
    TDG_Entity* a = A->getEntity();
    TDG_Entity* b = B->getEntity();

    if(a->getPos()->getPosY() > b->getPos()->getPosY())
    {
        A->setEntity(b);
        B->setEntity(a);

        return true;
    }

    return false;
}

bool TDG_EntityHandler::collisionDetectionFor(TDG_Character* chara)
{
    //check for background (environment) collision
    if(chara->collisionWith(this->bg))
        return true;
    else
    {
        //check collision on all other entities
        TDG_EntityList* col = this->first;
        while(col != NULL)
        {
            //dont check collision on itself
            if(col->getEntity() != chara)
            {
                //move back on collision detection with other entities
                if(chara->collisionWith(col->getEntity()))
                    return true;
            }
            col = col->getNext();
        }
    }
    return false;
}

void TDG_EntityHandler::moveEntities()
{
    TDG_EntityList* tmp = this->first;
    while(tmp != NULL)
    {
        if(tmp->getEntity()->getTyp() == Character)
        {
            TDG_Character* chara = (TDG_Character*) tmp->getEntity();
            if(chara->isMoveable())
            {
                chara->move();

                //is collision detection enabled and does the character collide with anything?
                if(this->collisionDetection && collisionDetectionFor(chara))
                    chara->moveBack();
            }
        }
        tmp = tmp->getNext();
    }
}

Uint32 TDG_EntityHandler::motion_timer(Uint32 intervall, void* param)
{
    ((TDG_EntityHandler*)param)->moveEntities();
    return intervall;
}

void TDG_EntityHandler::updateAllAnimations()
{
    TDG_EntityList* tmp = this->first;
    while(tmp != NULL)
    {
        tmp->getEntity()->updateAnimation();

        tmp = tmp->getNext();
    }
}

Uint32 TDG_EntityHandler::next_image_timer(Uint32 intervall, void* param)
{
    ((TDG_EntityHandler*)param)->updateAllAnimations();
    return intervall;
}
