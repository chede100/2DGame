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

void TDG_EntityHandler::removeAllExcept(TDG_Entity* expt)
{
    TDG_EntityList* tmp = this->first;
    while(tmp != NULL)
    {
        if(tmp->getEntity() == expt)
        {
            tmp->setEntity(NULL);
            break;
        }

        tmp = tmp->getNext();
    }

    //delete the list
    TDG_EntityList* del = this->first;
    TDG_EntityList* next;
    while(del != NULL)
    {
        next = del->getNext();
        delete del;
        del = next;
    }

    //add the entity to the new list
    this->first = new TDG_EntityList();
    this->first->setEntity(expt);
}

void TDG_EntityHandler::render(TDG_Window* win, TDG_View* view)
{
    //sort before rendering
    sort();

    TDG_EntityList* tmp = this->first;
    while(tmp != NULL)
    {
        if(tmp->getEntity() != NULL)
            tmp->getEntity()->render(win, view);
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

void TDG_EntityHandler::print(EntityTyp typ, ofstream* out)
{
    TDG_EntityList* tmp = this->first;

    while(tmp != NULL)
    {
        if(tmp->getEntity() != NULL)
        {
            if(tmp->getEntity()->getTyp() == typ)
            {
                int id = tmp->getEntity()->getID();
                int x = tmp->getEntity()->getPos()->getPosX();
                int y = tmp->getEntity()->getPos()->getPosY();
                string status = "s_south";
                MovementStatus s = tmp->getEntity()->getMovementStatus();
                if(s == s_south) status = "s_south";
                else if(s == s_south_west) status = "s_south_west";
                else if(s == s_west) status = "s_west";
                else if(s == s_north_west) status = "s_north_west";
                else if(s == s_north) status = "s_north";
                else if(s == s_north_east) status = "s_north_east";
                else if(s == s_east) status = "s_east";
                else if(s == s_south_east) status = "s_south_east";
                *out << id << " " << x << " " << y << " " << status << " ";
            }
        }
        tmp = tmp->getNext();
    }
}

TDG_Entity* TDG_EntityHandler::getEntity(int posX, int posY)
{
    TDG_EntityList* tmp = this->first;
    TDG_EntityList* next;

    while(tmp != NULL)
    {
        next = tmp->getNext();

        if(tmp->getEntity() != NULL)
        {
            int ePosX = tmp->getEntity()->getPos()->getPosX();
            int eWidth = tmp->getEntity()->getImageWidth();

            if((ePosX <= posX) && (ePosX + eWidth >= posX))
            {
                int ePosY = tmp->getEntity()->getPos()->getPosY();
                int eHight = tmp->getEntity()->getImageHight();

                if((ePosY <= posY) && (ePosY + eHight >= posY))
                    return tmp->getEntity();
            }
        }

        tmp = next;
    }

    return NULL;
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

    if(a == NULL || b == NULL)
        return false;

    int aPosY = a->getPos()->getPosY();
    int bPosY = b->getPos()->getPosY();

    //check render priority for character position
    if(aPosY > bPosY)
    {
        if(aPosY + a->getImageHight() > bPosY + b->getImageHight())
        {
            A->setEntity(b);
            B->setEntity(a);

            return true;
        }
    }
    else if(aPosY + a->getImageHight() > bPosY + b->getImageHight())
    {
            A->setEntity(b);
            B->setEntity(a);

            return true;
    }

    return false;
}

void TDG_EntityHandler::moveEntities()
{
    TDG_EntityList* tmp = this->first;
    while(tmp != NULL)
    {
        if(tmp->getEntity() != NULL)
        {
            if(tmp->getEntity()->getTyp() == Character)
            {
                TDG_Character* chara = (TDG_Character*) tmp->getEntity();
                if(chara->isMoveable())
                {
                    chara->moveAndCollision(this->first, this->bg);
                }
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
        if(tmp->getEntity() != NULL)
            tmp->getEntity()->updateAnimation();

        tmp = tmp->getNext();
    }
}

Uint32 TDG_EntityHandler::next_image_timer(Uint32 intervall, void* param)
{
    ((TDG_EntityHandler*)param)->updateAllAnimations();
    return intervall;
}
