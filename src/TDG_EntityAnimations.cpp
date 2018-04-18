#include "TDG_EntityAnimations.h"

TDG_EntityAnimations::TDG_EntityAnimations()
{
    this->aniL = NULL;
    this->animationID = 0;
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

bool TDG_EntityAnimations::loadAnimations(TDG_GUI* gui, int animationID, EntityTyp typ)
{
    this->animationID = animationID;
    this->typ = typ;

    string folderPath;
    if(typ == Character)
        folderPath = "./data/img/entity/char/";
    else if(typ == Object)
        folderPath = "./data/img/entity/object/";

    string sName = "";
    ostringstream ss;
    ss << animationID;
    sName += ss.str();

    TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
    if(!sprite->loadSprite(folderPath, sName))
    {
        cout << "Unable to load sprite: " << folderPath << sName << endl;
        delete sprite;
        return false;
    }

    //Save width and hight of the entity animation images
    this->image_width = sprite->getImgWidth();
    this->image_hight = sprite->getImgHight();

    //Load all entity animations from sprite sheet and store them.
    int i;
    for(i = 1; i <= sprite->getSpriteMaxRows(); i++)
    {
        TDG_Animation* newAni = sprite->getAnimation(gui, i);
        if(newAni == NULL)
        {
            cout << "Unable to load animation. Row: " << i << " Sprite sheet: " << folderPath << sName << endl;
            delete sprite;
            return false;
        }
        else
            add(newAni);
    }

    delete sprite;

    if(typ == Character)
        cout << "Stored character animation with ID: "<< this->animationID << endl;
    else if(typ == Object)
        cout << "Stored object animation with ID: " << this->animationID << endl;

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

EntityTyp TDG_EntityAnimations::getTyp()
{
    return this->typ;
}

int TDG_EntityAnimations::getID()
{
    return this->animationID;
}

int TDG_EntityAnimations::getImagesWidth()
{
    return this->image_width;
}
int TDG_EntityAnimations::getImagesHight()
{
    return this->image_hight;
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
