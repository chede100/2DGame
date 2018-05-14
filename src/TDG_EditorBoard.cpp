#include "TDG_EditorBoard.h"

TDG_EditorBoard::TDG_EditorBoard()
{
    this->backg = NULL;
    this->view = NULL;
    this->entityGraphics = NULL;
    this->entities = NULL;
    this->viewPos = NULL;
    this->mouse = NULL;

    this->roomID = 0;
}

TDG_EditorBoard::~TDG_EditorBoard()
{
    if(this->backg != NULL)
        delete this->backg;
    if(this->view != NULL)
        delete this->view;
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
    if(this->entities != NULL)
        delete this->entities;
    if(this->viewPos != NULL)
        delete this->viewPos;
    if(this->mouse != NULL)
        delete this->mouse;
}

bool TDG_EditorBoard::init()
{
    this->mouse = new TDG_Mouse();

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = 420;
    int viewHight = 320;
    this->view->init(viewWidth, viewHight, 352, 224);

    //bind view to center of the window
    this->viewPos = new TDG_Position(160.0, 96.0);

    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->viewPos, 0, 0))
    {
        cout << "Unable to bind field of view!" << endl;
        return false;
    }

    return true;
}

bool TDG_EditorBoard::render(TDG_GUI* gui)
{
    SDL_RenderClear(gui->getRenderer());

    if(this->backg != NULL)
    {
        if(!renderBackground(gui))
            return false;
    }

    if(this->entities != NULL)
    {
        this->entities->render(gui, this->view);
    }

    if(this->mouse != NULL)
    {
        this->mouse->renderSelRect(gui, this->view);
    }

    SDL_RenderPresent(gui->getRenderer());

    return true;
}

bool TDG_EditorBoard::renderBackground(TDG_GUI* gui)
{
    //position of field of view on the game board
    int xPosView = this->view->getPosX();
    int yPosView = this->view->getPosY();

    //render the background at this position, so the field of view keeps its relative path to the game board.
    int backgRenderPosX = xPosView*(-1);
    int backgRenderPosY = yPosView*(-1);

    if(!this->backg->renderAtPos(gui, backgRenderPosX, backgRenderPosY))
    {
        cout << "Unable to render background at Position X: " << backgRenderPosX << " Y: " << backgRenderPosY << "!" << endl;
        return false;
    }

    return true;
}

bool TDG_EditorBoard::createRoom(int id, int width, int hight)
{
    return true;
}

bool TDG_EditorBoard::loadRoom(TDG_GUI* gui, Room* room)
{
    if(this->entities == NULL)
        this->entities = new TDG_EntityHandler();
    else
    {
        this->entities->stopAnimations();
    }

    if(this->entityGraphics == NULL)
        this->entityGraphics = new TDG_StoredEntityAnimations();

    this->roomName = room->roomName;
    this->roomID = room->roomID;

    //Create background
    this->backg = new TDG_Background();
    if(!this->backg->create(gui, room))
    {
        cout << "Unable to create background!" << endl;
        return false;
    }

    //collision enabled
    this->entities->enableCollision(this->backg);

    //*************************************************************************************
    //Load and store all necessary entity graphics (animations from Player, NPC, Objects)
    //Store all npc graphics/animations
    list<Entity>::const_iterator it, e;
    for (it = room->npc.begin(), e = room->npc.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Character, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Character, it->animationID);
    }

    //Store all object graphics/animations
    for (it = room->obj.begin(), e = room->obj.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Object, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Object, it->animationID);
    }
    //***************************************************************************************
    //create all npc
    for (it = room->npc.begin(), e = room->npc.end(); it != e; it++)
    {
        TDG_Character* chara = new TDG_Character();
        chara->init(*it, Character, false);
        //bind npc animations to the npc
        if(!chara->assignAnimations(this->entityGraphics))
        {
            cout << "Unable to bind animations to npc " << it->name << "!" << endl;
            return false;
        }
        //bind cBox to character
        chara->bindCBox();

        //add the npc to the list of all entities
        this->entities->add(chara);
    }

    //create all room objects
    for (it = room->obj.begin(), e = room->obj.end(); it != e; it++)
    {
        TDG_Object* obj = new TDG_Object();
        obj->init(*it, Object);
        //bind object animations to the object
        if(!obj->assignAnimations(this->entityGraphics))
        {
            cout << "Unable to bind animations to obj " << it->name << "!" << endl;
            return false;
        }
        //bind cBox to object
        obj->bindCBox();

        //add the object to the list of all entities
        this->entities->add(obj);
    }


    this->entities->startAnimations();

    return true;
}

bool TDG_EditorBoard::saveRoom()
{
    return true;
}

void TDG_EditorBoard::handleInput(TDG_EventHandler* event)
{
    this->mouse->handleEvent(event->getEvent(), this->entities, this->backg, this->view);
}
