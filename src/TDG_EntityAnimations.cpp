#include "TDG_EntityAnimations.h"

TDG_EntityAnimations::TDG_EntityAnimations()
{
    this->aniL = NULL;
    this->entityID = 0;
}

TDG_EntityAnimations::~TDG_EntityAnimations()
{
    TDG_AnimationList* tmp = this->aniL;
    TDG_AnimationList* next;

    while(tmp != NULL)
    {
        next = tmp->getNext();
        delete tmp;
        tmp = next;
    }
}

bool TDG_EntityAnimations::loadAnimations(TDG_GUI* gui, int entityID, EntityTyp typ)
{
    this->entityID = entityID;
    this->typ = typ;

    string folderPath;
    if(typ == Player || typ == NPC)
        folderPath = "./data/img/entity/char/";
    else if(typ == Object)
        folderPath = "./data/img/entity/object/";

    string sName = "";
    ostringstream ss;
    ss << entityID;
    sName += ss.str();

    TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
    if(!sprite->loadSprite(folderPath, sName))
    {
        cout << "Unable to load sprite: " << folderPath << sName << endl;
        return false;
    }

    add(sprite->getAnimation(gui, 1, move_east));
    add(sprite->getAnimation(gui, 2, move_north));
    add(sprite->getAnimation(gui, 3, move_south));
    add(sprite->getAnimation(gui, 4, stand_east));
    add(sprite->getAnimation(gui, 5, stand_north));
    add(sprite->getAnimation(gui, 6, stand_south));

    delete sprite;

    return true;
}

TDG_Animation* TDG_EntityAnimations::getAnimation(AnimationTyp typ)
{
    TDG_AnimationList* tmp = this->aniL;

    while(tmp != NULL)
    {
        if(tmp->getAnimation()->getTyp() == typ)
            return tmp->getAnimation();

        tmp = tmp->getNext();
    }

    return NULL;
}

void TDG_EntityAnimations::add(TDG_Animation* ani)
{
    if(this->aniL == NULL)
    {
        this->aniL = new TDG_AnimationList();
        aniL->setAnimation(ani);
    }
    else
    {
        TDG_AnimationList* tmp = this->aniL;
        TDG_AnimationList* next = this->aniL->getNext();

        while(next != NULL)
        {
            tmp = next;
            next = tmp->getNext();
        }

        TDG_AnimationList* newAni = new TDG_AnimationList();
        newAni->setAnimation(ani);

        tmp->setNext(newAni);
    }
}
